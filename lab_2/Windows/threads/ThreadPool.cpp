#include "ThreadPool.h"

ThreadPool::ThreadPool(bool stopFlag, size_t activeCount): 
	stopFlag(stopFlag), activeCount(activeCount) {}

ThreadPool::~ThreadPool() {}
