#include "SSL/SslCtxResource.hpp"
#include "SSL/SslResource.hpp"

SslCtxResource::SslCtxResource(const SSL_METHOD* method)
    : sslCtx(nullptr)
{
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    sslCtx = ::SSL_CTX_new(method);
}

SslCtxResource::~SslCtxResource()
{
    if (sslCtx != nullptr) {
        ::SSL_CTX_free(sslCtx);
        sslCtx = nullptr;
    }
}

std::unique_ptr<SslResource> SslCtxResource::getNewSsl()
{
    SSL* ssl = ::SSL_new(sslCtx);
    return std::make_unique<SslResource>(ssl);
}

bool SslCtxResource::setCertificateFile(const std::string& fileName)
{
    if (::SSL_CTX_use_certificate_file(sslCtx, fileName.data(), SSL_FILETYPE_PEM) <= 0) return false;
    return true;
}

bool SslCtxResource::setKeyFile(const std::string& fileName)
{
    if (::SSL_CTX_use_PrivateKey_file(sslCtx, fileName.data(), SSL_FILETYPE_PEM) <= 0) return false;
    return true;
}