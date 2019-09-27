#include "writer.hpp"

#include <iostream>


namespace signature {

Writer::Writer(const std::string &filename)
{
    ofs_.open(filename);
    if (!ofs_) {
        throw std::system_error(errno, std::system_category(), "Failed to open " + filename);
    }
}

Writer::Writer(Writer &&writer) : ofs_(std::move(writer.ofs_))
{
}

}
