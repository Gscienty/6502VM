#ifndef _6502VM_SIGNED_H
#define _6502VM_SIGNED_H

#include <cstdint>

namespace vm6502
{

struct byte_meta
{
    static const int bit_position = 0x80;
};

struct word_meta
{
    static const int bit_position = 0x8000;
};

template <typename Signed_Meta>
struct signed_operator
{
    inline static bool is_negative(uint16_t val)
    {
        return (Signed_Meta::bit_position & val) != 0;
    }
};

}

#endif
