#pragma once

#include <memory>
#include <string>
#include "Interface/SocketInterface.hpp"

class TcpSocket;

class CommonSocket final : public SocketInterface
{
public:
    explicit CommonSocket() noexcept;
    ~CommonSocket() override;
    CommonSocket(const CommonSocket&) = delete;
    CommonSocket& operator=(const CommonSocket&) = delete;
    CommonSocket(CommonSocket&& rhv) noexcept;
    CommonSocket& operator=(CommonSocket&& rhv) noexcept;
    CommonSocket(std::unique_ptr<TcpSocket>& rhv) noexcept;

    bool connectToHost(const std::string& address, const std::string& port);
    std::unique_ptr<TcpSocket> getResource();

private:
    void closeImplement() override;

    size_t receiveImplement(char* const direct, const size_t maxSize) override;
    size_t sendImplement(const char* const source, const size_t dataSize) override;

    bool isValidImplement() const override;
    std::string getConnectedIpImplement() const override;

    int getSocketIdImplement() const override;

private:
    std::unique_ptr<TcpSocket> tcpSocket;
};

