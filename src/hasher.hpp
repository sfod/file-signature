#pragma once

#include <cstdint>
#include <vector>

#include "data.hpp"


namespace signature {

class Hasher {
public:
    using type = uint32_t;

public:
    type hash(const Buffer &buf);

};

}
