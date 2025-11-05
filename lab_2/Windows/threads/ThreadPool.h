#pragma once

#include <functional>
#include <queue>

class ThreadPool {
protected:
	ThreadPool(bool stopFlag, size_t activeCount);

public:
	virtual void start() = 0;
	virtual void enqueue(std::function<void()> task) = 0;
	virtual void waitAll() = 0;

	virtual ~ThreadPool() = 0;

protected:
	virtual size_t getSystemNumberOfThreads() const = 0;

protected:
	std::queue<std::function<void()>> tasks;

	bool stopFlag;
	size_t activeCount;
};
