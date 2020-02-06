#pragma once
#include "openssl/ssl.h"
#include "openssl/err.h"
#ifdef _MSC_VER
#pragma comment (lib, "libssl.lib")
#pragma comment (lib, "libcrypto.lib")
#endif

class SslResource
{
public:
    explicit SslResource(SSL* source);
    ~SslResource();
    SslResource(const SslResource&) = delete;
    SslResource& operator=(const SslResource&) = delete;

    bool connectSocket(const int socketId);
    bool acceptSocket(const int socketId);

    size_t receive(char* const direct, const size_t maxSize);
    size_t send(const char* const source, const size_t dataSize);

    void close();

private:
    SSL* ssl;
};

