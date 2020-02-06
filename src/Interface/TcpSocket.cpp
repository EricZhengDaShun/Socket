#include "Interface/TcpSocket.hpp"

bool TcpSocket::connectToHost(const std::string& address, const std::string& port)
{
    return connectToHostImplement(address, port);
}

void TcpSocket::close()
{
    closeImplement();
    return;
}

size_t TcpSocket::receive(char* const direct, const size_t maxSize)
{
    return receiveImplement(direct, maxSize);
}

size_t TcpSocket::send(const char* const source, const size_t dataSize)
{
    return sendImplement(source, dataSize);
}

bool TcpSocket::isValid() const
{
    return isValidImplement();
}

std::string TcpSocket::getConnectedIP() const
{
    return getConnectedIpImplement();
}

int TcpSocket::getSocketId() const
{
    return getSocketIdImplement();
}