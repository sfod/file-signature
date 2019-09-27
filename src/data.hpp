#pragma once

#include <memory>
#include <vector>


namespace signature {

struct Buffer {
    std::unique_ptr<char[]> data;
    size_t size;
};

struct InputData {
    Buffer buf;
    size_t idx;
};

template <typename T>
struct OutputData {
    T hash;
    size_t idx;
};

}
