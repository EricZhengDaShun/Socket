#pragma once

#include <string>
#include <memory>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "Interface/TcpServer.hpp"

class TcpSocket;

class WinServer : public TcpServer
{
public:
    explicit WinServer();
    virtual ~WinServer();
    WinServer(const WinServer&) = delete;
    WinServer& operator=(const WinServer& rhv) = delete;

private:
    bool startListenImplement(const std::string& port) override;
    void closeImplement() override;
    std::unique_ptr<TcpSocket> waitNewConnectionImplement() override;

private:
    bool isDllSetupSuccess;
    SOCKET listenSocket;
};
