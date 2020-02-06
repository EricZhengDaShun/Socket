#include "CommonServer.hpp"
#include "Interface/SocketInterface.hpp"
#include "Interface/TcpServer.hpp"
#include "Interface/TcpSocket.hpp"
#include "CommonSocket.hpp"

#ifdef _MSC_VER
#include "WinServer.hpp"

CommonServer::CommonServer()
    : tcpServer(std::make_unique<WinServer>())
{

}

#elif __GNUC__
#include "LinuxServer.hpp"

CommonServer::CommonServer()
    : tcpServer(std::make_unique<LinuxServer>())
{

}

#endif 

CommonServer::~CommonServer()
{

}

bool CommonServer::startListenImplement(const std::string& port)
{
    return tcpServer->startListen(port);
}

void CommonServer::closeImplement()
{
    tcpServer->close();
    return;
}

std::unique_ptr<SocketInterface> CommonServer::waitNewConnectionImplement()
{
    std::unique_ptr<TcpSocket> tcpSocket = tcpServer->waitNewConnection();
    if (tcpSocket.get() == nullptr) return nullptr;
    std::unique_ptr<SocketInterface> socketInterface = std::make_unique<CommonSocket>(tcpSocket);
    return socketInterface;
}
