#include "mainwindow.h"

#include <QApplication>
#include "QtTest/EventLoopThread_Test.h"
#include "QtTest/TcpClient_Test.h"
#include "QtTest/TcpServer_Test.h"
#include "QtTest/UdpClient_Test.h"
#include "QtTest/UdpServer_Test.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    setbuf(stdout,nullptr);
//    MainWindow w;
//    w.show();

//    EventLoopThread_Test();
//    TcpClient_Test();
//    TcpServer_Test();
//    UdpClient_Test();
    UdpServer_Test();

    return a.exec();
}
