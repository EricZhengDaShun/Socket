#pragma once
#include <string>
#include <memory>
#include "Interface/ServerInterface.hpp"

class CommonServer;
class WebSocketServerSideProcess;

class WebSocketServer final
{
public:
    explicit WebSocketServer();
    ~WebSocketServer();
    WebSocketServer(const WebSocketServer&) = delete;
    WebSocketServer& operator=(const WebSocketServer&) = delete;

    bool startListen(const std::string& port);
    void close();
    std::unique_ptr<WebSocketServerSideProcess> waitNewConnection();

private:
    std::unique_ptr<ServerInterface> serverInterface;
};