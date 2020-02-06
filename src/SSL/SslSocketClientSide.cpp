#include "SSL/SslSocketClientSide.hpp"
#include "SSL/SslCtxResource.hpp"
#include "SSL/SslResource.hpp"
#include "CommonSocket.hpp"

SslSocketClientSide::SslSocketClientSide()
    : tcpSocket(std::make_unique<CommonSocket>())
    , sslCtx(std::make_unique<SslCtxResource>(SSLv23_client_method()))
    , ssl(nullptr)
{

}

SslSocketClientSide& SslSocketClientSide::operator=(SslSocketClientSide&& rhv)
{
    ssl = std::move(rhv.ssl);
    sslCtx = std::move(rhv.sslCtx);
    tcpSocket = std::move(rhv.tcpSocket);

    rhv.tcpSocket = std::make_unique<CommonSocket>();
    rhv.sslCtx = std::make_unique<SslCtxResource>(SSLv23_client_method());

    return *this;
}


SslSocketClientSide::~SslSocketClientSide()
{
    close();
}

SslSocketClientSide::SslSocketClientSide(SslSocketClientSide&& rhv)
{
    (*this) = std::move(rhv);
}

bool SslSocketClientSide::connectToHost(const std::string& address, const std::string& port)
{
    close();

    std::unique_ptr<CommonSocket> commonSocket = std::make_unique<CommonSocket>();
    if (!commonSocket->connectToHost(address, port)) return false;
    tcpSocket = std::move(commonSocket);
    int socketId = tcpSocket->getSocketId();

    ssl = sslCtx->getNewSsl();
    if (ssl.get() == nullptr) return false;
    if (!ssl->connectSocket(socketId)) return false;
    return true;
}

void SslSocketClientSide::closeImplement()
{
    tcpSocket->close();
    if(ssl.get() != nullptr) ssl->close();
    return;
}

size_t SslSocketClientSide::receiveImplement(char* const direct, const size_t maxSize)
{
    if (ssl == nullptr) return 0;
    return ssl->receive(direct, maxSize);
}

size_t SslSocketClientSide::sendImplement(const char* const source, const size_t dataSize)
{
    if (ssl == nullptr) return 0;
    return ssl->send(source, dataSize);
}

bool SslSocketClientSide::isValidImplement() const
{
    return tcpSocket->isValid();
}

std::string SslSocketClientSide::getConnectedIpImplement() const
{
    return tcpSocket->getConnectedIP();
}

int SslSocketClientSide::getSocketIdImplement() const
{
    return tcpSocket->getSocketId();
}
