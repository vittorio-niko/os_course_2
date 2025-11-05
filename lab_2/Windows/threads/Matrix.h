#pragma once
#include <vector>

#include "Arithmetic.h"

template <Arithmetic T>
class Matrix {
private:
    std::vector<std::vector<T>> data;
    size_t rows;
    size_t cols;

public:
    Matrix() : Matrix(std::vector<std::vector<T>>(1, std::vector<T> (1, 0))) {}
    Matrix(size_t r, size_t c) : rows(r), cols(c), data(r, std::vector<T>(c, 0)) {}

    Matrix(const std::vector<std::vector<T>>& input): data(input), 
        rows(input.size()), cols(input[0].size()) {}

    size_t getRows() const {
        return rows; 
    }
    size_t getCols() const {
        return cols; 
    }

    T& operator()(size_t i, size_t j) { return data[i][j]; }
    const T& operator()(size_t i, size_t j) const { return data[i][j]; }

    void print() const {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                std::cout << std::setw(4) << data[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
};
