#!/bin/bash

OUTPUT=program

SOURCES=(
    "src/ThreadPool.cpp"
    "src/ThreadPoolPThreads.cpp"
	"src/threads.cpp"
)

g++ -std=c++20 -Wall -Wextra -pthread -Iinclude "${SOURCES[@]}" -o $OUTPUT

if [ $? -eq 0 ]; then
    echo "Build successful, run: ./$(basename $OUTPUT)"
else
    echo "Build failed"
fi