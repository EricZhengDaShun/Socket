#pragma once
#include <memory>
#include <atomic>
#include <vector>
#include <thread>
#include <mutex>

class SocketInterface;
class SocketTask;

class SocketTaskManager final
{
public:
    explicit SocketTaskManager();
    ~SocketTaskManager();
    SocketTaskManager(const SocketTaskManager&) = delete;
    SocketTaskManager& operator=(const SocketTaskManager&) = delete;

    void addSocketAndTask(std::unique_ptr<SocketInterface> socket, std::unique_ptr<SocketTask> uniqueTask);

private:
    void scan();

private:
    std::atomic<bool> run;
    std::unique_ptr<std::thread> scanThread;
    std::vector<std::unique_ptr<SocketTask>> taskList;
    std::mutex taskListMutex;
};

