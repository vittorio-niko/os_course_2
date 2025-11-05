#include "ThreadPoolThreads.h"

ThreadPoolThreads::ThreadPoolThreads() : ThreadPool(false, 0) {}

ThreadPoolThreads::~ThreadPoolThreads() { stop(); }

void ThreadPoolThreads::start() {
    size_t threadCount = getSystemNumberOfThreads();
    workers.reserve(threadCount);
    for (size_t i = 0; i < threadCount; ++i) {
        workers.emplace_back(&ThreadPoolThreads::workerLoop, this);
    }
}

void ThreadPoolThreads::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.push(std::move(task));
    }
    taskAvailable.notify_one();
}

void ThreadPoolThreads::waitAll() {
    std::unique_lock<std::mutex> lock(queueMutex);
    allDone.wait(lock, [&]() { return tasks.empty() && activeCount == 0; });
}

void ThreadPoolThreads::stop() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stopFlag = true;
    }
    taskAvailable.notify_all();

    for (auto& t : workers) {
        if (t.joinable()) {
            t.join();
        }
    }
    workers.clear();
}

size_t ThreadPoolThreads::getSystemNumberOfThreads() const {
    return std::thread::hardware_concurrency();
}

void ThreadPoolThreads::workerLoop() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            taskAvailable.wait(lock, [&]() { return stopFlag || !tasks.empty(); });

            if (stopFlag && tasks.empty()) {
                return;
            }
            task = std::move(tasks.front());
            tasks.pop();
            ++activeCount;
        }

        task();

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            --activeCount;
            if (tasks.empty() && activeCount == 0) {
                allDone.notify_all();
            }
        }
    }
}
