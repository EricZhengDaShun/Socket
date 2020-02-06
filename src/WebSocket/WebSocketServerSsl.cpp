#include "WebSocket/WebSocketServerSsl.hpp"
#include "SSL/SslServer.hpp"
#include "Interface/SocketInterface.hpp"
#include "Interface/WebSocketServerSideProcess.hpp"
#include "WebSocket/WebSocketServerSide.hpp"

WebSocketServerSsl::WebSocketServerSsl()
    : serverInterface(std::make_unique<SslServer>())
{

}

WebSocketServerSsl::~WebSocketServerSsl()
{

}

void WebSocketServerSsl::setCertificateFileName(const std::string& fileName)
{
    certFileName = fileName;
    return;
}

void WebSocketServerSsl::setKeyFileName(const std::string& fileName)
{
    keyFileName = fileName;
    return;
}

bool WebSocketServerSsl::startListen(const std::string& port)
{
    std::unique_ptr<SslServer> server = std::make_unique<SslServer>();
    server->setCertificateFileName(certFileName);
    server->setKeyFileName(keyFileName);
    serverInterface = std::move(server);
    return serverInterface->startListen(port);
}

void WebSocketServerSsl::close()
{
    serverInterface->close();
    return;
}

std::unique_ptr<WebSocketServerSideProcess> WebSocketServerSsl::waitNewConnection()
{
    std::unique_ptr<SocketInterface> socketInterface = serverInterface->waitNewConnection();
    if(socketInterface.get() == nullptr) return nullptr;
    std::unique_ptr<WebSocketServerSideProcess> webSocketServerSide = std::make_unique<WebSocketServerSide>();
    if (webSocketServerSide->acceptSocket(std::move(socketInterface))) {
        return webSocketServerSide;
    }
    return nullptr;
}
