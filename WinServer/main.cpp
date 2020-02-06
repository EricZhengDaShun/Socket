#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "WinServer.hpp"
#include "Interface/TcpSocket.hpp"

int main()
{
    static const std::string port = "7788";

    WinServer server;
    if (!server.startListen(port)) {
        std::cout << "Listen port : " << port << " fail !" << std::endl;
        std::cin.get();
        return 0;
    }

    std::unique_ptr<TcpSocket> socket = server.waitNewConnection();
    if (socket.get() == nullptr) {
        server.close();
        std::cout << "Socket error ! " << std::endl;
        std::cin.get();
        return 0;
    }

    server.close();

    std::vector<char> buf(1024);
    const size_t recvSize = socket->receive(buf.data(), buf.size());
    std::cout << "Recive data : " << std::string(buf.data(), recvSize) << std::endl;
    std::cout << "Echo" << std::endl;
    socket->send(buf.data(), recvSize);
    socket->close();
    std::cin.get();
    return 0;
}