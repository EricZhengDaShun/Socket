#pragma once
#include <string>
#include <memory>

class TcpSocket;

class TcpServer
{
public:
    virtual ~TcpServer() = default;

    bool startListen(const std::string& port);
    void close();
    std::unique_ptr<TcpSocket> waitNewConnection();

protected:
    TcpServer() = default;

private:
    virtual bool startListenImplement(const std::string& port) = 0;
    virtual void closeImplement() = 0;
    virtual std::unique_ptr<TcpSocket> waitNewConnectionImplement() = 0;
};