#pragma once
#include <string>

class SocketInterface
{
public:
    virtual ~SocketInterface() = default;
    SocketInterface(const SocketInterface&) = default;
    SocketInterface& operator=(const SocketInterface&) = default;

    void close();
    size_t receive(char* const direct, const size_t maxSize);
    size_t send(const char* const source, const size_t dataSize);

    bool isValid() const;
    std::string getConnectedIP() const;

    int getSocketId() const;

protected:
    SocketInterface() = default;

private:
    virtual void closeImplement() = 0;
    virtual size_t receiveImplement(char* const direct, const size_t maxSize) = 0;
    virtual size_t sendImplement(const char* const source, const size_t dataSize) = 0;

    virtual bool isValidImplement() const = 0;
    virtual std::string getConnectedIpImplement() const = 0;

    virtual int getSocketIdImplement() const = 0;
};