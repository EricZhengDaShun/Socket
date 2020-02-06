#pragma once

#include <atomic>
#include <memory>
#include "Interface/SocketTask.hpp"

class SocketInterface;

class EchoTask : public SocketTask
{
public:
    explicit EchoTask();
    ~EchoTask() override;
    EchoTask(const EchoTask&) = delete;
    EchoTask& operator=(const EchoTask&) = delete;

private:
    void task(const std::atomic<bool>& isRunState, std::unique_ptr<SocketInterface>& socket) override;
};

