#pragma once
#include <string>
#include <memory>
#include "Interface/TcpServer.hpp"

class TcpSocket;

class LinuxServer : public TcpServer
{
public:
    LinuxServer();
    ~LinuxServer() override;
    LinuxServer(const LinuxServer&) = delete;
    LinuxServer& operator=(const LinuxServer& rhv) = delete;

private:
    bool startListenImplement(const std::string& port) override;
    void closeImplement() override;
    std::unique_ptr<TcpSocket> waitNewConnectionImplement() override;

private:
    int listenSocket;
};


