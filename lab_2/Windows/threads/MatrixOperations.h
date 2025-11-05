#pragma once

#include <thread>

#include "Arithmetic.h"
#include "Matrix.h"

template <Arithmetic T>
class MatrixOperations {
public:
    //1-thread multiplication
    static Matrix<T> multiply(const Matrix<T>& A, const Matrix<T>& B) {
        size_t n = A.getRows();
        size_t m = A.getCols();
        size_t p = B.getCols();

        Matrix<T> result(n, p);

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < p; ++j) {
                T sum = 0;
                for (size_t k = 0; k < m; ++k) {
                    sum += A(i, k) * B(k, j);
                }
                result(i, j) = sum;
            }
        }

        return result;
    }

    static Matrix<T> multiplyParallel(const Matrix<T>& A, const Matrix<T>& B, 
        int blockSize) {
        size_t n = A.getRows();
        size_t p = B.getCols();

        Matrix<T> result(n, p);
        std::vector<std::thread> threads;

        size_t numRowBlocks = (n + blockSize - 1) / blockSize;
        size_t numColBlocks = (p + blockSize - 1) / blockSize;

        for (size_t blockRow = 0; blockRow < numRowBlocks; ++blockRow) {
            for (size_t blockCol = 0; blockCol < numColBlocks; ++blockCol) {
                size_t startRow = blockRow * blockSize;
                size_t endRow = std::min(startRow + blockSize, n);

                size_t startCol = blockCol * blockSize;
                size_t endCol = std::min(startCol + blockSize, p);

                threads.emplace_back(multiplyBlock,
                    std::cref(A), std::cref(B), std::ref(result),
                    startRow, endRow, startCol, endCol);
            }
        }

        for (auto& t : threads) {
            t.join();
        }

        return result;
    }

private:
    static void multiplyBlock(const Matrix<T>& A, const Matrix<T>& B, Matrix<T>& result,
        size_t startRow, size_t endRow, size_t startCol, size_t endCol) {
        size_t m = A.getCols();

        for (size_t i = startRow; i < endRow; ++i) {
            for (size_t j = startCol; j < endCol; ++j) {
                T sum = 0;
                for (size_t k = 0; k < m; ++k) {
                    sum += A(i, k) * B(k, j);
                }

                result(i, j) = sum;
            }
        }
    }
};
