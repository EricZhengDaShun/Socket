#include "SSL/SslServer.hpp"
#include "CommonServer.hpp"
#include "Interface/SocketInterface.hpp"
#include "SSL/SslSocketServerSide.hpp"

SslServer::SslServer()
    : commonServer(std::make_unique<CommonServer>())
{

}

SslServer::~SslServer()
{

}

void SslServer::setCertificateFileName(const std::string& fileName)
{
    certFileName = fileName;
    return;
}

void SslServer::setKeyFileName(const std::string& fileName)
{
    keyFileName = fileName;
    return;
}

std::unique_ptr<SslSocketServerSide> SslServer::waitNewConnection()
{
    std::unique_ptr<SocketInterface> tcpSocket = commonServer->waitNewConnection();
    if (tcpSocket.get() == nullptr) return nullptr;

    std::unique_ptr<SslSocketServerSide> sslSocketServerSide = std::make_unique<SslSocketServerSide>();
    if (!sslSocketServerSide->acceptSocket(certFileName, keyFileName, std::move(tcpSocket))) {
        sslSocketServerSide.reset();
    }
    return sslSocketServerSide;
}

bool SslServer::startListenImplement(const std::string& port)
{
    return commonServer->startListen(port);
}

void SslServer::closeImplement()
{
    commonServer->close();
    return;
}

std::unique_ptr<SocketInterface> SslServer::waitNewConnectionImplement()
{
    std::unique_ptr<SslSocketServerSide> sslSocketServerSide = waitNewConnection();
    return sslSocketServerSide;
}