#pragma once

#include <concepts>
#include <functional>

template<typename T>
concept Pool = requires(T pool, std::function<void()> task) {
    { pool.start() } -> std::same_as<void>;
    { pool.enqueue(task) } -> std::same_as<void>;
    { pool.waitAll() } -> std::same_as<void>;
};
