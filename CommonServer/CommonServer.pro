TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../include \
                ../include/Interface


SOURCES += \
        main.cpp \
    ../src/Interface/ServerInterface.cpp \
    ../src/Interface/SocketInterface.cpp \
    ../src/Interface/TcpServer.cpp \
    ../src/Interface/TcpSocket.cpp \
    ../src/CommonServer.cpp \
    ../src/CommonSocket.cpp \
    ../src/LinuxServer.cpp \
    ../src/LinuxSocket.cpp

HEADERS += \
    ../include/Interface/ServerInterface.hpp \
    ../include/Interface/SocketInterface.hpp \
    ../include/Interface/TcpServer.hpp \
    ../include/Interface/TcpSocket.hpp \
    ../include/CommonServer.hpp \
    ../include/CommonSocket.hpp \
    ../include/LinuxServer.hpp \
    ../include/LinuxSocket.hpp
