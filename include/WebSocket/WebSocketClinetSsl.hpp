#pragma once
#include <string>
#include <memory>
#include <atomic>
#include <mutex>
#include "Interface/WebSocketClientProcess.hpp"
#include "WebSocket/Opcode.hpp"

class SocketInterface;
class FrameData;
class FrameParser;

class WebSocketClinetSsl : public WebSocketClientProcess
{
public:
    explicit WebSocketClinetSsl();
    ~WebSocketClinetSsl() override;
    WebSocketClinetSsl(const WebSocketClinetSsl&) = delete;
    WebSocketClinetSsl& operator=(const WebSocketClinetSsl&) = delete;

private:
    bool connectToHostImplement(const std::string& address, const std::string& port) override;
    void closeImplement() override;

    bool sendContinuationImplement(const char* const source, const size_t size) override;
    bool sendTextImplement(const std::string& text) override;
    bool sendBinaryImplement(const char* const source, const size_t size) override;
    bool sendCloseImplement() override;
    bool sendPingImplement() override;
    bool sendPongImplement() override;

    bool recvDataImplement(Opcode::Type& opcodeType, std::vector<char>& direct) override;

private:
    bool handshakeProcess(const std::string& address);
    std::string getHandshakeData(const std::string& address);
    std::string getSecWebSocketKey();
    bool checkSecWebSocketAccept(const std::string& recvHandshakeData);
    std::string getCheckString();
    std::string getRecvCheckString(const std::string& recvHandshakeData);

private:
    std::unique_ptr<SocketInterface> sslSocket;
    std::string secWebSocketKeyBuf;
    std::unique_ptr<FrameData> frameData;
    
    std::atomic<bool> sendAvailable;
    std::atomic<bool> recvAvailable;

    std::mutex sendMutex;
    std::mutex recvMutex;
    std::vector<char> recvBuf;

    std::unique_ptr<FrameParser> frameParser;
};