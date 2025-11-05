#include <iostream>
#include <chrono>

#include "MatrixExamples.h"

#include "MatrixOperationsWithPool.h"
#include "ThreadPoolPThreads.h"

void flushCache() {
    constexpr size_t cacheSize = 32 * 1024 * 1024; 
    static std::vector<char> trash(cacheSize);
    for (auto& x : trash) {
        x = static_cast<char>(x + 1);
    }
}

int main() {
    constexpr int MATRIX_SIZE = 100;
    std::cout << "Matrix size: " << MATRIX_SIZE << "\n";
    const Matrix<int> M_1 = MatrixExamples::matrix_example_1(MATRIX_SIZE);
    const Matrix<int> M_2 = MatrixExamples::matrix_example_2(MATRIX_SIZE);
    const Matrix<int> M_3 = MatrixExamples::matrix_example_3(MATRIX_SIZE);
    
    //pthread.h part
    {
        flushCache();
        auto start = std::chrono::high_resolution_clock::now();
        Matrix<int> result = MatrixOperationsWithPool<int, ThreadPoolPThreads>::multiply(M_1, M_2);
        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "===========1-thread multiplication: " << time.count() << " mcs===========\n";
    }
    {
        constexpr int N = 20;
        Matrix<int> result;
        std::cout << "===========Pthread.h library===========\n";
        long long int time_counter = 0;
        for (int k = 1; k < N + 1; ++k) {
            flushCache();
            auto start = std::chrono::high_resolution_clock::now();
            result = MatrixOperationsWithPool<int, ThreadPoolPThreads>::multiplyParallel(M_1, M_2, k);
            auto end = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            time_counter += time.count();
            std::cout << "Block size: " << k << "x" << k << ", " << time.count() << " mcs\n";
        }
        std::cout << "Average time: " << static_cast<long double>(time_counter) / N << " mcs\n";
    }

    return 0;
}


