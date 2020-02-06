#include <iostream>
#include <string>
#include <vector>
#include "CommonSocket.hpp"

int main()
{
    static const std::string address = "127.0.0.1";
    static const std::string port = "7788";

    CommonSocket socket;
    if (!socket.connectToHost(address, port)) {
        std::cout << "Connect : " << address << " port: " << port << " fail !" << std::endl;
        std::cin.get();
        return 0;
    }

    static const std::string sendData = "GET  /  HTTP/1.1\r\n\
Host: www.google.com\r\n\r\n";
    std::cout << "Send : \r\n" << sendData << std::endl;
    const size_t sendSize = socket.send(sendData.c_str(), sendData.size());
    if (sendSize != sendData.size()) {
        socket.close();
        std::cout << "Send fial !" << std::endl;
        std::cin.get();
        return 0;
    }

    std::vector<char> recvBuf(512);
    const size_t recvSize = socket.receive(recvBuf.data(), recvBuf.size());
    const std::string recvData(recvBuf.data(), recvSize);
    std::cout << "Recv : \r\n" << recvData << std::endl;
    socket.close();
    std::cin.get();
    return 0;
}