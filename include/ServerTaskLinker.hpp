#pragma once
#include <memory>
#include <thread>
#include "Interface/ServerInterface.hpp"
#include "Interface/SocketInterface.hpp"
#include "SocketTaskManager.hpp"

template<typename T>
class PushSocketToManaager
{
public:
    void operator()(std::unique_ptr<ServerInterface>& server, std::shared_ptr<SocketTaskManager>& sockTaskManager)
    {
        std::unique_ptr<SocketInterface> socket = server->waitNewConnection();
        while (socket.get() != nullptr) {
            sockTaskManager->addSocketAndTask(std::move(socket), std::make_unique<T>());
            socket = server->waitNewConnection();
        }
        return;
    }
};

class ServerTaskLinker final
{
public:
    explicit ServerTaskLinker();
    ~ServerTaskLinker();
    ServerTaskLinker(const ServerTaskLinker&) = delete;
    ServerTaskLinker& operator=(const ServerTaskLinker&) = delete;

    bool setServer(std::unique_ptr<ServerInterface> taskServer);
    bool setTaskManager(std::shared_ptr<SocketTaskManager>& manager);

    template<typename T>
    bool link()
    {
        if (pushSocketThread.get() != nullptr) {
            if (pushSocketThread->joinable()) return false;
        }

        pushSocketThread = std::make_unique<std::thread>(PushSocketToManaager<T>(),
            std::ref(server),
            std::ref(socketTaskManager));
        return true;
    }

    void closeServerAndUnlink();

private:
    std::unique_ptr<ServerInterface> server;
    std::shared_ptr<SocketTaskManager> socketTaskManager;
    std::unique_ptr<std::thread> pushSocketThread;
};

