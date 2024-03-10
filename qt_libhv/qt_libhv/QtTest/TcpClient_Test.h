#ifndef TCPCLIENT_TEST_H
#define TCPCLIENT_TEST_H

#include <iostream>

#include "TcpClient.h"
#include "htime.h"

#define TEST_RECONNECT  1
#define TEST_TLS        0

using namespace hv;

int TcpClient_Test() {
    const char* remote_host = "192.168.3.91";
    int remote_port = 9090;

    TcpClient cli;

    //设置tcp组包策略为固定分隔符，只有收到OK时才会调用 onMessage 接收回调函数；
    //如果不设置 setUnpack ，则收到消息会直接调用 onMessage 回调。
    unpack_setting_t OK_unpack_setting;
    memset(&OK_unpack_setting, 0, sizeof(unpack_setting_t));
    OK_unpack_setting.package_max_length = DEFAULT_PACKAGE_MAX_LENGTH;
    OK_unpack_setting.mode = UNPACK_BY_DELIMITER;
    OK_unpack_setting.delimiter[0] = 'O';
    OK_unpack_setting.delimiter[1] = 'K';
    OK_unpack_setting.delimiter_bytes = 2;
    cli.setUnpack(&OK_unpack_setting);

    int connfd = cli.createsocket(remote_port, remote_host);
    if (connfd < 0) {
        return -20;
    }
    printf("client connect to port %d, connfd=%d ...\n", remote_port, connfd);

    //连接成功或断开连接回调
    cli.onConnection = [&cli](const SocketChannelPtr& channel) {
        std::string peeraddr = channel->peeraddr();
        if (channel->isConnected()) {
            printf("connected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
            // send(time) every 3s
            //当连接成功时创建定时器，周期性向服务器发送消息
            setInterval(3000, [channel](TimerID timerID){
                if (channel->isConnected()) {
                    if (channel->isWriteComplete()) {
                        char str[DATETIME_FMT_BUFLEN] = {0};
                        datetime_t dt = datetime_now();
                        datetime_fmt(&dt, str);
                        channel->write(str);
                    }
                } else {
                    killTimer(timerID);
                }
            });
        } else {
            printf("disconnected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
        }
        if (cli.isReconnect()) {
            printf("reconnect cnt=%d, delay=%d\n", cli.reconn_setting->cur_retry_cnt, cli.reconn_setting->cur_delay);
        }
    };

    //接收消息回调
    cli.onMessage = [](const SocketChannelPtr& channel, Buffer* buf) {
        printf("< %.*s\n", (int)buf->size(), (char*)buf->data());
    };

    //连接失败或断开时会重连，重连周期可设置固定、线性、指数等策略
#if TEST_RECONNECT
    // reconnect: 1,2,4,8,10,10,10...
    reconn_setting_t reconn;
    reconn_setting_init(&reconn);
    reconn.min_delay = 1000;
    reconn.max_delay = 10000;
    reconn.delay_policy = 2;
    cli.setReconnect(&reconn);
#endif

#if TEST_TLS
    cli.withTLS();
#endif

    cli.start();

    std::string str;
    while (std::getline(std::cin, str)) {
        if (str == "close") {
            cli.closesocket();
        } else if (str == "start") {
            cli.start();
        } else if (str == "stop") {
            cli.stop();
            break;
        } else {
            if (!cli.isConnected()) break;
            cli.send(str);
        }
    }

    return 0;
}

#endif // TCPCLIENT_TEST_H
