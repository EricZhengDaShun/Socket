#pragma once
#include <string>
#include <memory>
#include "Interface/ServerInterface.hpp"

class CommonServer;
class WebSocketServerSideProcess;

class WebSocketServerSsl final
{
public:
    explicit WebSocketServerSsl();
    ~WebSocketServerSsl();
    WebSocketServerSsl(const WebSocketServerSsl&) = delete;
    WebSocketServerSsl& operator=(const WebSocketServerSsl&) = delete;

    void setCertificateFileName(const std::string& fileName);
    void setKeyFileName(const std::string& fileName);

    bool startListen(const std::string& port);
    void close();
    std::unique_ptr<WebSocketServerSideProcess> waitNewConnection();

private:
    std::unique_ptr<ServerInterface> serverInterface;
    std::string certFileName;
    std::string keyFileName;
};