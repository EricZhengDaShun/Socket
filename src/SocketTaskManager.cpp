#include "SocketTaskManager.hpp"
#include <chrono>
#include "Interface/SocketInterface.hpp"
#include "Interface/SocketTask.hpp"

SocketTaskManager::SocketTaskManager()
    : run(true)
    , scanThread(std::make_unique<std::thread>(&SocketTaskManager::scan, this))
{

}

SocketTaskManager::~SocketTaskManager()
{
    run.store(false);
    if (scanThread->joinable()) {
        scanThread->join();
    }
    std::lock_guard<std::mutex> lockGuard(taskListMutex);
    for (std::unique_ptr<SocketTask>& socketTask : taskList) {
        if (socketTask->isRunning()) {
            socketTask->stop();
        }
    }
}

void SocketTaskManager::addSocketAndTask(std::unique_ptr<SocketInterface> socket, std::unique_ptr<SocketTask> uniqueTask)
{
    if (!run.load()) return;

    uniqueTask->run(std::move(socket));
    std::lock_guard<std::mutex> lockGuard(taskListMutex);
    taskList.push_back(std::move(uniqueTask));
    return;
}

void SocketTaskManager::scan()
{
    while (run.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::lock_guard<std::mutex> lockGuard(taskListMutex);
        for (auto it = taskList.begin(); it != taskList.end(); ) {
            const bool isRunning = (*it)->isRunning();
            if (!isRunning) {
                it = taskList.erase(it);
            } else {
                ++it;
            }
        }
    }
    return;
}
