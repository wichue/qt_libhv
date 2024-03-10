QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DESTDIR = bin
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HOME_BASE=../../
INCLUDEPATH += $$HOME_BASE
INCLUDEPATH += $$HOME_BASE/base
INCLUDEPATH += $$HOME_BASE/cpputil
INCLUDEPATH += $$HOME_BASE/event
INCLUDEPATH += $$HOME_BASE/evpp
INCLUDEPATH += $$HOME_BASE/http
INCLUDEPATH += $$HOME_BASE/misc
INCLUDEPATH += $$HOME_BASE/mqtt
INCLUDEPATH += $$HOME_BASE/protocol
INCLUDEPATH += $$HOME_BASE/ssl
INCLUDEPATH += $$HOME_BASE/util


SOURCES += \
    ../../base/hbase.c \
    ../../base/herr.c \
    ../../base/hlog.c \
    ../../base/hmain.c \
    ../../base/hsocket.c \
    ../../base/htime.c \
    ../../base/hversion.c \
    ../../base/rbtree.c \
    ../../cpputil/RAII.cpp \
    ../../cpputil/ThreadLocalStorage.cpp \
    ../../cpputil/hasync.cpp \
    ../../cpputil/hdir.cpp \
    ../../cpputil/hpath.cpp \
    ../../cpputil/hstring.cpp \
    ../../cpputil/hurl.cpp \
    ../../cpputil/ifconfig.cpp \
    ../../cpputil/iniparser.cpp \
    ../../event/epoll.c \
    ../../event/evport.c \
    ../../event/hevent.c \
    ../../event/hloop.c \
    ../../event/iocp.c \
    ../../event/kcp/hkcp.c \
    ../../event/kcp/ikcp.c \
    ../../event/kqueue.c \
    ../../event/nio.c \
    ../../event/nlog.c \
    ../../event/noevent.c \
    ../../event/overlapio.c \
    ../../event/poll.c \
    ../../event/rudp.c \
    ../../event/select.c \
    ../../event/unpack.c \
#    ../../event/wepoll/wepoll.c \
#    ../../evpp/EventLoopThreadPool_test.cpp \
#    ../../evpp/EventLoopThread_test.cpp \
#    ../../evpp/EventLoop_test.cpp \
#    ../../evpp/TcpClientEventLoop_test.cpp \
#    ../../evpp/TcpClient_test.cpp \
#    ../../evpp/TcpServer_test.cpp \
#    ../../evpp/TimerThread_test.cpp \
#    ../../evpp/UdpClient_test.cpp \
#    ../../evpp/UdpServer_test.cpp \
    ../../http/Http1Parser.cpp \
    ../../http/Http2Parser.cpp \
    ../../http/HttpMessage.cpp \
    ../../http/HttpParser.cpp \
    ../../http/WebSocketChannel.cpp \
    ../../http/WebSocketParser.cpp \
    ../../http/client/AsyncHttpClient.cpp \
    ../../http/client/HttpClient.cpp \
    ../../http/client/WebSocketClient.cpp \
    ../../http/http_content.cpp \
    ../../http/http_parser.c \
    ../../http/httpdef.c \
    ../../http/multipart_parser.c \
    ../../http/server/FileCache.cpp \
    ../../http/server/HttpHandler.cpp \
    ../../http/server/HttpMiddleware.cpp \
    ../../http/server/HttpResponseWriter.cpp \
    ../../http/server/HttpServer.cpp \
    ../../http/server/HttpService.cpp \
    ../../http/server/http_page.cpp \
    ../../http/websocket_parser.c \
    ../../http/wsdef.c \
    ../../mqtt/mqtt_client.c \
    ../../mqtt/mqtt_protocol.c \
    ../../protocol/dns.c \
    ../../protocol/ftp.c \
    ../../protocol/icmp.c \
    ../../protocol/smtp.c \
    ../../ssl/appletls.c \
    ../../ssl/gnutls.c \
    ../../ssl/hssl.c \
    ../../ssl/mbedtls.c \
    ../../ssl/nossl.c \
    ../../ssl/openssl.c \
    ../../ssl/wintls.c \
    ../../util/base64.c \
    ../../util/md5.c \
    ../../util/sha1.c \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../../base/array.h \
    ../../base/hatomic.h \
    ../../base/hbase.h \
    ../../base/hbuf.h \
    ../../base/hdef.h \
    ../../base/heap.h \
    ../../base/hendian.h \
    ../../base/herr.h \
    ../../base/hlog.h \
    ../../base/hmain.h \
    ../../base/hmath.h \
    ../../base/hmutex.h \
    ../../base/hplatform.h \
    ../../base/hproc.h \
    ../../base/hsocket.h \
    ../../base/hsysinfo.h \
    ../../base/hthread.h \
    ../../base/htime.h \
    ../../base/hversion.h \
    ../../base/list.h \
    ../../base/netinet.h \
    ../../base/queue.h \
    ../../base/rbtree.h \
    ../../cpputil/ThreadLocalStorage.h \
    ../../cpputil/hasync.h \
    ../../cpputil/hdir.h \
    ../../cpputil/hfile.h \
    ../../cpputil/hmap.h \
    ../../cpputil/hobjectpool.h \
    ../../cpputil/hpath.h \
    ../../cpputil/hscope.h \
    ../../cpputil/hstring.h \
    ../../cpputil/hthreadpool.h \
    ../../cpputil/hurl.h \
    ../../cpputil/ifconfig.h \
    ../../cpputil/iniparser.h \
    ../../cpputil/json.hpp \
    ../../cpputil/singleton.h \
    ../../event/hevent.h \
    ../../event/hloop.h \
    ../../event/iowatcher.h \
    ../../event/kcp/hkcp.h \
    ../../event/kcp/ikcp.h \
    ../../event/nlog.h \
    ../../event/overlapio.h \
    ../../event/rudp.h \
    ../../event/unpack.h \
