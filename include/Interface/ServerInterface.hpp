#pragma once

#include <string>
#include <memory>

class SocketInterface;

class ServerInterface
{
public:
    virtual ~ServerInterface();
    ServerInterface(const ServerInterface&) = default;
    ServerInterface& operator=(const ServerInterface&) = default;

    bool startListen(const std::string& port);
    void close();
    std::unique_ptr<SocketInterface> waitNewConnection();

protected:
    ServerInterface();

private:
    virtual bool startListenImplement(const std::string& port) = 0;
    virtual void closeImplement() = 0;
    virtual std::unique_ptr<SocketInterface> waitNewConnectionImplement() = 0;
};
