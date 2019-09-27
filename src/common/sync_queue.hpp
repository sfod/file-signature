#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>


template <typename T>
class SyncQueue {
public:
    void push(T x)
    {
        auto start = std::chrono::steady_clock::now();
        std::lock_guard lock(mutex_);
        auto end = std::chrono::steady_clock::now();
        dur_ += end - start;

        queue_.push_back(std::move(x));
        cv_.notify_one();
    }

    std::optional<T> pop()
    {
        auto start = std::chrono::steady_clock::now();
        std::unique_lock lock(mutex_);
        auto end = std::chrono::steady_clock::now();
        dur_ += end - start;

        while (queue_.empty()) {
            cv_.wait_for(lock, std::chrono::milliseconds(100));
            if (release_) {
                return std::nullopt;
            }
        }

        T x = std::move(queue_.front());
        queue_.pop_front();
        return std::move(x);
    }

    bool empty() const noexcept
    {
        auto start = std::chrono::steady_clock::now();
        std::lock_guard lock(mutex_);
        auto end = std::chrono::steady_clock::now();
        dur_ += end - start;

        return queue_.empty();
    }

    void release()
    {
        release_ = true;
    }

    double duration() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(dur_).count();
    }

private:
    std::deque<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    bool release_ = false;
    mutable std::chrono::nanoseconds dur_;
};
