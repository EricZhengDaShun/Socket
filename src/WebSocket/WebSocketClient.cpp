#include "WebSocket/WebSocketClinet.hpp"
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

WebSocketClinet::WebSocketClinet()
    : tcpSocket(std::make_unique<CommonSocket>())
    , secWebSocketKeyBuf("")
    , frameData(std::make_unique<FrameData>())
    , sendAvailable(false)
    , recvAvailable(false)
    , recvBuf(recvBufSize)
    , frameParser(std::make_unique<FrameParser>())
{

}

WebSocketClinet::~WebSocketClinet()
{

}

bool WebSocketClinet::connectToHostImplement(const std::string& address, const std::string& port)
{
    close();

    std::lock(sendMutex, recvMutex);
    std::lock_guard<std::mutex> lockSend(sendMutex, std::adopt_lock);
    std::lock_guard<std::mutex> lockRecv(recvMutex, std::adopt_lock);

    std::unique_ptr<CommonSocket> socket = std::make_unique<CommonSocket>();
    if (!socket->connectToHost(address, port)) return false;
    tcpSocket = std::move(socket);

    if (!handshakeProcess(address)) {
        tcpSocket->close();
        return false;
    }

    sendAvailable.store(true);
    recvAvailable.store(true);
    return true;
}

void WebSocketClinet::closeImplement()
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

bool WebSocketClinet::sendContinuationImplement(const char* const source, const size_t size)
{
    if (!sendAvailable.load()) return false;
    std::lock_guard<std::mutex> lockGuard(sendMutex);

    std::vector<char> tmp(source, source + size);
    FramTool::client::setContinuation(frameData, tmp);
    const std::vector<char> buf = frameData->getData();
    const size_t sendSize = tcpSocket->send(buf.data(), buf.size());
    return (buf.size() == sendSize);
}

bool WebSocketClinet::sendTextImplement(const std::string& text)
{
    if (!sendAvailable.load()) return false;
    std::lock_guard<std::mutex> lockGuard(sendMutex);

    FramTool::client::setText(frameData, text);
    const std::vector<char> buf = frameData->getData();
    const size_t sendSize = tcpSocket->send(buf.data(), buf.size());
    return (buf.size() == sendSize);
}

bool WebSocketClinet::sendBinaryImplement(const char* const source, const size_t size)
{
    if (!sendAvailable.load()) return false;
    std::lock_guard<std::mutex> lockGuard(sendMutex);

    std::vector<char> tmp(source, source + size);
    FramTool::client::setBinary(frameData, tmp);
    const std::vector<char> buf = frameData->getData();
    const size_t sendSize = tcpSocket->send(buf.data(), buf.size());
    return (buf.size() == sendSize);
}

bool WebSocketClinet::sendCloseImplement()
{
    if (!sendAvailable.load()) return false;
    std::lock_guard<std::mutex> lockGuard(sendMutex);
    FramTool::client::setClose(frameData);
    const std::vector<char> buf = frameData->getData();
    const size_t sendSize = tcpSocket->send(buf.data(), buf.size());
    const bool result = (buf.size() == sendSize);
    if (result) sendAvailable.store(false);
    return result;
}

bool WebSocketClinet::sendPingImplement()
{
    if (!sendAvailable.load()) return false;
    std::lock_guard<std::mutex> lockGuard(sendMutex);

    FramTool::client::setPing(frameData);
    const std::vector<char> buf = frameData->getData();
    const size_t sendSize = tcpSocket->send(buf.data(), buf.size());
    return (buf.size() == sendSize);
}

bool WebSocketClinet::sendPongImplement()
{
    if (!sendAvailable.load()) return false;
    std::lock_guard<std::mutex> lockGuard(sendMutex);

    FramTool::client::setPong(frameData);
    const std::vector<char> buf = frameData->getData();
    const size_t sendSize = tcpSocket->send(buf.data(), buf.size());
    return (buf.size() == sendSize);
}

bool WebSocketClinet::recvDataImplement(Opcode::Type& opcodeType, std::vector<char>& direct)
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

bool WebSocketClinet::handshakeProcess(const std::string& address)
{
    const std::string handshakeData = getHandshakeData(address);
    const size_t sendSize = tcpSocket->send(handshakeData.c_str(), handshakeData.size());
    if (sendSize != handshakeData.size()) return false;

    std::vector<char> recvHandshakeBuf(1024);
    const size_t revcSize = tcpSocket->receive(recvHandshakeBuf.data(), recvHandshakeBuf.size());
    const std::string recvHandshakeString(recvHandshakeBuf.data(), revcSize);
    if (!checkSecWebSocketAccept(recvHandshakeString)) return false;
    return true;
}

std::string WebSocketClinet::getHandshakeData(const std::string& address)
{
    std::string result = "GET / HTTP/1.1\r\n";
    result += "Host: " + address + "\r\n";
    result += "Upgrade: websocket\r\n";
    result += "Connection: Upgrade\r\n";
    const std::string secWebSocketKey = getSecWebSocketKey();
    result += "Sec-WebSocket-Key: "+ secWebSocketKey + "\r\n";
    result += "Sec-WebSocket-Version: 13\r\n";
    result += "\r\n";
    return result;
}

std::string WebSocketClinet::getSecWebSocketKey()
{
    static const size_t secWebSocketKeyDataSize = 16;

    std::random_device randomDevice;
    const unsigned int randomSeedValue = randomDevice();
    std::default_random_engine randomEngine{ randomSeedValue };
    std::uniform_int_distribution<int> uniformIntDis{ 0, 255 };

    std::vector<char> dataBuf;
    for (size_t count = 0; count < secWebSocketKeyDataSize; ++count) {
        const char randomValue = static_cast<char>(uniformIntDis(randomEngine));
        dataBuf.push_back(randomValue);
    }

    Base64Processor::Base64Encoder::encode(dataBuf.data(), dataBuf.size(), secWebSocketKeyBuf);
    return secWebSocketKeyBuf;
}

bool WebSocketClinet::checkSecWebSocketAccept(const std::string& recvHandshakeData)
{
    const std::string checkString = getCheckString();
    const std::string recvCheckString = getRecvCheckString(recvHandshakeData);
    if (checkString != recvCheckString) return false;
    return true;
}

std::string WebSocketClinet::getCheckString()
{
    static const std::string keyWord = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    const std::string accpetOriginData = secWebSocketKeyBuf + keyWord;
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

std::string WebSocketClinet::getRecvCheckString(const std::string& recvHandshakeData)
{
    static const std::string acceptHeader = "Sec-WebSocket-Accept:";
    const std::string::size_type beginPosition = recvHandshakeData.find(acceptHeader);
    if (beginPosition == std::string::npos) return "";
    const std::string::size_type dataStartPosition = beginPosition + acceptHeader.size();
    const std::string::size_type endPosition = recvHandshakeData.find("\r\n", dataStartPosition);
    if (beginPosition == std::string::npos) return "";
    const std::string::size_type dataSize = endPosition - dataStartPosition;
    std::string result = recvHandshakeData.substr(dataStartPosition, dataSize);
    result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
    return result;
}
