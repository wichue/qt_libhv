#ifndef TCPSERVER_TEST_H
#define TCPSERVER_TEST_H

#include <iostream>

#include "TcpServer.h"

using namespace hv;

#define TEST_TLS        0

/**
* @brief tcp服务端测试
* TcpServer 继承于 EventLoopThread ,tcp监听 startAccept 使用这个继承的 EventLoopThread 线程;
* TcpServer 的基类 TcpServerEventLoopTmpl 拥有成员变量 EventLoopThreadPool,接入的tcp客户端分配到 EventLoopThreadPool.
* 新接入tcp客户端会创建 TSocketChannelPtr,存入 std::map<uint32_t, TSocketChannelPtr> channels 成员容器.
*
* 启动监听:任意线程->TcpServer::createsocket->Listen->ListenFD->系统调用 listen
* 监听tcp接入事件:EventLoop::runInLoop->...->TcpServerEventLoopTmpl::startAccept->haccept->hio_accept->hio_add->iowatcher_add_event,把 listenfd 添加到 epoll 监听.
* tcp客户端接入:tcp监听所在的 EventLoopThread->EventLoop::run->hloop_run->hloop_process_events->hloop_process_pendings->hio_handle_events->nio_accept(accept)->__accept_cb->hio_accept_cb->TcpServerEventLoopTmpl::onAccept,
* 在分配的 EventLoopThreadPool 线程执行->runInLoop->EventLoop::run->...->TcpServerEventLoopTmpl::newConnEvent->TcpServerEventLoopTmpl::onConnection,调用自定义的回调函数,tcp新接入和断开时均调用该回调函数.
*
* 接收数据:EventLoop::run->hloop_run->hloop_process_events->hloop_process_pendings->hio_handle_events->nio_read->__read_cb->hio_handle_read->hio_read_cb->Channel::on_read
* ->TcpServerEventLoopTmpl::newConnEvent->TcpServerEventLoopTmpl::onMessage,调用自定义接收回调,运行在分配的 EventLoopThreadPool 线程
*
* 数据发送:和tcp客户端类似,使用 SocketChannel::write 发送,建议使用分配的 EventLoopThreadPool 线程发送,也可在任意线程发送,线程安全锁 hio_t::write_mutex
*/
int TcpServer_Test() {
    int port = 9090;

    hlog_set_level(LOG_LEVEL_DEBUG);

    TcpServer srv;
    int listenfd = srv.createsocket(port);
    if (listenfd < 0) {
        return -20;
    }
    printf("server listen on port %d, listenfd=%d ...\n", port, listenfd);

    //当有客户端接入和断开时执行回调
    srv.onConnection = [](const SocketChannelPtr& channel) {
        std::string peeraddr = channel->peeraddr();
        if (channel->isConnected()) {
            printf("%s connected! connfd=%d id=%d tid=%ld\n", peeraddr.c_str(), channel->fd(), channel->id(), currentThreadEventLoop->tid());
        } else {
            printf("%s disconnected! connfd=%d id=%d tid=%ld\n", peeraddr.c_str(), channel->fd(), channel->id(), currentThreadEventLoop->tid());
        }
    };

    //收到客户端消息回调
    srv.onMessage = [](const SocketChannelPtr& channel, Buffer* buf) {
        // echo
        printf("< %.*s\n", (int)buf->size(), (char*)buf->data());
        channel->write(buf);
    };

    //设置EventLoopThreadPool     worker_threads 线程数量
    srv.setThreadNum(4);
    //设置线程分配策略，最小连接数优先
    srv.setLoadBalance(LB_LeastConnections);

#if TEST_TLS
    hssl_ctx_opt_t ssl_opt;
    memset(&ssl_opt, 0, sizeof(hssl_ctx_opt_t));
    ssl_opt.crt_file = "cert/server.crt";
    ssl_opt.key_file = "cert/server.key";
    ssl_opt.verify_peer = 0;
    srv.withTLS(&ssl_opt);
#endif

    //启动监听线程和任务线程池
    srv.start();

    std::string str;
    while (std::getline(std::cin, str)) {
        if (str == "close") {
            srv.closesocket();
        } else if (str == "start") {
            srv.start();
        } else if (str == "stop") {
            srv.stop();
            break;
        } else {
            srv.broadcast(str.data(), str.size());
        }
    }

    return 0;
}

#endif // TCPSERVER_TEST_H
