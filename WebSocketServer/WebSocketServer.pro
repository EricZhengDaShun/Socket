TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpthread

INCLUDEPATH += /usr/local/ssl/include

LIBS += -L/usr/local/ssl/lib/ -lssl
LIBS += -L/usr/local/ssl/lib/ -lcrypto

INCLUDEPATH += ../include \
                ../include/Interface

SOURCES += \
        main.cpp \
    ../src/Base64/Base64Decoder.cpp \
    ../src/Base64/Base64Encoder.cpp \
    ../src/Base64/Base64Processor.cpp \
    ../src/Interface/ServerInterface.cpp \
    ../src/Interface/SocketInterface.cpp \
    ../src/Interface/SocketTask.cpp \
    ../src/Interface/TcpServer.cpp \
    ../src/Interface/TcpSocket.cpp \
    ../src/Interface/WebSocketClientProcess.cpp \
    ../src/Interface/WebSocketServerSideProcess.cpp \
    ../src/Sha1/DataShiftOperator.cpp \
    ../src/Sha1/DataStuffer.cpp \
    ../src/Sha1/LogicFunctionExecutor.cpp \
    ../src/Sha1/MethodOne.cpp \
    ../src/Sha1/MethodTwo.cpp \
    ../src/Sha1/Sha1.cpp \
    ../src/Sha1/ShaAlgorithm.cpp \
    ../src/Sha1/Utility.cpp \
    ../src/SSL/SslCtxResource.cpp \
    ../src/SSL/SslResource.cpp \
    ../src/SSL/SslServer.cpp \
    ../src/SSL/SslSocketClientSide.cpp \
    ../src/SSL/SslSocketServerSide.cpp \
    ../src/WebSocket/FrameData.cpp \
    ../src/WebSocket/FrameParser.cpp \
    ../src/WebSocket/Opcode.cpp \
    ../src/WebSocket/PayloadLength.cpp \
    ../src/WebSocket/WebSocketClient.cpp \
    ../src/WebSocket/WebSocketClientSsl.cpp \
    ../src/WebSocket/WebSocketMask.cpp \
    ../src/WebSocket/WebSocketServer.cpp \
    ../src/WebSocket/WebSocketServerSide.cpp \
    ../src/WebSocket/WebSocketServerSsl.cpp \
    ../src/CommonServer.cpp \
    ../src/CommonSocket.cpp \
    ../src/LinuxServer.cpp \
    ../src/LinuxSocket.cpp \
    ../src/ServerTaskLinker.cpp \
    ../src/SocketTaskManager.cpp

HEADERS += \
    ../include/Base64/Base64Decoder.hpp \
    ../include/Base64/Base64Define.hpp \
    ../include/Base64/Base64Encoder.hpp \
    ../include/Base64/Base64Processor.hpp \
    ../include/Interface/ServerInterface.hpp \
    ../include/Interface/SocketInterface.hpp \
    ../include/Interface/SocketTask.hpp \
    ../include/Interface/TcpServer.hpp \
    ../include/Interface/TcpSocket.hpp \
    ../include/Interface/WebSocketClientProcess.hpp \
    ../include/Interface/WebSocketServerSideProcess.hpp \
    ../include/Sha1/DataShiftOperator.hpp \
    ../include/Sha1/DataStuffer.hpp \
    ../include/Sha1/LogicFunctionExecutor.hpp \
    ../include/Sha1/MethodOne.hpp \
    ../include/Sha1/MethodTwo.hpp \
    ../include/Sha1/Sha1.hpp \
    ../include/Sha1/ShaAlgorithm.hpp \
    ../include/Sha1/TypeDefine.hpp \
    ../include/Sha1/Utility.hpp \
    ../include/SSL/SslCtxResource.hpp \
    ../include/SSL/SslResource.hpp \
    ../include/SSL/SslServer.hpp \
    ../include/SSL/SslSocketClientSide.hpp \
    ../include/SSL/SslSocketServerSide.hpp \
    ../include/WebSocket/FrameData.hpp \
    ../include/WebSocket/FrameParser.hpp \
    ../include/WebSocket/Opcode.hpp \
    ../include/WebSocket/PayloadLength.hpp \
    ../include/WebSocket/WebSocketClinet.hpp \
    ../include/WebSocket/WebSocketClinetSsl.hpp \
    ../include/WebSocket/WebSocketMask.hpp \
    ../include/WebSocket/WebSocketServer.hpp \
    ../include/WebSocket/WebSocketServerSide.hpp \
    ../include/WebSocket/WebSocketServerSsl.hpp \
    ../include/CommonServer.hpp \
    ../include/CommonSocket.hpp \
    ../include/LinuxServer.hpp \
    ../include/LinuxSocket.hpp \
    ../include/ServerTaskLinker.hpp \
    ../include/SocketTaskManager.hpp
