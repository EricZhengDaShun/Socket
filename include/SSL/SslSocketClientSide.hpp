#pragma once

#include <memory>
#include <string>
#include "openssl/ssl.h"
#include "openssl/err.h"
#ifdef _MSC_VER
#pragma comment (lib, "libssl.lib")
#pragma comment (lib, "libcrypto.lib")
#endif
#include "Interface/SocketInterface.hpp"

class SslCtxResource;
class SslResource;

class SslSocketClientSide final : public SocketInterface
{
public:
    explicit SslSocketClientSide();
    ~SslSocketClientSide() override;
    SslSocketClientSide(const SslSocketClientSide&) = delete;
    SslSocketClientSide& operator=(const SslSocketClientSide&) = delete;
    SslSocketClientSide(SslSocketClientSide&& rhv);
    SslSocketClientSide& operator=(SslSocketClientSide&& rhv);

    bool connectToHost(const std::string& address, const std::string& port);

private:
    void closeImplement() override;

    size_t receiveImplement(char* const direct, const size_t maxSize) override;
    size_t sendImplement(const char* const source, const size_t dataSize) override;

    bool isValidImplement() const override;
    std::string getConnectedIpImplement() const override;

    int getSocketIdImplement() const override;

private:
    std::unique_ptr<SocketInterface> tcpSocket;
    std::unique_ptr<SslCtxResource> sslCtx;
    std::unique_ptr<SslResource> ssl;
};

