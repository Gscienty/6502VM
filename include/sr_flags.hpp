#ifndef _6502VM_SR_FLAGS_H
#define _6502VM_SR_FLAGS_H

#include <cstdint>

namespace vm6502
{

template <uint64_t N>
struct sr_flags_mask
{
    static uint8_t const mask = sr_flags_mask<N / 10>::mask * 2 + N % 10;
};

template <>
struct sr_flags_mask<0>
{
    static uint8_t const mask = 0;
};

enum sr_flags : uint8_t
{
    sr_flags_negative  = sr_flags_mask<1>::mask,
    sr_flags_overflow  = sr_flags_mask<10>::mask,
    sr_flags_ignore    = sr_flags_mask<100>::mask,
    sr_flags_break     = sr_flags_mask<1000>::mask,
    sr_flags_decimal   = sr_flags_mask<10000>::mask,
    sr_flags_interrupt = sr_flags_mask<100000>::mask,
    sr_flags_zero      = sr_flags_mask<1000000>::mask,
    sr_flags_carry     = sr_flags_mask<10000000>::mask
};


template <sr_flags Flag>
struct sr_operator
{
    static bool is_set(uint8_t sr)
    {
        return (Flag & sr) != 0x00;
    }

    static bool is_clear(uint8_t sr)
    {
        return (Flag & sr) == 0x00;
    }

    static bool set(uint8_t & sr)
    {
        sr |= Flag;
    }

    static bool clear(uint8_t & sr)
    {
        sr &= ~Flag;
    }
};

}

#endif
