#pragma once

#include <cstddef>
#include <string>
#include "Interface/TcpSocket.hpp"

class LinuxSocket : public TcpSocket
{
public:
    explicit LinuxSocket();
    LinuxSocket(const int id, const std::string ip);
    ~LinuxSocket() override;
    LinuxSocket(const LinuxSocket&) = delete;
    LinuxSocket& operator=(const LinuxSocket&) = delete;

private:
    bool connectToHostImplement(const std::string& address, const std::string& port) override;
    void closeImplement() override;

    size_t receiveImplement(char* const direct, const size_t maxSize) override;
    size_t sendImplement(const char* const source, const size_t dataSize) override;

    bool isValidImplement() const override;
    std::string getConnectedIpImplement() const override;

    int getSocketIdImplement() const override;

private:
    int sockfd;
    std::string connectIP;
};
