#pragma once

#include "Matrix.h"

namespace MatrixExamples {
    Matrix<int> matrix_example_1(size_t size) {
        std::vector<std::vector<int>> m(size, std::vector<int>(size, 0));
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                m[i][j] = static_cast<int>(i + j);
            }
        }
        return Matrix<int>(m);
    }
    Matrix<int> matrix_example_2(int size) {
        std::vector<std::vector<int>> m(size, std::vector<int>(size, 0));
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                m[i][j] = (i == j) ? 2 : 1;
            }
        }
        return Matrix<int>(m);
    }
    Matrix<int> matrix_example_3(int size) {
        std::vector<std::vector<int>> m(size, std::vector<int>(size, 0));
        int counter = 1;
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                m[i][j] = counter++;
            }
        }
        return Matrix<int>(m);
    }
}
