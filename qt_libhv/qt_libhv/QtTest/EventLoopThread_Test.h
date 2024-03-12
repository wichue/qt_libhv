#ifndef EVENTLOOPTHREAD_TEST_H
#define EVENTLOOPTHREAD_TEST_H

#include "hv.h"

#include "EventLoopThread.h"

using namespace hv;

static void onTimer(TimerID timerID, int n) {
    printf("tid=%ld timerID=%lu time=%lus n=%d\n", hv_gettid(), (unsigned long)timerID, (unsigned long)time(NULL), n);
}

/**
 * @brief EventLoopThread 事件循环线程测试
 * 事件循环线程是一个网络框架的核心， libhv 把事件循环(EventLoopPtr)和线程(std::thread)分开封装，可以先创建 EventLoopPtr ，再创建 EventLoopThread 启动线程。
 * EventLoopThread 构建时可以传入已有的 EventLoopPtr ，如果不传则自动创建一个 EventLoopPtr ;也可传入Functor pre post ，分别在循环前和循环后执行。
 * EventLoopThread 可以暂停(HLOOP_STATUS_PAUSE 线程还在，只是不处理任务)，或无激活事件时退出线程，或只执行一次(HLOOP_FLAG_RUN_ONCE)，或一直执行。
 *
 * libhv 事件循环线程主要处理3种任务： 定时器，网络IO，自定义事件。
 * 1、定时器：支持创建单次或循环定时器，存放在小顶堆(timers)中，堆顶即是最先超时的定时器，每次事件循环检测堆顶，超时的定时器会先出堆再重新入堆， libhv 会优先处理定时器任务。
 *          事件循环检测堆顶定时器后，会把下一次定时器超时时长 blocktime_ms 传入 epoll_wait ，避免过渡执行循环,如果没有定时器则 epoll_wait 默认100ms超时。
 * 2、自定义任务：使用 epoll 实现事件监听，事件循环刚启动时创建事件(eventfds)和 epoll，调用 runInLoop 把自定义事件加入队列 customEvents ，并通过 eventfds 唤醒 epoll 执行循环。
 * 3、网络IO：使用和自定义事件同一个 epoll 监听网络IO事件，比如创建tcp客户端时把fd传入epoll监听。
 *
 * 创建epoll调用栈:EventLoopThread::start->EventLoopThread::loop_thread->EventLoop::run->hloop_run->hloop_create_eventfds->hread->hio_read->hio_add->iowatcher_add_event->iowatcher_init->epoll_create.
 * epoll_wait调用栈:EventLoopThread::start->EventLoopThread::loop_thread->EventLoop::run->hloop_run->hloop_process_events->hloop_process_ios->iowatcher_poll_events.
 */

/**
 * @brief 定时器测试
 * 可以在 EventLoopThread 内添加定时器，也可以创建一个事件循环线程(TimerThread)专门用来执行定时器,定时器被存放在小顶堆(heap)中,堆顶就是最先超时的定时器.
 * 创建定时器时会生成 TimerID(uint64_t 原子) 作为定时器的唯一标识,停止定时器和重启定时器需要传参 TimerID.
 * 添加定时器调用栈:任意线程->EventLoop::setInterval->setTimerInLoop->runInLoop->添加自定义事件调用栈->事件循环线程->EventLoop::setTimer->htimer_add(hloop_t::timers)->添加到 EventLoop::timers
 * 定时器超时调用栈:EventLoopThread::loop_thread->EventLoop::run->hloop_run->hloop_process_events->hloop_process_timers->__hloop_process_timers->EVENT_PENDING(timer) 加入优先级队列->hloop_process_pendings 执行回调
 *
 * 刚添加定时器时不会立即更新 epoll_wait 超时时长,默认还是100ms超时(建议无事件时 epoll_wait 一直睡眠,添加定时器时立即唤醒一次线程更新 epoll_wait 超时时长).
 */

/**
 * @brief 自定义事件
 * EventLoop::runInLoop 把自定义事件函数加入 EventLoop 的 customEvents 队列，使用eventfds唤醒线程执行，有锁线程安全，可以在任意线程调用。
 * 添加自定义事件调用栈： EventLoop::runInLoop->queueInLoop->postEvent->hloop_post_event->event_queue_push_back(&loop->custom_events, ev)
 * postEvent 传入回调(cb)，使用cb创建 EventPtr ev， ev 加入 EventLoop 的 customEvents ， ev->event 加入 loop->custom_events。
 * 唤醒循环调用栈：EventLoopThread::loop_thread->EventLoop::run->hloop_run->hloop_process_events->hloop_process_ios->iowatcher_poll_events->EVENT_PENDING(io) 加入待处理队列 pendings
 * 待处理队列调用栈：EventLoopThread::loop_thread->EventLoop::run->hloop_run->hloop_process_events->hloop_process_pendings->cur->cb(cur) 执行回调
 * hloop_s::pendings(待处理优先级队列),是个二维数组,每个优先级一个数组,优先处理高优先级的数组.
 */
int EventLoopThread_Test() {
    HV_MEMCHECK;

    printf("main tid=%ld\n", hv_gettid());

    EventLoopThread loop_thread;
    const EventLoopPtr& loop = loop_thread.loop();

    // runEvery 1s
    loop->setInterval(1000, std::bind(onTimer, std::placeholders::_1, 100));

    // runAfter 10s
    loop->setTimeout(10000, [&loop](TimerID timerID){
        loop->stop();
    });

    loop_thread.start();

    loop->queueInLoop([](){
        printf("queueInLoop tid=%ld\n", hv_gettid());
    });

    loop->runInLoop([](){
        printf("runInLoop tid=%ld\n", hv_gettid());
    });

    // wait loop_thread exit
    loop_thread.join();

    return 0;
}

#endif // EVENTLOOPTHREAD_TEST_H
