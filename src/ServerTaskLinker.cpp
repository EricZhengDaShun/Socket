#include "ServerTaskLinker.hpp"

ServerTaskLinker::ServerTaskLinker()
    : server(nullptr)
    , socketTaskManager(nullptr)
    , pushSocketThread(nullptr)
{

}

ServerTaskLinker::~ServerTaskLinker()
{

}

bool ServerTaskLinker::setServer(std::unique_ptr<ServerInterface> taskServer)
{
    if (pushSocketThread.get() != nullptr) {
        if (pushSocketThread->joinable()) return false;
    }
    
    server = std::move(taskServer);
    if (server.get() == nullptr) return false;
    return true;
}

bool ServerTaskLinker::setTaskManager(std::shared_ptr<SocketTaskManager>& manager)
{
    if (pushSocketThread.get() != nullptr) {
        if (pushSocketThread->joinable()) return false;
    }

    socketTaskManager = manager;
    if (socketTaskManager.get() == nullptr) return false;
    return true;
}

void ServerTaskLinker::closeServerAndUnlink()
{
    if (server.get() == nullptr) return;
    server->close();
    if (pushSocketThread.get() == nullptr) return;
    if (pushSocketThread->joinable()) pushSocketThread->join();
    return;
}