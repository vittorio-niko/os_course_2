#include <pthread.h>
#include <unistd.h> 

#include <vector>

#include "ThreadPool.h"

class ThreadPoolPThreads : public ThreadPool {
public:
    ThreadPoolPThreads();
    ~ThreadPoolPThreads() override;

    void start() override;
    void enqueue(std::function<void()> task) override;
    void waitAll() override;

private:
    size_t getSystemNumberOfThreads() const override;
    static void* workerEntry(void* arg);
    void workerLoop();
    void stop();

private:
    std::vector<pthread_t> workers;

    pthread_mutex_t queueMutex;
    pthread_cond_t taskAvailable;
    pthread_cond_t allDone;
};