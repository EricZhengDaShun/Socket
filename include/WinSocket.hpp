#pragma once

#ifdef _MSC_VER

#include <string>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "Interface/TcpSocket.hpp"

class WinSocket : public TcpSocket
{
public:
    explicit WinSocket();
    explicit WinSocket(const SOCKET socketId, const std::string ip);
    virtual ~WinSocket();
    WinSocket(const WinSocket&) = delete;
    WinSocket& operator=(const WinSocket& rhv) = delete;

private:
    bool connectToHostImplement(const std::string& address, const std::string& port) override;
    void closeImplement() override;

    size_t receiveImplement(char* const direct, const size_t maxSize) override;
    size_t sendImplement(const char* const source, const size_t dataSize) override;

    bool isValidImplement() const override;
    std::string getConnectedIpImplement() const override;

    int getSocketIdImplement() const override;

private:
    bool isDllSetupSuccess;
    SOCKET connectSocket;
    std::string connectIP;
};

#endif