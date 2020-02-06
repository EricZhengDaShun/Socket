#include <iostream>
#include <vector>
#include <string>
#include "WebSocket/WebSocketClinet.hpp"
#include "WebSocket/Opcode.hpp"
#include "WebSocket/WebSocketClinetSsl.hpp"

int main()
{
    static const std::string address = "127.0.0.1";
    static const std::string port = "8080";
    Opcode::Type type;
    std::vector<char> v;

    WebSocketClinetSsl webSocketClinet;
    if (!webSocketClinet.connectToHost(address, port)) {
        std::cout << "Connect to " << address << " port : " << port << " fail !" << std::endl;
        std::cin.get();
        return 0;
    }

    const std::string data = "1122";
    bool sendResult = webSocketClinet.sendText(data);
    if (sendResult) {
        std::cout << "Send data : " << data << std::endl;

    } else {
        std::cout << "Send data fail! " << std::endl;
        std::cin.get();
        return 0;
    }

    if (webSocketClinet.recvData(type, v)) {
        std::cout << "Recv data : " << std::string(v.data(), v.size()) << std::endl;

    } else {
        std::cout << "Recv data fail! " << std::endl;
        std::cin.get();
        return 0;
    }


    if (webSocketClinet.sendPing()) {
        std::cout << "Send ping  " << std::endl;

    } else {
        std::cout << "Send ping " << std::endl;
        std::cin.get();
        return 0;
    }

    if (webSocketClinet.recvData(type, v)) {
        if (type == Opcode::Type::Pong) {
            std::cout << "Recv pong" << std::endl;
        } else {
            std::cout << "Recv other" << std::endl;
        }

    } else {
        std::cout << "Recv data fail! " << std::endl;
        std::cin.get();
        return 0;
    }



    if (webSocketClinet.sendClose()) {
        std::cout << "Send close done !" << std::endl;
    } else {
        std::cout << "Send close fail! " << std::endl;
        std::cin.get();
        return 0;
    }

    if (webSocketClinet.recvData(type, v)) {
        if (type == Opcode::Type::ConnectionClose) {
            std::cout << "Recv ConnectionClose" << std::endl;
            webSocketClinet.close();
        } else {
            std::cout << "Recv other" << std::endl;
        }
        
    } else {
        std::cout << "Recv data fail! " << std::endl;
        std::cin.get();
        return 0;
    }

    std::cin.get();
    return 0;
}
