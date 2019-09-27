#include "hasher.hpp"

namespace signature {

Hasher::type Hasher::hash(const Buffer &buf)
{
    uint32_t crc = 0;
    crc = ~crc;

    size_t len = buf.size;
    const char *s = buf.data.get();

    while (len--) {
        crc ^= *s++;
        for (int k = 0; k < 8; k++)
            crc = crc & 1 ? (crc >> 1) ^ 0x82f63b78 : crc >> 1;
    }

    return ~crc;
}

}
