#pragma once

#include <string>
#include <vector>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include "data.hpp"


namespace signature {

namespace bip = boost::interprocess;

class Reader {
public:
    Reader(const std::string &filename, size_t block_size);
    /**
     * Check if input file has data to read.
     */
    bool has_next() const;

    /**
     * Read next chunk of data from input file.
     *
     * \note The last chunk size might be smaller than `block_size`.
     */
    InputData next();

private:
    size_t block_size_;
    bip::file_mapping fm_;
    bip::mapped_region region_;
    uint64_t offset_ = 0;
    uint64_t idx_ = 0;
};

}
