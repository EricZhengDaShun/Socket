#include "Interface/WebSocketClientProcess.hpp"

bool WebSocketClientProcess::connectToHost(const std::string& address, const std::string& port)
{
    return connectToHostImplement(address, port);
}

void WebSocketClientProcess::close()
{
    closeImplement();
    return;
}

bool WebSocketClientProcess::sendContinuation(const char* const source, const size_t size)
{
    return sendContinuationImplement(source, size);
}

bool WebSocketClientProcess::sendContinuation(const std::vector<char>& data)
{
    return sendContinuationImplement(data.data(), data.size());
}

bool WebSocketClientProcess::sendText(const std::string& text)
{
    return sendTextImplement(text);
}

bool WebSocketClientProcess::sendText(const std::vector<char>& data)
{
    const std::string tmp(data.data(), data.size());
    return sendTextImplement(tmp);
}

bool WebSocketClientProcess::sendBinary(const char* const source, const size_t size)
{
    return sendBinaryImplement(source, size);
}

bool WebSocketClientProcess::sendBinary(const std::vector<char>& data)
{
    return sendBinaryImplement(data.data(), data.size());
}

bool WebSocketClientProcess::sendClose()
{
    return sendCloseImplement();
}

bool WebSocketClientProcess::sendPing()
{
    return sendPingImplement();
}

bool WebSocketClientProcess::sendPong()
{
    return sendPongImplement();
}

bool WebSocketClientProcess::recvData(Opcode::Type& opcodeType, std::vector<char>& direct)
{
    return recvDataImplement(opcodeType, direct);
}