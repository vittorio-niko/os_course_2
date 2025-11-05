#include "ThreadPoolWin.h"

ThreadPoolWin::ThreadPoolWin(): ThreadPool(false, 0) {
    InitializeCriticalSection(&queueLock);
    taskAvailable = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
    allDone = CreateEvent(NULL, TRUE, TRUE, NULL);
}

ThreadPoolWin::~ThreadPoolWin() {
    stop();
    DeleteCriticalSection(&queueLock);
    CloseHandle(taskAvailable);
    CloseHandle(allDone);
}

void ThreadPoolWin::start() {
    size_t threadCount = getSystemNumberOfThreads();
    workers.reserve(threadCount);
    for (size_t i = 0; i < threadCount; ++i) {
        THREAD_HANDLE hThread = CreateThread(
            NULL, 0, &ThreadPoolWin::workerProc, this, 0, NULL
        );
        if (hThread) {
            workers.push_back(hThread);
        }
    }
}

void ThreadPoolWin::enqueue(std::function<void()> task) {
    EnterCriticalSection(&queueLock);
    tasks.push(std::move(task));
    ResetEvent(allDone);
    LeaveCriticalSection(&queueLock);

    ReleaseSemaphore(taskAvailable, 1, NULL);
}

void ThreadPoolWin::waitAll() {
    WaitForSingleObject(allDone, INFINITE);
}

size_t ThreadPoolWin::getSystemNumberOfThreads() const {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    return sysInfo.dwNumberOfProcessors;
}

void ThreadPoolWin::stop() {
    EnterCriticalSection(&queueLock);
    stopFlag = true;
    LeaveCriticalSection(&queueLock);

    for (size_t i = 0; i < workers.size(); ++i) {
        ReleaseSemaphore(taskAvailable, 1, NULL);
    }
        
    WaitForMultipleObjects((DWORD)workers.size(), workers.data(), TRUE, INFINITE);

    for (THREAD_HANDLE worker : workers) {
        CloseHandle(worker);
    }
        
    workers.clear();
}


DWORD WINAPI ThreadPoolWin::workerProc(LPVOID param) {
    ThreadPoolWin* pool = reinterpret_cast<ThreadPoolWin*>(param);
    pool->workerLoop();
    return 0;
}

void ThreadPoolWin::workerLoop() {
    while (true) {
        std::function<void()> task;
        WaitForSingleObject(taskAvailable, INFINITE);

        {
            EnterCriticalSection(&queueLock);
            if (stopFlag && tasks.empty()) {
                LeaveCriticalSection(&queueLock);
                break;
            }

            if (tasks.empty()) {
                LeaveCriticalSection(&queueLock);
                continue;
            }

            task = std::move(tasks.front());
            tasks.pop();
            ++activeCount;
            LeaveCriticalSection(&queueLock);
        }
        
        task();

        {
            EnterCriticalSection(&queueLock);
            --activeCount;
            if (tasks.empty() && activeCount == 0) {
                SetEvent(allDone);
            }
            LeaveCriticalSection(&queueLock);
        }
    }
}
