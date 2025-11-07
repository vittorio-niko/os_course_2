#pragma once

#include <functional>
#include <iostream>

#include "Arithmetic.h"
#include "Matrix.h"
#include "BufferedChannel.h"

template<Arithmetic T> 
class MatrixMultiplyer {
public:
	MatrixMultiplyer(size_t buffer_capacity): channel(buffer_capacity), stopped(false) {
        size_t thread_count = std::thread::hardware_concurrency();
        workers.reserve(thread_count);
        for (size_t i = 0; i < thread_count; ++i) {
            workers.emplace_back(&MatrixMultiplyer::worker_loop, this);
        }
    }

    ~MatrixMultiplyer() { stop(); }

    Matrix<T> multiplyParallel(const Matrix<T>& A, const Matrix<T>& B,
        int blockSize ) {
        size_t n = A.getRows();
        size_t p = B.getCols();

        Matrix<T> result(n, p);
        std::atomic<size_t> completed_blocks{ 0 };

        size_t numRowBlocks = (n + blockSize - 1) / blockSize;
        size_t numColBlocks = (p + blockSize - 1) / blockSize;
        size_t total_blocks = numRowBlocks * numColBlocks;

        for (size_t blockRow = 0; blockRow < numRowBlocks; ++blockRow) {
            for (size_t blockCol = 0; blockCol < numColBlocks; ++blockCol) {
                size_t startRow = blockRow * blockSize;
                size_t endRow = std::min(startRow + blockSize, n);

                size_t startCol = blockCol * blockSize;
                size_t endCol = std::min(startCol + blockSize, p);

                try {
                    channel.send([&, startRow, endRow, startCol, endCol]() {
                        multiplyBlock(A, B, result, startRow, endRow, startCol, endCol);
                        completed_blocks.fetch_add(1, std::memory_order_release);
                        });
                }
                catch (const std::runtime_error& e) {
                    std::cout << e.what() << "\n";
                    return Matrix<T>();
                }
                
            }
        }
        waitForCompletion(completed_blocks, total_blocks);
        return result;
    }

private:
    void waitForCompletion(std::atomic<size_t>& completed, size_t total) {
        while (completed.load(std::memory_order_acquire) < total) {
            std::this_thread::yield();
        }
    }

    static void multiplyBlock(const Matrix<T>& A, const Matrix<T>& B, Matrix<T>& result,
        size_t startRow, size_t endRow, size_t startCol, size_t endCol) {
        size_t m = A.getCols();

        for (size_t i = startRow; i < endRow; ++i) {
            for (size_t k = 0; k < m; ++k) {           
                T a_ik = A(i, k);                      
                for (size_t j = startCol; j < endCol; ++j) {
                    result(i, j) += a_ik * B(k, j);    
                }
            }
        }
    }

    void stop() {
        if (!stopped.exchange(true)) {
            channel.close();
            for (auto& worker : workers) {
                if (worker.joinable()) {
                    worker.join();
                }
            }
        }
    }

    void worker_loop() {
        while (true) {
            auto task = channel.receive();
            if (task.has_value()) {
                auto task_function = task.value();
                task_function();
            }
            else {
                break;
            }
        }
    }

private:
    BufferedChannel<std::function<void()>> channel;
    std::vector<std::thread> workers;
    std::atomic<bool> stopped;
};
