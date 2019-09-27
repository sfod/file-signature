#pragma once

#include <deque>
#include <fstream>
#include <mutex>
#include <string>
#include <vector>

#include "data.hpp"


namespace signature {

class Writer {
public:
    explicit Writer(const std::string &filename);

    Writer(const Writer &writer) = delete;
    Writer(Writer &&writer);

    Writer &operator=(const Writer &writer) = delete;
    Writer &operator=(Writer &&writer) = delete;

    /**
     * Write output data at the offset calculated by the chunk index.
     */
    template <typename T>
    void write(OutputData<T> output_data)
    {
        std::lock_guard lock(mutex_);
        size_t offset = output_data.idx * sizeof(output_data.hash);
        ofs_.seekp(offset);
        ofs_.write(reinterpret_cast<const char *>(&output_data.hash), sizeof(output_data.hash));
    }

private:
    std::ofstream ofs_;
    std::mutex mutex_;
};

}
