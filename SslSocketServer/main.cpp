#include <iostream>
#include <memory>
#include <vector>
#include "SSL/SslServer.hpp"
#include "Interface/SocketInterface.hpp"
#include "SSL/SslSocketServerSide.hpp"

int main()
{
    SslServer sslServer;
    sslServer.setCertificateFileName("certificate.pem");
    sslServer.setKeyFileName("key.pem");
    sslServer.startListen("7788");
    std::unique_ptr<SocketInterface> sslSocket = sslServer.waitNewConnection();
    if (sslSocket.get() == nullptr) {
        return 0;
    }
    std::vector<char> buf(4096);
    size_t len = sslSocket->receive(buf.data(), buf.size());
    sslSocket->send(buf.data(), len);
    sslSocket->close();
    sslServer.close();
    std::cin.get();
    return 0;
}