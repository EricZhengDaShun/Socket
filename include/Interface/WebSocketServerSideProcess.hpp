#pragma once
#include <string>
#include <memory>
#include <vector>
#include "WebSocket/Opcode.hpp"

class SocketInterface;

class WebSocketServerSideProcess
{
public:
    virtual ~WebSocketServerSideProcess() = default;
    WebSocketServerSideProcess(const WebSocketServerSideProcess&) = default;
    WebSocketServerSideProcess& operator=(const WebSocketServerSideProcess&) = default;

    bool acceptSocket(std::unique_ptr<SocketInterface> socket);
    void close();

    bool sendContinuation(const char* const source, const size_t size);
    bool sendContinuation(const std::vector<char>& data);

    bool sendText(const std::string& text);
    bool sendText(const std::vector<char>& data);

    bool sendBinary(const char* const source, const size_t size);
    bool sendBinary(const std::vector<char>& data);

    bool sendClose();

    bool sendPing();

    bool sendPong();

    bool recvData(Opcode::Type& opcodeType, std::vector<char>& direct);

private:

    virtual bool acceptSocketImplement(std::unique_ptr<SocketInterface> socket) = 0;
    virtual void closeImplement() = 0;

    virtual bool sendContinuationImplement(const char* const source, const size_t size) = 0;
    virtual bool sendTextImplement(const std::string& text) = 0;
    virtual bool sendBinaryImplement(const char* const source, const size_t size) = 0;
    virtual bool sendCloseImplement() = 0;
    virtual bool sendPingImplement() = 0;
    virtual bool sendPongImplement() = 0;

    virtual bool recvDataImplement(Opcode::Type& opcodeType, std::vector<char>& direct) = 0;

protected:
    WebSocketServerSideProcess() = default;
};