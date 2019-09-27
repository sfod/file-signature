#pragma once

#include <deque>
#include <mutex>
#include <thread>
#include <vector>

#include "data.hpp"
#include "common/semaphore.hpp"
#include "common/sync_queue.hpp"


namespace signature {

template <typename W, typename H>
class Processor {
public:
    Processor(W writer, H hasher, int64_t memory) : writer_(std::move(writer)), hasher_(std::move(hasher)), memory_(memory)
    {
        for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
            thread_pool_.emplace_back(&Processor::do_work, this);
        }
    }

    ~Processor()
    {
        running_ = false;
        input_queue_.release();
        for (auto &t : thread_pool_) {
            t.join();
        }
    }

    /**
     * Process input data using specified hasher and writer.
     */
    void process(InputData input_data)
    {
        memory_.wait(input_data.buf.size);
        input_queue_.push(std::move(input_data));
    }

private:
    /**
     * Read data from shared queue, process data, then pass result to the writer.
     */
    void do_work()
    {
        while (running_ || !input_queue_.empty()) {
            auto input_data = std::move(input_queue_.pop());
            if (!input_data) {
                continue;
            }

            // Calculate hash of a chunk.
            auto h = hasher_.hash(input_data->buf);
            // Pass calculated hash to the writer along with chunk index.
            writer_.write(OutputData<typename H::type>{ h, input_data->idx });

            memory_.notify(input_data->buf.size);
        }
    }

private:
    std::vector<std::thread> thread_pool_;
    SyncQueue<InputData> input_queue_;
    bool running_ = true;
    W writer_;
    H hasher_;
    Semaphore memory_;
};

}
