#ifndef _6502VM_REGISTER_H
#define _6502VM_REGISTER_H

#include <cstdint>

namespace vm6502 {

struct reg {
    uint16_t pc;
    uint8_t ac;
    uint8_t x;
    uint8_t y;
    uint8_t sr;
    uint8_t sp;
};

}

#endif
