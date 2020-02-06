#include "WinSocket.hpp"

#ifdef _MSC_VER
#include <cstring>
#include <ws2tcpip.h>

namespace {
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

    SOCKET findEffectiveConnect(const addrinfo* const source, std::string& addressIP)
    {
        const addrinfo* list = source;
        SOCKET result = INVALID_SOCKET;
        for (const addrinfo* address = list; address != nullptr; address = address->ai_next) {
            result = ::socket(address->ai_family, address->ai_socktype, address->ai_protocol);
            if (result == INVALID_SOCKET) break;

            const int connectResult = ::connect(result, address->ai_addr, static_cast<int>(address->ai_addrlen));
            if (connectResult == SOCKET_ERROR) {
                ::closesocket(result);
                result = INVALID_SOCKET;
                continue;
            } else {
                addressIP = ::getAddressIP(address);
                break;
            }
        }
        return result;
    }
}

WinSocket::WinSocket()
    : isDllSetupSuccess(false)
    , connectSocket(INVALID_SOCKET)
    , connectIP("")
{
    const WORD dllVersion(MAKEWORD(2, 2));
    WSADATA wsaData;
    if (::WSAStartup(dllVersion, &wsaData) == NO_ERROR) isDllSetupSuccess = true;
}

WinSocket::WinSocket(const SOCKET socketId, const std::string ip)
    : WinSocket()
{
    connectSocket = socketId;
    connectIP = ip;
}

WinSocket::~WinSocket()
{
    close();
    ::WSACleanup();
}

bool WinSocket::connectToHostImplement(const std::string& address, const std::string& port)
{
    if (connectSocket != INVALID_SOCKET) close();

    addrinfo hints;
    ::initialAddrinfo(hints);
    addrinfo* addrInfoList = nullptr;
    int result = NO_ERROR;
    result = ::getaddrinfo(address.c_str(), port.c_str(), &hints, &addrInfoList);
    if (result != NO_ERROR) return false;
    std::string addressIP = "";
    connectSocket = ::findEffectiveConnect(addrInfoList, addressIP);
    ::freeaddrinfo(addrInfoList);
    if (connectSocket == INVALID_SOCKET) return false;

    connectIP = addressIP;
    return true;
}

void WinSocket::closeImplement() 
{
    connectIP.clear();
    ::shutdown(connectSocket, SD_BOTH);
    ::closesocket(connectSocket);
    connectSocket = INVALID_SOCKET;
    return;
}

size_t WinSocket::receiveImplement(char* const direct, const size_t maxSize)
{
    if (connectSocket == INVALID_SOCKET) return 0;

    static const int nullFlag = 0;
    int result = NO_ERROR;
    result = ::recv(connectSocket, direct, static_cast<int>(maxSize), nullFlag);
    if (result == SOCKET_ERROR) {
        close();
        result = 0;
    }
    return result;
}

size_t WinSocket::sendImplement(const char* const source, const size_t dataSize)
{
    if (connectSocket == INVALID_SOCKET) return 0;

    static const int nullFlag = 0;
    int result = NO_ERROR;
    result = ::send(connectSocket, source, static_cast<int>(dataSize), nullFlag);
    if (result == SOCKET_ERROR) {
        close();
        result = 0;
    }
    return result;
}

bool WinSocket::isValidImplement() const
{
    if (!isDllSetupSuccess) return false;
    return (connectSocket != INVALID_SOCKET);
}

std::string WinSocket::getConnectedIpImplement() const
{
    return connectIP;
}

int WinSocket::getSocketIdImplement() const
{
    return static_cast<int>(connectSocket);
}

#endif