#    ../../event/wepoll/wepoll.h \
    ../../evpp/Buffer.h \
    ../../evpp/Channel.h \
    ../../evpp/Event.h \
    ../../evpp/EventLoop.h \
    ../../evpp/EventLoopThread.h \
    ../../evpp/EventLoopThreadPool.h \
    ../../evpp/Status.h \
    ../../evpp/TcpClient.h \
    ../../evpp/TcpServer.h \
    ../../evpp/TimerThread.h \
    ../../evpp/UdpClient.h \
    ../../evpp/UdpServer.h \
    ../../hconfig.h \
    ../../hexport.h \
    ../../http/Http1Parser.h \
    ../../http/Http2Parser.h \
    ../../http/HttpMessage.h \
    ../../http/HttpParser.h \
    ../../http/WebSocketChannel.h \
    ../../http/WebSocketParser.h \
    ../../http/client/AsyncHttpClient.h \
    ../../http/client/HttpClient.h \
    ../../http/client/WebSocketClient.h \
    ../../http/client/axios.h \
    ../../http/client/requests.h \
    ../../http/grpcdef.h \
    ../../http/http2def.h \
    ../../http/http_content.h \
    ../../http/http_parser.h \
    ../../http/httpdef.h \
    ../../http/multipart_parser.h \
    ../../http/server/FileCache.h \
    ../../http/server/HttpContext.h \
    ../../http/server/HttpHandler.h \
    ../../http/server/HttpMiddleware.h \
    ../../http/server/HttpResponseWriter.h \
    ../../http/server/HttpServer.h \
    ../../http/server/HttpService.h \
    ../../http/server/WebSocketServer.h \
    ../../http/server/http_page.h \
    ../../http/websocket_parser.h \
    ../../http/wsdef.h \
    ../../misc/grpc_server.h \
    ../../misc/win32_getopt.h \
    ../../mqtt/mqtt_client.h \
    ../../mqtt/mqtt_protocol.h \
    ../../protocol/dns.h \
    ../../protocol/ftp.h \
    ../../protocol/icmp.h \
    ../../protocol/smtp.h \
    ../../ssl/hssl.h \
    ../../util/base64.h \
    ../../util/md5.h \
    ../../util/sha1.h \
    mainwindow.h \
    QtTest/EventLoopThread_Test.h \
    QtTest/TcpClient_Test.h \
    QtTest/TcpServer_Test.h \
    QtTest/UdpClient_Test.h \
    QtTest/UdpServer_Test.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../../base/README.md \
    ../../cpputil/README.md \
    ../../event/README.md \
    ../../event/kcp/LICENSE \
    ../../event/wepoll/LICENSE \
    ../../event/wepoll/README.md \
    ../../evpp/README.md \
    ../../evpp/build_test.sh \
    ../../http/README.md \
    ../../protocol/README.md \
    ../../util/README.md
