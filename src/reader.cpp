#include "reader.hpp"

#include <filesystem>
#include <iostream>


namespace signature {

Reader::Reader(const std::string &filename, size_t block_size)
        : block_size_(block_size), fm_(filename.c_str(), bip::read_only)
{
    // NOTE mmap call fails with a generalized error on an empty files which are a valid input,
    // so handle this case separately.
    if (std::filesystem::file_size(std::filesystem::path(filename)) != 0) {
        region_ = bip::mapped_region(fm_, bip::read_only);
    }
}

bool Reader::has_next() const
{
    return offset_ < region_.get_size();
}

InputData Reader::next()
{
    uint64_t start = offset_;
    uint64_t end = start + block_size_;
    if (end > region_.get_size()) {
        end = region_.get_size();
    }

    offset_ = end;

    char *addr = static_cast<char *>(region_.get_address());
    auto ptr = std::make_unique<char[]>(end - start);
    std::strncpy(ptr.get(), addr + start, end - start);

    return { { std::move(ptr), end - start }, idx_++ };
}

}
