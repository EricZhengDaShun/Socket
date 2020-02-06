#include "WebSocket/WebSocketServer.hpp"
#include "CommonServer.hpp"
#include "Interface/SocketInterface.hpp"
#include "WebSocket/WebSocketServerSide.hpp"
#include "Interface/WebSocketServerSideProcess.hpp"

WebSocketServer::WebSocketServer()
    : serverInterface(std::make_unique<CommonServer>())
{

}

WebSocketServer::~WebSocketServer()
{

}

bool WebSocketServer::startListen(const std::string& port)
{
    return serverInterface->startListen(port);
}

void WebSocketServer::close()
{
    serverInterface->close();
    return;
}

std::unique_ptr<WebSocketServerSideProcess> WebSocketServer::waitNewConnection()
{
    std::unique_ptr<SocketInterface> socketInterface = serverInterface->waitNewConnection();
    std::unique_ptr<WebSocketServerSide> webSocketServerSide = std::make_unique<WebSocketServerSide>();
    if (webSocketServerSide->acceptSocket(std::move(socketInterface))) {
        return webSocketServerSide;
    }
    return nullptr;
}
