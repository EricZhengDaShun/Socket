#include "Interface/SocketInterface.hpp"

void SocketInterface::close()
{
    closeImplement();
    return;
}

size_t SocketInterface::receive(char* const direct, const size_t maxSize)
{
    return receiveImplement(direct, maxSize);
}

size_t SocketInterface::send(const char* const source, const size_t dataSize)
{
    return sendImplement(source, dataSize);
}

bool SocketInterface::isValid() const
{
    return isValidImplement();
}

std::string SocketInterface::getConnectedIP() const
{
    return getConnectedIpImplement();
}

int SocketInterface::getSocketId() const
{
    return getSocketIdImplement();
}