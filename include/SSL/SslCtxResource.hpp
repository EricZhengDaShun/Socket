#pragma once
#include <memory>
#include <string>
#include "openssl/ssl.h"
#include "openssl/err.h"
#ifdef _MSC_VER
#pragma comment (lib, "libssl.lib")
#pragma comment (lib, "libcrypto.lib")
#endif

class SslResource;

class SslCtxResource final
{
public:
    explicit SslCtxResource(const SSL_METHOD* method);
    ~SslCtxResource();
    SslCtxResource(const SslCtxResource&) = delete;
    SslCtxResource& operator=(const SslCtxResource&) = delete;

    std::unique_ptr<SslResource> getNewSsl();

    bool setCertificateFile(const std::string& fileName);
    bool setKeyFile(const std::string& fileName);

private:
    SSL_CTX* sslCtx;
};

