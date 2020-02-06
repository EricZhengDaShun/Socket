#include "EchoTask.hpp"
#include <vector>
#include "Interface/SocketInterface.hpp"

EchoTask::EchoTask()
{

}

EchoTask::~EchoTask()
{

}

void EchoTask::task(const std::atomic<bool>& isRunState, std::unique_ptr<SocketInterface>& socket)
{
    std::vector<char> buf(1024);
    while (isRunState.load()) {
        const size_t dataSize = socket->receive(buf.data(), buf.size());
        if (dataSize == 0) break;
        const size_t sendDataSize = socket->send(buf.data(), dataSize);
        if (sendDataSize != dataSize) break;
    }
    return;
}