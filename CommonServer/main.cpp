#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "CommonServer.hpp"
#include "Interface/SocketInterface.hpp"

int main()
{
    static const std::string port = "7788";
    CommonServer server;
    if (!server.startListen(port)) {
        std::cout << "Start listen port : " << port << " fail !" << std::endl;
        std::cin.get();
        return 0;
    }

    std::unique_ptr<SocketInterface> socket = server.waitNewConnection();
    
    if (socket.get() == nullptr) {
        server.close();
        std::cout << "Socket error ! " << std::endl;
        std::cin.get();
        return 0;
    }

    std::vector<char> buf(1024);
    const size_t recvSize = socket->receive(buf.data(), buf.size());
    std::cout << "Recive data : " << std::string(buf.data(), recvSize) << std::endl;
    std::cout << "Echo" << std::endl;
    socket->send(buf.data(), recvSize);
    server.close();

    std::cin.get();
    return 0;
}
