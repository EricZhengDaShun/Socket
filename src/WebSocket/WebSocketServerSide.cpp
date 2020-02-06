#include "WebSocket/WebSocketServerSide.hpp"
#include <cctype>
#include <vector>
#include <algorithm>
#include <random>
#include <thread>
#include <chrono>
#include "CommonSocket.hpp"
#include "Base64/Base64Processor.hpp"
#include "Sha1/Sha1.hpp"
#include "WebSocket/FrameData.hpp"
#include "WebSocket/FrameParser.hpp"

namespace {
    static const size_t recvBufSize = 2048;
}

WebSocketServerSide::WebSocketServerSide()
    : tcpSocket(std::make_unique<CommonSocket>())
    , secWebSocketKeyBuf("")
    , frameData(std::make_unique<FrameData>())
    , sendAvailable(false)
    , recvAvailable(false)
    , recvBuf(recvBufSize)
    , frameParser(std::make_unique<FrameParser>())
{

}

WebSocketServerSide::~WebSocketServerSide()
{

}

bool WebSocketServerSide::acceptSocketImplement(std::unique_ptr<SocketInterface> socket)
{
    close();

    std::lock(sendMutex, recvMutex);
    std::lock_guard<std::mutex> lockSend(sendMutex, std::adopt_lock);
    std::lock_guard<std::mutex> lockRecv(recvMutex, std::adopt_lock);

    if (tcpSocket.get() == nullptr) return false;
    tcpSocket = std::move(socket);
    const size_t recvSize = tcpSocket->receive(recvBuf.data(), recvBuf.size());
    const std::string handshakeHader(recvBuf.data(), recvSize);
    if (!handshakeProcess(handshakeHader)) {
        tcpSocket->close();
        return false;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    if (!tcpSocket->isValid()) return false;

    sendAvailable.store(true);
    recvAvailable.store(true);
    return true;
}

void WebSocketServerSide::closeImplement()
{
    if (sendAvailable.load()) {
        if (!sendClose()) {
            sendAvailable.store(false);
            recvAvailable.store(false);
            tcpSocket->close();
            return;
        }

        sendAvailable.store(false);
    }

    while (recvAvailable.load()) {
        if (!tcpSocket->isValid()) {
            recvAvailable.store(false);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    tcpSocket->close();
    return;
}

bool WebSocketServerSide::sendContinuationImplement(const char* const source, const size_t size)
{
    if (!sendAvailable.load()) return false;
    std::lock_guard<std::mutex> lockGuard(sendMutex);

    std::vector<char> tmp(source, source + size);
    FramTool::server::setContinuation(frameData, tmp);
    const std::vector<char> buf = frameData->getData();
    const size_t sendSize = tcpSocket->send(buf.data(), buf.size());
    return (buf.size() == sendSize);
}

bool WebSocketServerSide::sendTextImplement(const std::string& text)
{
    if (!sendAvailable.load()) return false;
    std::lock_guard<std::mutex> lockGuard(sendMutex);

    FramTool::server::setText(frameData, text);
    const std::vector<char> buf = frameData->getData();
    const size_t sendSize = tcpSocket->send(buf.data(), buf.size());
    return (buf.size() == sendSize);
}

bool WebSocketServerSide::sendBinaryImplement(const char* const source, const size_t size)
{
    if (!sendAvailable.load()) return false;
    std::lock_guard<std::mutex> lockGuard(sendMutex);

    std::vector<char> tmp(source, source + size);
    FramTool::server::setBinary(frameData, tmp);
    const std::vector<char> buf = frameData->getData();
    const size_t sendSize = tcpSocket->send(buf.data(), buf.size());
    return (buf.size() == sendSize);
}

bool WebSocketServerSide::sendCloseImplement()
{
    if (!sendAvailable.load()) return false;
    std::lock_guard<std::mutex> lockGuard(sendMutex);
    FramTool::server::setClose(frameData);
    const std::vector<char> buf = frameData->getData();
    const size_t sendSize = tcpSocket->send(buf.data(), buf.size());
    const bool result = (buf.size() == sendSize);
    if (result) sendAvailable.store(false);
    return result;
}

bool WebSocketServerSide::sendPingImplement()
{
    if (!sendAvailable.load()) return false;
    std::lock_guard<std::mutex> lockGuard(sendMutex);

    FramTool::server::setPing(frameData);
    const std::vector<char> buf = frameData->getData();
    const size_t sendSize = tcpSocket->send(buf.data(), buf.size());
    return (buf.size() == sendSize);
}

bool WebSocketServerSide::sendPongImplement()
{
    if (!sendAvailable.load()) return false;
    std::lock_guard<std::mutex> lockGuard(sendMutex);

    FramTool::server::setPong(frameData);
    const std::vector<char> buf = frameData->getData();
    const size_t sendSize = tcpSocket->send(buf.data(), buf.size());
    return (buf.size() == sendSize);
}

bool WebSocketServerSide::recvDataImplement(Opcode::Type& opcodeType, std::vector<char>& direct)
{
    if (!recvAvailable.load()) return false;

    std::lock_guard<std::mutex> lockGuard(recvMutex);
    const size_t recvSize = tcpSocket->receive(recvBuf.data(), recvBuf.size());
    if (recvSize == 0) return false;

    frameParser->reset();
    frameParser->setData(recvBuf);
    if (!frameParser->parse()) return false;
    opcodeType = frameParser->getOpcode();
    const bool hasMask = frameParser->getMaskState();
    const std::vector<char> mask = frameParser->getMask();
    const std::vector<char> payload = frameParser->getPayload();
    direct.clear();
    for (size_t count = 0; count < payload.size(); ++count) {
        if (hasMask) {
            const char originData = payload[count] ^ mask[count % mask.size()];
            direct.push_back(originData);
        } else {
            direct.push_back(payload[count]);
        }
    }

    if (opcodeType == Opcode::Type::ConnectionClose) recvAvailable.store(false);
    return true;
}

bool WebSocketServerSide::handshakeProcess(const std::string& header)
{
    const std::string secWebSocketKey = getSecWebSocketKey(header);
    if (secWebSocketKey.empty()) return false;
    const std::string secWebSocketAccept = getSecWebSocketAccept(secWebSocketKey);
    std::string responese = "HTTP/1.1 101 Switching Protocols\r\n\
Upgrade: websocket\r\n\
Connection: Upgrade\r\n\
Sec-WebSocket-Accept: ";
    responese += secWebSocketAccept;
    responese += "\r\n\r\n";

    const size_t sendSize = tcpSocket->send(responese.data(), responese.size());
    if (sendSize != responese.size()) return false;
    return true;
}

std::string WebSocketServerSide::getSecWebSocketKey(const std::string& header)
{
    std::string headerTmp{ header };
    std::transform(headerTmp.begin(), 
        headerTmp.end(), 
        headerTmp.begin(), 
        [](unsigned char c) { return static_cast<char>(std::toupper(c)); });

    static const std::string keyWord = "SEC-WEBSOCKET-KEY:";
    const std::string::size_type beginPos = headerTmp.find(keyWord);
    if (beginPos == std::string::npos) return "";
    const std::string::size_type endPos = headerTmp.find("\r\n", beginPos);
    if (endPos == std::string::npos) return "";
    const std::string::size_type length = endPos - (beginPos + keyWord.size());
    std::string secWebSocketKey = header.substr(beginPos + keyWord.size(), length);
    secWebSocketKey.erase(std::remove(secWebSocketKey.begin(), secWebSocketKey.end(), ' '), 
        secWebSocketKey.end());

    static const size_t secWebSocketKeyBase64DataSize = 24;
    if (secWebSocketKey.size() != secWebSocketKeyBase64DataSize) return "";
    return secWebSocketKey;
}

std::string WebSocketServerSide::getSecWebSocketAccept(const std::string& secWebSocketKey)
{
    static const std::string keyWord = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    const std::string accpetOriginData = secWebSocketKey + keyWord;
    std::vector<unsigned char> sha1Buf(accpetOriginData.cbegin(), accpetOriginData.cend());
    Sha1 sha1;
    sha1.setData(sha1Buf);
    std::vector<unsigned char> checkWordSource = sha1.getResultWithMethodOne();
    std::string result;
    Base64Processor::Base64Encoder::encode(reinterpret_cast<const char*>(checkWordSource.data()),
        checkWordSource.size(),
        result);
    return result;
}
