#include "Interface/TcpServer.hpp"
#include "Interface/TcpSocket.hpp"

bool TcpServer::startListen(const std::string& port)
{
    return startListenImplement(port);
}

void TcpServer::close()
{
    closeImplement();
    return;
}

std::unique_ptr<TcpSocket> TcpServer::waitNewConnection()
{
    return waitNewConnectionImplement();
}