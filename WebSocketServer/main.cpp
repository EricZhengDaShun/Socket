#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "WebSocket/WebSocketServerSide.hpp"
#include "CommonServer.hpp"
#include "CommonSocket.hpp"
#include "Interface/SocketInterface.hpp"
#include "WebSocket/Opcode.hpp"
#include "SSL/SslServer.hpp"
#include "SSL/SslSocketServerSide.hpp"
#include "WebSocket/WebSocketServer.hpp"
#include "Interface/WebSocketServerSideProcess.hpp"
#include "WebSocket/WebSocketServerSsl.hpp"

int main()
{
    WebSocketServerSsl server;
    server.setCertificateFileName("certificate.pem");
    server.setKeyFileName("key.pem");
    server.startListen("8080");
    std::unique_ptr<WebSocketServerSideProcess> socket = server.waitNewConnection();
    if (socket.get() == nullptr) {
        std::cout << "Accept socket fail !" << std::endl;
        std::cin.get();
        return 0;
    }

    Opcode::Type type;
    std::vector<char> buf;
    if (!socket->recvData(type, buf)) {
        std::cout << "Recv data fail !" << std::endl;
        std::cin.get();
        return 0;
    } else {
        std::cout << "Recv data successful" << std::endl;
    }

    if (!socket->sendText(buf)) {
        std::cout << "Echo data fail !" << std::endl;
        std::cin.get();
        return 0;
    } else {
        std::cout << "Echo data successful" << std::endl;
    }

    if (!socket->recvData(type, buf)) {
        std::cout << "Recv data fail !" << std::endl;
        std::cin.get();
        return 0;
    } else {
        std::cout << "Recv data successful" << std::endl;
    }

    if (type == Opcode::Type::Ping) {
        std::cout << "Recv Ping !" << std::endl;
        socket->sendPong();
    }

    if (!socket->recvData(type, buf)) {
        std::cout << "Recv data fail !" << std::endl;
        std::cin.get();
        return 0;
    } else {
        std::cout << "Recv data successful" << std::endl;
    }

    if (type == Opcode::Type::ConnectionClose) {
        std::cout << "Recv close !" << std::endl;
        if (socket->sendClose()) {
            socket->close();
        }
    }

    return 0;
}