#include <iostream>
#include <string>
#include <memory>
#include "CommonServer.hpp"
#include "Interface/SocketInterface.hpp"
#include "SocketTaskManager.hpp"
#include "Interface/SocketTask.hpp"
#include "EchoTask.hpp"
#include "SSL/SslSocketServerSide.hpp"
#include "SSL/SslServer.hpp"

int main()
{
    /*
    static const std::string port = "7788";
    CommonServer server;
    if (!server.startListen(port)) {
        std::cout << "Listen port : " << port << " fail !" << std::endl;
        std::cin.get();
        return 0;
    }

    std::unique_ptr<SocketInterface> socket = server.waitNewConnection();
    SocketTaskManager socketTaskManager;
    while (socket.get() != nullptr) {
        std::unique_ptr<SocketTask> socketTask = std::make_unique<EchoTask>();
        socketTaskManager.addSocketAndTask(std::move(socket), std::move(socketTask));
        socket = server.waitNewConnection();
    }
    */

    static const std::string port = "7788";
    SslServer sslServer;
    sslServer.setCertificateFileName("certificate.pem");
    sslServer.setKeyFileName("key.pem");
    if (!sslServer.startListen(port)) {
        std::cout << "Listen oirt : " << port << " fail !" << std::endl;
        std::cin.get();
        return 0;
    }

    std::unique_ptr<SocketInterface> socket = sslServer.waitNewConnection();
    SocketTaskManager socketTaskManager;
    while (socket.get() != nullptr) {
        std::unique_ptr<SocketTask> socketTask = std::make_unique<EchoTask>();
        socketTaskManager.addSocketAndTask(std::move(socket), std::move(socketTask));
        socket = sslServer.waitNewConnection();
    }
    std::cin.get();

    return 0;
}