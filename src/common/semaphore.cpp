#include "semaphore.hpp"


Semaphore::Semaphore(int64_t cnt) : cnt_(cnt)
{
}

void Semaphore::notify(int64_t cnt)
{
    std::lock_guard lock(mutex_);
    cnt_ += cnt;
    cv_.notify_one();
}

void Semaphore::wait(int64_t cnt)
{
    std::unique_lock lock(mutex_);
    while (cnt_ < cnt) {
        cv_.wait(lock);
    }
    cnt_ -= cnt;
}
