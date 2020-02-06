#include <iostream>
#include <memory>
#include <string>
#include "Interface/SocketTask.hpp"
#include "Interface/SocketInterface.hpp"
#include "EchoTask.hpp"
#include "SocketTaskManager.hpp"
#include "CommonServer.hpp"
#include "ServerTaskLinker.hpp"
#include "SSL/SslServer.hpp"
#include "SSL/SslSocketServerSide.hpp"

int main()
{
    /*
    std::shared_ptr<SocketTaskManager> socketTaskManager = std::make_shared<SocketTaskManager>();
    std::cout << "Establish socketTaskManager done" << std::endl;

    const std::string port = "7788";
    std::unique_ptr<CommonServer> server = std::make_unique<CommonServer>();
    if (server->startListen(port)) {
        std::cout << "Server start listen port " << port << " !" << std::endl;
    } else {
        std::cout << "Server listen port " << port << " fail !" << std::endl;
        std::cin.get();
        return 0;
    }

    ServerTaskLinker serverTaskLinker;
    serverTaskLinker.setServer(std::move(server));
    serverTaskLinker.setTaskManager(socketTaskManager);
    serverTaskLinker.link<EchoTask>();
    std::cout << "Link !" << std::endl;
    std::cin.get();
    std::cout << "Stop !" << std::endl;
    serverTaskLinker.closeServerAndUnlink();
    std::cout << "closed !" << std::endl;
    std::cin.get();
    socketTaskManager.reset();
    */

    std::shared_ptr<SocketTaskManager> socketTaskManager = std::make_shared<SocketTaskManager>();
    std::cout << "Establish socketTaskManager done" << std::endl;

    const std::string port = "7788";
    std::unique_ptr<SslServer> sslServer = std::make_unique<SslServer>();
    sslServer->setCertificateFileName("certificate.pem");
    sslServer->setKeyFileName("key.pem");
    if (sslServer->startListen(port)) {
        std::cout << "Server start listen port " << port << " !" << std::endl;
    } else {
        std::cout << "Server listen port " << port << " fail !" << std::endl;
        std::cin.get();
        return 0;
    }

    ServerTaskLinker serverTaskLinker;
    serverTaskLinker.setServer(std::move(sslServer));
    serverTaskLinker.setTaskManager(socketTaskManager);
    serverTaskLinker.link<EchoTask>();
    std::cout << "Link !" << std::endl;
    std::cin.get();
    std::cout << "Stop !" << std::endl;
    serverTaskLinker.closeServerAndUnlink();
    std::cout << "closed !" << std::endl;
    std::cin.get();
    socketTaskManager.reset();
    return 0;
}