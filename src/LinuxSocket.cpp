#include "LinuxSocket.hpp"

#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace {
    static const int noError = 0;
    static const int error = -1;

    void initialAddrinfo(addrinfo& info)
    {
        std::memset(&info, 0, sizeof(info));
        info.ai_family = AF_UNSPEC;
        info.ai_socktype = SOCK_STREAM;
        info.ai_protocol = IPPROTO_TCP;
        return;
    }

    std::string getAddressIP(const addrinfo* const source)
    {
        std::string result = "";
        const int family = source->ai_family;
        if (family == AF_INET) {
            char ipAddress[INET_ADDRSTRLEN];
            std::memset(ipAddress, 0, INET_ADDRSTRLEN);
            const sockaddr_in* const ipv4 = reinterpret_cast<const sockaddr_in* const>(source->ai_addr);
            ::inet_ntop(AF_INET, &(ipv4->sin_addr), ipAddress, INET_ADDRSTRLEN);
            result.assign(ipAddress);

        } else if (family == AF_INET6) {
            char ipAddress[INET6_ADDRSTRLEN];
            std::memset(ipAddress, 0, INET6_ADDRSTRLEN);
            const sockaddr_in6* const ipv6 = reinterpret_cast<const sockaddr_in6* const>(source->ai_addr);
            ::inet_ntop(AF_INET6, &(ipv6->sin6_addr), ipAddress, INET6_ADDRSTRLEN);
            result.assign(ipAddress);
        }

        return result;
    }

    int findEffectiveConnect(const addrinfo* const source, std::string& addressIP)
    {
        const addrinfo* list = source;
        int result = error;
        for (const addrinfo* address = list; address != nullptr; address = address->ai_next) {
            result = ::socket(address->ai_family, address->ai_socktype, address->ai_protocol);
            if (result == error) break;

            const int connectResult = ::connect(result, address->ai_addr, static_cast<socklen_t>(address->ai_addrlen));
            if (connectResult == error) {
                ::close(result);
                result = error;
                continue;
            } else {
                addressIP = ::getAddressIP(address);
                break;
            }
        }
        return result;
    }
}

LinuxSocket::LinuxSocket()
    : sockfd(0)
    , connectIP("")
{

}

LinuxSocket::LinuxSocket(const int id, const std::string ip)
    : LinuxSocket()
{
    sockfd = id;
    connectIP = ip;
}

LinuxSocket::~LinuxSocket()
{
    close();
}

bool LinuxSocket::connectToHostImplement(const std::string& address, const std::string& port)
{
    close();

    addrinfo hints;
    ::initialAddrinfo(hints);
    addrinfo* addrInfoList = nullptr;
    int result = noError;
    result = ::getaddrinfo(address.c_str(), port.c_str(), &hints, &addrInfoList);
    if (result != noError) return false;
    std::string addressIP = "";
    sockfd = ::findEffectiveConnect(addrInfoList, addressIP);
    ::freeaddrinfo(addrInfoList);
    if ((sockfd == 0) || (sockfd == error)) return false;

    connectIP = addressIP;
    return true;
}

void LinuxSocket::closeImplement()
{
    if(sockfd > 0) {
        ::shutdown(sockfd, SHUT_RDWR);
        ::close(sockfd);
    }
    sockfd = 0;
    connectIP.clear();
    return;
}

size_t LinuxSocket::receiveImplement(char* const direct, const size_t maxSize)
{
    if (sockfd == 0) return 0;

    static const int nullFlag = 0;
    ssize_t result = noError;
    result = ::recv(sockfd, direct, maxSize, nullFlag);
    if (result == error) {
        close();
        result = 0;
    }
    return static_cast<size_t>(result);
}

size_t LinuxSocket::sendImplement(const char* const source, const size_t dataSize)
{
    if (sockfd == 0) return 0;

    static const int nullFlag = 0;
    ssize_t result = noError;
    result = ::send(sockfd, source, dataSize, nullFlag);
    if (result == error) {
        close();
        result = 0;
    }
    return static_cast<size_t>(result);
}

bool LinuxSocket::isValidImplement() const
{
    return (sockfd != 0);
}

std::string LinuxSocket::getConnectedIpImplement() const
{
    return connectIP;
}

int LinuxSocket::getSocketIdImplement() const
{
    return sockfd;
}

