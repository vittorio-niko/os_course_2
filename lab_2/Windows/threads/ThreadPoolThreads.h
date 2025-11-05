#pragma once

#include <thread>
#include <mutex>

#include <vector>

#include "ThreadPool.h"

class ThreadPoolThreads: public ThreadPool {
public:
    ThreadPoolThreads();
    ~ThreadPoolThreads() override;

    void start() override;
    void enqueue(std::function<void()> task) override;
    void waitAll() override;

private:
    size_t getSystemNumberOfThreads() const override;
    void workerLoop();
    void stop();
    
private:
    std::vector<std::thread> workers;

    std::mutex queueMutex;
    std::condition_variable taskAvailable;
    std::condition_variable allDone;
};
