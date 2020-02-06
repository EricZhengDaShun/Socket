#pragma once
#include <cstddef>
#include <string>

class TcpSocket
{
public:
    virtual ~TcpSocket() = default;
    TcpSocket(const TcpSocket&) = default;
    TcpSocket& operator=(const TcpSocket&) = default;

    bool connectToHost(const std::string& address, const std::string& port);
    void close();

    size_t receive(char* const direct, const size_t maxSize);
    size_t send(const char* const source, const size_t dataSize);

    bool isValid() const;
    std::string getConnectedIP() const;

    int getSocketId() const;

protected:
    TcpSocket() = default;

private:
    virtual bool connectToHostImplement(const std::string& address, const std::string& port) = 0;
    virtual void closeImplement() = 0;

    virtual size_t receiveImplement(char* const direct, const size_t maxSize) = 0;
    virtual size_t sendImplement(const char* const source, const size_t dataSize) = 0;

    virtual bool isValidImplement() const = 0;
    virtual std::string getConnectedIpImplement() const = 0;

    virtual int getSocketIdImplement() const = 0;
};

