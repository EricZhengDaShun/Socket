#include "Interface/ServerInterface.hpp"
#include "Interface/SocketInterface.hpp"

ServerInterface::ServerInterface()
{

}

ServerInterface::~ServerInterface()
{

}

bool ServerInterface::startListen(const std::string& port)
{
    return startListenImplement(port);
}

void ServerInterface::close()
{
    closeImplement();
    return;
}

std::unique_ptr<SocketInterface> ServerInterface::waitNewConnection()
{
    return waitNewConnectionImplement();
}