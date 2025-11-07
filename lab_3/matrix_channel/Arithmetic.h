#pragma once

#include <concepts>

template<typename T>
concept Arithmetic = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
    { a * b } -> std::convertible_to<T>;
    { a - b } -> std::convertible_to<T>;
    { a / b } -> std::convertible_to<T>;
    requires std::copyable<T>;
};



