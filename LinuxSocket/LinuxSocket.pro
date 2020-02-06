TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../include
                ../src

SOURCES += \
        main.cpp \
    ../src/Interface/TcpSocket.cpp \
    ../src/LinuxSocket.cpp

HEADERS += \
    ../include/Interface/TcpSocket.hpp \
    ../include/LinuxSocket.hpp
