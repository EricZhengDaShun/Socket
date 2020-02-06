#include "SSL/SslSocketServerSide.hpp"
#include "SSL/SslCtxResource.hpp"
#include "SSL/SslResource.hpp"
#include "CommonSocket.hpp"

SslSocketServerSide::SslSocketServerSide()
    : tcpSocket(std::make_unique<CommonSocket>())
    , sslCtx(std::make_unique<SslCtxResource>(SSLv23_server_method()))
    , ssl(nullptr)
{

}

SslSocketServerSide& SslSocketServerSide::operator=(SslSocketServerSide&& rhv)
{
    ssl = std::move(rhv.ssl);
    sslCtx = std::move(rhv.sslCtx);
    tcpSocket = std::move(rhv.tcpSocket);

    rhv.tcpSocket = std::make_unique<CommonSocket>();
    rhv.sslCtx = std::make_unique<SslCtxResource>(SSLv23_server_method());
    return *this;
}

SslSocketServerSide::~SslSocketServerSide()
{
    close();
}

SslSocketServerSide::SslSocketServerSide(SslSocketServerSide&& rhv)
{
    (*this) = std::move(rhv);
}

bool SslSocketServerSide::acceptSocket(const std::string& certFileName,
    const std::string& keyFileName,
    std::unique_ptr<SocketInterface> socket)
{
    if (!sslCtx->setCertificateFile(certFileName)) return false;
    if (!sslCtx->setKeyFile(keyFileName)) return false;
    ssl = sslCtx->getNewSsl();
    if (ssl.get() == nullptr) return false;

    tcpSocket = std::move(socket);
    return ssl->acceptSocket(tcpSocket->getSocketId());
}

void SslSocketServerSide::closeImplement()
{
    tcpSocket->close();
    if (ssl.get() != nullptr) ssl->close();
    return;
}

size_t SslSocketServerSide::receiveImplement(char* const direct, const size_t maxSize)
{
    if (ssl == nullptr) return 0;
    return ssl->receive(direct, maxSize);
}

size_t SslSocketServerSide::sendImplement(const char* const source, const size_t dataSize)
{
    if (ssl == nullptr) return 0;
    return ssl->send(source, dataSize);
}

bool SslSocketServerSide::isValidImplement() const
{
    return tcpSocket->isValid();
}

std::string SslSocketServerSide::getConnectedIpImplement() const
{
    return tcpSocket->getConnectedIP();
}

int SslSocketServerSide::getSocketIdImplement() const
{
    return tcpSocket->getSocketId();
}