#include "SSL/SslResource.hpp"

SslResource::SslResource(SSL* source)
    : ssl(source)
{

}

SslResource::~SslResource()
{
    close();
    if (ssl != nullptr) {
        ::SSL_free(ssl);
        ssl = nullptr;
    }
}

bool SslResource::connectSocket(const int socketId)
{
    if (ssl == nullptr) return false;

    static const int connectFail = -1;
    ::SSL_set_fd(ssl, socketId);
    if (::SSL_connect(ssl) == connectFail) return false;
    return true;
}

bool SslResource::acceptSocket(const int socketId)
{
    if (ssl == nullptr) return false;

    ::SSL_set_fd(ssl, socketId);
    if (::SSL_accept(ssl) < 0) return false;
    return true;
}

size_t SslResource::receive(char* const direct, const size_t maxSize)
{
    if (ssl == nullptr) return 0;
    const int recvNum = ::SSL_read(ssl, direct, static_cast<int>(maxSize));
    return static_cast<size_t>(recvNum);
}

size_t SslResource::send(const char* const source, const size_t dataSize)
{
    if (ssl == nullptr) return 0;
    const int sendLen = ::SSL_write(ssl, static_cast<const void*>(source), static_cast<int>(dataSize));
    return static_cast<size_t>(sendLen);
}

void SslResource::close()
{
    if (ssl != nullptr) ::SSL_shutdown(ssl);
    return;
}