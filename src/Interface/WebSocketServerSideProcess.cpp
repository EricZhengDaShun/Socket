#include "Interface/WebSocketServerSideProcess.hpp"
#include "Interface/SocketInterface.hpp"

bool WebSocketServerSideProcess::acceptSocket(std::unique_ptr<SocketInterface> socket)
{
    return acceptSocketImplement(std::move(socket));
}

void WebSocketServerSideProcess::close()
{
    closeImplement();
    return;
}

bool WebSocketServerSideProcess::sendContinuation(const char* const source, const size_t size)
{
    return sendContinuationImplement(source, size);
}

bool WebSocketServerSideProcess::sendContinuation(const std::vector<char>& data)
{
    return sendContinuationImplement(data.data(), data.size());
}

bool WebSocketServerSideProcess::sendText(const std::string& text)
{
    return sendTextImplement(text);
}

bool WebSocketServerSideProcess::sendText(const std::vector<char>& data)
{
    const std::string tmp(data.data(), data.size());
    return sendTextImplement(tmp);
}

bool WebSocketServerSideProcess::sendBinary(const char* const source, const size_t size)
{
    return sendBinaryImplement(source, size);
}

bool WebSocketServerSideProcess::sendBinary(const std::vector<char>& data)
{
    return sendBinaryImplement(data.data(), data.size());
}

bool WebSocketServerSideProcess::sendClose()
{
    return sendCloseImplement();
}

bool WebSocketServerSideProcess::sendPing()
{
    return sendPingImplement();
}

bool WebSocketServerSideProcess::sendPong()
{
    return sendPongImplement();
}

bool WebSocketServerSideProcess::recvData(Opcode::Type& opcodeType, std::vector<char>& direct)
{
    return recvDataImplement(opcodeType, direct);
}