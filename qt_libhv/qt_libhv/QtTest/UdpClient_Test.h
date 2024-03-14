#ifndef UDPCLIENT_TEST_H
#define UDPCLIENT_TEST_H

#include <iostream>

#include "UdpClient.h"
#include "htime.h"

using namespace hv;

int UdpClient_Test() {
    const char* remote_host = "192.168.3.91";
    int remote_port = 9090;

    UdpClient cli;
    //创建udp客户端，指定远端IP和端口号，本地端口号随机分配
    int sockfd = cli.createsocket(remote_port, remote_host);
    if (sockfd < 0) {
        return -20;
    }

    //接收回调
    printf("client sendto port %d, sockfd=%d ...\n", remote_port, sockfd);
    cli.onMessage = [](const SocketChannelPtr& channel, Buffer* buf) {
        printf("< %.*s\n", (int)buf->size(), (char*)buf->data());
    };

    //UdpClient 构建可传入已有EventLoopThread，不传入时自动创建一个 EventLoopThread。
    cli.start();

    // sendto(time) every 3s
    //创建定时器，间隔向对端发送消息
    cli.loop()->setInterval(3000, [&cli](TimerID timerID) {
        char str[DATETIME_FMT_BUFLEN] = {0};
        datetime_t dt = datetime_now();
        datetime_fmt(&dt, str);
        cli.sendto(str);
    });

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
            cli.sendto(str);
        }
    }

    return 0;
}

#endif // UDPCLIENT_TEST_H
