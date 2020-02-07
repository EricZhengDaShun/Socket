#include "CommonSocket.hpp"
#include "Interface/TcpSocket.hpp"

#ifdef _MSC_VER
#include "WinSocket.hpp"

CommonSocket::CommonSocket() noexcept
    : tcpSocket(std::make_unique<WinSocket>())
{

}

CommonSocket& CommonSocket::operator=(CommonSocket&& rhv) noexcept
{
    tcpSocket = std::move(rhv.tcpSocket);
    rhv.tcpSocket = std::make_unique<WinSocket>();
    return *this;
}

std::unique_ptr<TcpSocket> CommonSocket::getResource()
{
    std::unique_ptr<TcpSocket> tmp = std::move(tcpSocket);
    tcpSocket = std::make_unique<WinSocket>();
    return tmp;
}

#elif __GNUC__
#include "LinuxSocket.hpp"

CommonSocket::CommonSocket()
    : tcpSocket(std::make_unique<LinuxSocket>())
{

}

CommonSocket& CommonSocket::operator=(CommonSocket&& rhv)
{
    tcpSocket = std::move(rhv.tcpSocket);
    rhv.tcpSocket = std::make_unique<LinuxSocket>();
    return *this;
}

std::unique_ptr<TcpSocket> CommonSocket::getResource()
{
    std::unique_ptr<TcpSocket> tmp = std::move(tcpSocket);
    tcpSocket = std::make_unique<LinuxSocket>();
    return tmp;
}

#endif 

CommonSocket::~CommonSocket()
{

}

CommonSocket::CommonSocket(CommonSocket&& rhv) noexcept
{
    (*this) = std::move(rhv);
}

CommonSocket::CommonSocket(std::unique_ptr<TcpSocket>& rhv) noexcept
    : CommonSocket()
{
    if (rhv.get() == nullptr) return;
    tcpSocket = std::move(rhv);
}

bool CommonSocket::connectToHost(const std::string& address, const std::string& port)
{
    return tcpSocket->connectToHost(address, port);
}

void CommonSocket::closeImplement()
{
    tcpSocket->close();
    return;
}

size_t CommonSocket::receiveImplement(char* const direct, const size_t maxSize)
{
    return tcpSocket->receive(direct, maxSize);
}

size_t CommonSocket::sendImplement(const char* const source, const size_t dataSize)
{
    return tcpSocket->send(source, dataSize);
}

bool CommonSocket::isValidImplement() const
{
    return tcpSocket->isValid();
}

std::string CommonSocket::getConnectedIpImplement() const
{
    return tcpSocket->getConnectedIP();
}

int CommonSocket::getSocketIdImplement() const
{
    return tcpSocket->getSocketId();
}