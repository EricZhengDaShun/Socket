#include "WinServer.hpp"
#include <ws2tcpip.h>
#include "Interface/TcpSocket.hpp"
#include "WinSocket.hpp"

namespace {
    void initialAddrinfo(addrinfo& info)
    {
        std::memset(&info, 0, sizeof(info));
        info.ai_family = AF_INET;
        info.ai_socktype = SOCK_STREAM;
        info.ai_protocol = IPPROTO_TCP;
        return;
    }

    std::string getAddressIP(const SOCKADDR* const source)
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
            const sockaddr_in6* const ipv6 = reinterpret_cast<const sockaddr_in6* const>(source);
            ::inet_ntop(AF_INET6, &(ipv6->sin6_addr), ipAddress, INET6_ADDRSTRLEN);
            result.assign(ipAddress);
        }

        return result;
    }

}


WinServer::WinServer()
    : isDllSetupSuccess(false)
    , listenSocket(INVALID_SOCKET)
{
    const WORD dllVersion(MAKEWORD(2, 2));
    WSADATA wsaData;
    if (::WSAStartup(dllVersion, &wsaData) == NO_ERROR) isDllSetupSuccess = true;
}

WinServer::~WinServer()
{
    WSACleanup();
}

bool WinServer::startListenImplement(const std::string& port)
{
    addrinfo hints;
    ::initialAddrinfo(hints);
    addrinfo* resolve = nullptr;
    int result = NO_ERROR;
    result = ::getaddrinfo(NULL, port.c_str(), &hints, &resolve);
    if (result != NO_ERROR) return false;

    listenSocket = ::socket(resolve->ai_family, resolve->ai_socktype, resolve->ai_protocol);
    if (listenSocket == INVALID_SOCKET) {
        ::freeaddrinfo(resolve);
        return false;
    }

    result = ::bind(listenSocket, resolve->ai_addr, static_cast<int>(resolve->ai_addrlen));
    if (result == SOCKET_ERROR) {
        ::freeaddrinfo(resolve);
        ::closesocket(listenSocket);
        return false;
    }
    ::freeaddrinfo(resolve);

    result = ::listen(listenSocket, SOMAXCONN);
    if (result == SOCKET_ERROR) {
        ::closesocket(listenSocket);
        return false;
    }

    return true;
}

void WinServer::closeImplement()
{
    ::shutdown(listenSocket, SD_BOTH);
    ::closesocket(listenSocket);
    listenSocket = INVALID_SOCKET;
    return;
}

std::unique_ptr<TcpSocket> WinServer::waitNewConnectionImplement()
{
    std::unique_ptr<TcpSocket> tcpSocket;
    if (listenSocket == INVALID_SOCKET) return tcpSocket;

    SOCKADDR addrInfo;
    int addrInfoLen = sizeof(addrInfo);
    SOCKET clientSocket = ::accept(listenSocket, &addrInfo, &addrInfoLen);
    if (clientSocket == INVALID_SOCKET) return tcpSocket;
    const std::string ip = ::getAddressIP(&addrInfo);
    tcpSocket = std::make_unique<WinSocket>(clientSocket, ip);
    return tcpSocket;
}
