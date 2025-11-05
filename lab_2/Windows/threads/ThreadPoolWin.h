#pragma once

#include <Windows.h>

#include <vector>

#include "ThreadPool.h"

typedef void* THREAD_HANDLE;
typedef void* EVENT_HANDLE;
typedef void* SEMAPHORE_HANDLE;

class ThreadPoolWin: public ThreadPool {
public:
    ThreadPoolWin();
    ~ThreadPoolWin() override;

    void start() override;
    void enqueue(std::function<void()> task) override;
    void waitAll() override;

private:
    static DWORD WINAPI workerProc(LPVOID param);
    void workerLoop();
    void stop();
    size_t getSystemNumberOfThreads() const override;

private:
    std::vector<THREAD_HANDLE> workers;

    CRITICAL_SECTION queueLock;
    SEMAPHORE_HANDLE taskAvailable;
    EVENT_HANDLE allDone;
};
