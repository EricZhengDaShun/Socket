#pragma once
#include <atomic>
#include <memory>
#include <thread>

class SocketInterface;

class SocketTask
{
public:
    virtual ~SocketTask();
    SocketTask(const SocketTask&) = default;
    SocketTask& operator=(const SocketTask&) = default;

    void run(std::unique_ptr<SocketInterface> socket);
    void stop();

    bool isRunning() const;

protected:
    SocketTask();

private:
    std::atomic<bool> isRun;
    std::unique_ptr<SocketInterface> taskSocket;
    std::unique_ptr<std::thread> taskThread;

private:
    virtual void task(const std::atomic<bool>& isRunState, std::unique_ptr<SocketInterface>& socket) = 0;
};
