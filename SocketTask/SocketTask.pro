TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpthread
INCLUDEPATH += /usr/local/ssl/include

LIBS += -L/usr/local/ssl/lib/ -lssl
LIBS += -L/usr/local/ssl/lib/ -lcrypto

INCLUDEPATH += ../include
                ../src

SOURCES += \
        main.cpp \
    EchoTask.cpp \
    ../src/Interface/ServerInterface.cpp \
    ../src/Interface/SocketInterface.cpp \
    ../src/Interface/SocketTask.cpp \
    ../src/Interface/TcpServer.cpp \
    ../src/Interface/TcpSocket.cpp \
    ../src/CommonServer.cpp \
    ../src/CommonSocket.cpp \
    ../src/LinuxServer.cpp \
    ../src/LinuxSocket.cpp \
    ../src/SocketTaskManager.cpp \
    ../src/SSL/SslServer.cpp \
    ../src/SSL/SslSocketServerSide.cpp \
    ../src/SSL/SslCtxResource.cpp \
    ../src/SSL/SslResource.cpp

HEADERS += \
    EchoTask.hpp \
    ../include/Interface/ServerInterface.hpp \
    ../include/Interface/SocketInterface.hpp \
    ../include/Interface/SocketTask.hpp \
    ../include/Interface/TcpServer.hpp \
    ../include/Interface/TcpSocket.hpp \
    ../include/CommonServer.hpp \
    ../include/CommonSocket.hpp \
    ../include/LinuxServer.hpp \
    ../include/LinuxSocket.hpp \
    ../include/SocketTaskManager.hpp \
    ../include/SSL/SslServer.hpp \
    ../include/SSL/SslSocketServerSide.hpp \
    ../include/SSL/SslCtxResource.hpp \
    ../include/SSL/SslResource.hpp
