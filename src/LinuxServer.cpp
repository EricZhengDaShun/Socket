 #include "LinuxServer.hpp"
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Interface/TcpSocket.hpp"
#include "LinuxSocket.hpp"

namespace {
    static const int noError = 0;
    static const int error = -1;

    void initialAddrinfo(addrinfo& info)
    {
        std::memset(&info, 0, sizeof(info));
        info.ai_family = AF_INET;
        info.ai_socktype = SOCK_STREAM;
        info.ai_protocol = IPPROTO_TCP;
        return;
    }

    std::string getAddressIP(const sockaddr* const source)
    {
        std::string result = "";
        const int family = source->sa_family;
        if (family == AF_INET) {
            char ipAddress[INET_ADDRSTRLEN];
            std::memset(ipAddress, 0, INET_ADDRSTRLEN);
            const sockaddr_in* const ipv4 = reinterpret_cast<const sockaddr_in* const>(source);
            ::inet_ntop(AF_INET, &(ipv4->sin_addr), ipAddress, INET_ADDRSTRLEN);
            result.assign(ipAddress);

        } else if (family == AF_INET6) {
            char ipAddress[INET6_ADDRSTRLEN];
            std::memset(ipAddress, 0, INET6_ADDRSTRLEN);
            const sockaddr_in6* const ipv6 = reinterpret_cast<const sockaddr_in6* const>(source->sa_data);
            ::inet_ntop(AF_INET6, &(ipv6->sin6_addr), ipAddress, INET6_ADDRSTRLEN);
            result.assign(ipAddress);
        }

        return result;
    }

}

LinuxServer::LinuxServer()
    : listenSocket(0)
{

}

LinuxServer::~LinuxServer()
{

}

bool LinuxServer::startListenImplement(const std::string& port)
{
    addrinfo hints;
    ::initialAddrinfo(hints);
    addrinfo* resolve = nullptr;
    int result = noError;
    result = ::getaddrinfo(nullptr, port.c_str(), &hints, &resolve);
    if (result != noError) return false;

    listenSocket = ::socket(resolve->ai_family, resolve->ai_socktype, resolve->ai_protocol);
    if (listenSocket == 0) {
        ::freeaddrinfo(resolve);
        return false;
    }

    result = ::bind(listenSocket, resolve->ai_addr, static_cast<socklen_t>(resolve->ai_addrlen));
    if (result != noError) {
        ::freeaddrinfo(resolve);
        ::close(listenSocket);
        return false;
    }
    ::freeaddrinfo(resolve);

    result = ::listen(listenSocket, SOMAXCONN);
    if (result != noError) {
        ::close(listenSocket);
        return false;
    }

    return true;
}

void LinuxServer::closeImplement()
{
    ::shutdown(listenSocket, SHUT_RDWR);
    ::close(listenSocket);
    listenSocket = 0;
    return;
}

std::unique_ptr<TcpSocket> LinuxServer::waitNewConnectionImplement()
{
    std::unique_ptr<TcpSocket> tcpSocket;
    if (listenSocket == 0) return tcpSocket;

    sockaddr addrInfo;
    socklen_t addrInfoLen = sizeof(addrInfo);
    int clientSocket = ::accept(listenSocket, &addrInfo, &addrInfoLen);
    if (clientSocket == error) return tcpSocket;
    const std::string ip = ::getAddressIP(&addrInfo);
    tcpSocket = std::make_unique<LinuxSocket>(clientSocket, ip);
    return tcpSocket;
}

