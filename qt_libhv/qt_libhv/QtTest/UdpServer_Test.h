#ifndef UDPSERVER_TEST_H
#define UDPSERVER_TEST_H

#include <iostream>

#include "UdpServer.h"

using namespace hv;

int UdpServer_Test() {
    int port = 9090;

    UdpServer srv;
    //创建udp服务端，绑定IP和端口号
    int bindfd = srv.createsocket(port);
    if (bindfd < 0) {
        return -20;
    }
    printf("server bind on port %d, bindfd=%d ...\n", port, bindfd);

    //接收回调
    //UdpServer 不记录接收到消息的历史udp客户端， io->peeraddr 只记录上一次接收到消息的udp客户端
    //channel->write 是发消息给 io->peeraddr
    //UdpServer::sendto 默认发给 io->peeraddr ，指定 peeraddr 时是发给指定的udp客户端。
    srv.onMessage = [](const SocketChannelPtr& channel, Buffer* buf) {
        // echo
        printf("< %.*s\n", (int)buf->size(), (char*)buf->data());
        channel->write(buf);
    };

    //启动 EventLoopThread 线程
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
            srv.sendto(str);
        }
    }

    return 0;
}

#endif // UDPSERVER_TEST_H
