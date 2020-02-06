#include "Interface/SocketTask.hpp"
#include "Interface/SocketInterface.hpp"

SocketTask::SocketTask()
    : isRun(true)
    , taskSocket(nullptr)
    , taskThread(nullptr)
{

}

SocketTask::~SocketTask()
{
    stop();
}

void SocketTask::run(std::unique_ptr<SocketInterface> socket)
{
    taskSocket = std::move(socket);
    taskThread = std::make_unique<std::thread>(&SocketTask::task, this, std::ref(isRun), std::ref(taskSocket));
    return;
}

void SocketTask::stop()
{
    isRun.store(false);
    if (taskSocket.get() != nullptr) taskSocket->close();
    if (taskThread.get() != nullptr) {
        if (taskThread->joinable()) taskThread->join();
    }
    return;
}

bool SocketTask::isRunning() const
{
    if (taskThread.get() == nullptr) return false;
    return taskThread->joinable();
}