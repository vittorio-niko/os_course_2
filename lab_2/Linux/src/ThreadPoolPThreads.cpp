#include "ThreadPoolPThreads.h"

ThreadPoolPThreads::ThreadPoolPThreads() : ThreadPool(false, 0) {
    pthread_mutex_init(&queueMutex, nullptr);
    pthread_cond_init(&taskAvailable, nullptr);
    pthread_cond_init(&allDone, nullptr);
}

ThreadPoolPThreads::~ThreadPoolPThreads() {
    stop();
    pthread_mutex_destroy(&queueMutex);
    pthread_cond_destroy(&taskAvailable);
    pthread_cond_destroy(&allDone);
}

size_t ThreadPoolPThreads::getSystemNumberOfThreads() const {
    return sysconf(_SC_NPROCESSORS_ONLN); 
}

void ThreadPoolPThreads::start() {
    size_t threadCount = getSystemNumberOfThreads();
    workers.resize(threadCount);
    for (size_t i = 0; i < threadCount; ++i) {
        pthread_create(&workers[i], nullptr, &ThreadPoolPThreads::workerEntry, this);
    }
}

void* ThreadPoolPThreads::workerEntry(void* arg) {
    ThreadPoolPThreads* pool = static_cast<ThreadPoolPThreads*>(arg);
    pool->workerLoop();
    return nullptr;
}

void ThreadPoolPThreads::workerLoop() {
    while (true) {
        std::function<void()> task;

        pthread_mutex_lock(&queueMutex);
        while (!stopFlag && tasks.empty()) {
            pthread_cond_wait(&taskAvailable, &queueMutex);
        }

        if (stopFlag && tasks.empty()) {
            pthread_mutex_unlock(&queueMutex);
            return;
        }

        task = std::move(tasks.front());
        tasks.pop();
        ++activeCount;
        pthread_mutex_unlock(&queueMutex);

        task();

        pthread_mutex_lock(&queueMutex);
        --activeCount;
        if (tasks.empty() && activeCount == 0) {
            pthread_cond_broadcast(&allDone);
        }
        pthread_mutex_unlock(&queueMutex);
    }
}

void ThreadPoolPThreads::enqueue(std::function<void()> task) {
    pthread_mutex_lock(&queueMutex);
    tasks.push(std::move(task));
    pthread_mutex_unlock(&queueMutex);
    pthread_cond_signal(&taskAvailable);
}

void ThreadPoolPThreads::waitAll() {
    pthread_mutex_lock(&queueMutex);
    while (!tasks.empty() || activeCount != 0) {
        pthread_cond_wait(&allDone, &queueMutex);
    }
    pthread_mutex_unlock(&queueMutex);
}

void ThreadPoolPThreads::stop() {
    pthread_mutex_lock(&queueMutex);
    stopFlag = true;
    pthread_mutex_unlock(&queueMutex);

    pthread_cond_broadcast(&taskAvailable);

    for (auto& t : workers) {
        pthread_join(t, nullptr);
    }
    workers.clear();
}