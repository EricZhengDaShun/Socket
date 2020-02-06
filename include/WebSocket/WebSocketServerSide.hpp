#pragma once
#include <string>
#include <memory>
#include <atomic>
#include <mutex>
#include "Interface/WebSocketServerSideProcess.hpp"
#include "WebSocket/Opcode.hpp"

class SocketInterface;
class FrameData;
class FrameParser;

class WebSocketServerSide : public WebSocketServerSideProcess
{
public:
    explicit WebSocketServerSide();
    ~WebSocketServerSide() override;
    WebSocketServerSide(const WebSocketServerSide&) = delete;
    WebSocketServerSide& operator=(const WebSocketServerSide&) = delete;

private:
    bool acceptSocketImplement(std::unique_ptr<SocketInterface> socket) override;
    void closeImplement() override;

    bool sendContinuationImplement(const char* const source, const size_t size) override;
    bool sendTextImplement(const std::string& text) override;
    bool sendBinaryImplement(const char* const source, const size_t size) override;
    bool sendCloseImplement() override;
    bool sendPingImplement() override;
    bool sendPongImplement() override;

    bool recvDataImplement(Opcode::Type& opcodeType, std::vector<char>& direct) override;

private:
    bool handshakeProcess(const std::string& header);
    std::string getSecWebSocketKey(const std::string& header);
    std::string getSecWebSocketAccept(const std::string& secWebSocketKey);

private:
    std::unique_ptr<SocketInterface> tcpSocket;
    std::string secWebSocketKeyBuf;
    std::unique_ptr<FrameData> frameData;

    std::atomic<bool> sendAvailable;
    std::atomic<bool> recvAvailable;

    std::mutex sendMutex;
    std::mutex recvMutex;
    std::vector<char> recvBuf;

    std::unique_ptr<FrameParser> frameParser;
};