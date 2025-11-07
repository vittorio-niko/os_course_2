#include <iostream>
#include <chrono>

#include "MatrixMultiplyer.h"
#include "MatrixExamples.h"

int main() {
    constexpr size_t MATRIX_SIZE = 1000;
    std::cout << "Matrix size: " << MATRIX_SIZE << "\n";
    const Matrix<int> M_1 = MatrixExamples::matrix_example_1(MATRIX_SIZE);
    const Matrix<int> M_2 = MatrixExamples::matrix_example_2(MATRIX_SIZE);

    constexpr size_t CHANNEL_BUFFER_SIZE = 1028;
    MatrixMultiplyer<int> multiplyer(CHANNEL_BUFFER_SIZE);
    auto start = std::chrono::high_resolution_clock::now();
    Matrix<int> result = multiplyer.multiplyParallel(M_1, M_2, 256);
    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "===========Channel-based multiplication: " << time.count() << " mls===========\n";
    //got 319 mls for MATRIX_SIZE = 1000
}

