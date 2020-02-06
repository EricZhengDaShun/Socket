#pragma once
#include <string>
#include <memory>
#include "Interface/ServerInterface.hpp"

class SocketInterface;
class TcpServer;
class CommonSocket;

class CommonServer final : public ServerInterface
{
public:
    explicit CommonServer();
    ~CommonServer() override;
    CommonServer(const CommonServer&) = delete;
    CommonServer& operator=(const CommonServer&) = delete;

private:
    bool startListenImplement(const std::string& port) override;
    void closeImplement() override;

    std::unique_ptr<SocketInterface> waitNewConnectionImplement() override;

private:
    std::unique_ptr<TcpServer> tcpServer;
};

