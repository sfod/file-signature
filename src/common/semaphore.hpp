#pragma once

#include <condition_variable>
#include <mutex>

class Semaphore {
public:
    explicit Semaphore(int64_t cnt = 0);

    void notify(int64_t cnt);
    void wait(int64_t cnt);

private:
    int64_t cnt_;
    std::condition_variable cv_;
    std::mutex mutex_;
};
