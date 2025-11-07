#pragma once

#include <thread>
#include <mutex>

#include <queue>
#include <exception>
#include <optional>

template<class T>
class BufferedChannel {
public:
    explicit BufferedChannel(size_t capacity): capacity(capacity), 
        closed(false) {}

    void send(T value) {
        std::unique_lock<std::mutex> lock(data_mutex);
        send_cond_var.wait(lock, [this]() {
            return closed || queue.size() < capacity;
            });
        if (closed) {
            throw std::runtime_error("Channel is closed");
        }

        queue.push(std::move(value));
        receive_cond_var.notify_one();
    }

    std::optional<T> receive() {
        std::unique_lock<std::mutex> lock(data_mutex);
        receive_cond_var.wait(lock, [this]() {
            return closed || !queue.empty();
            });

        if (closed) {
            return std::nullopt;
        }

        T value = std::move(queue.front());
        queue.pop();
        send_cond_var.notify_one();
        return value;
    }

    void close() {
        {
            std::unique_lock<std::mutex> lock(data_mutex);
            closed = true;
        }
        send_cond_var.notify_all();
        receive_cond_var.notify_all();
    }

private:
    std::condition_variable send_cond_var;
    std::condition_variable receive_cond_var;
    std::mutex data_mutex;

    std::queue<T> queue;

    size_t capacity;
    bool closed;
};