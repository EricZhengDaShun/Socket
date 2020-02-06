#include <iostream>
#include <vector>
#include "SSL/SslSocketClientSide.hpp"

int main()
{
    SslSocketClientSide sslSocketClient;
    bool result = sslSocketClient.connectToHost("127.0.0.1", "7788");
    if (!result) {
        std::cout << "connect fail !" << std::endl;
        std::cin.get();
        return 0;
    }

    std::string request = "GET / HTTP/1.1\r\nHost: www.google.com\r\n\r\n";
    sslSocketClient.send(request.data(), request.size());

    SslSocketClientSide s2 = std::move(sslSocketClient);

    std::vector<char> buf(4096);
    s2.receive(buf.data(), buf.size());
    std::string response(buf.data());
    std::cout << "response:" << response << std::endl;

    s2.close();
    std::cin.get();
    return 0;
}
