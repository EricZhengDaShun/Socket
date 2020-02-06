#pragma once
#include <memory>
#include <string>
#include "Interface/ServerInterface.hpp"

class CommonServer;
class SslSocketServerSide;

class SslServer : public ServerInterface
{
public:
    explicit SslServer();
    ~SslServer() override;
    SslServer(const SslServer&) = delete;
    SslServer& operator=(const SslServer&) = delete;

    void setCertificateFileName(const std::string& fileName);
    void setKeyFileName(const std::string& fileName);

    std::unique_ptr<SslSocketServerSide> waitNewConnection();

private:
    bool startListenImplement(const std::string& port);
    void closeImplement();

    std::unique_ptr<SocketInterface> waitNewConnectionImplement();

private:
    std::string certFileName;
    std::string keyFileName;
    std::unique_ptr<CommonServer> commonServer;
};

