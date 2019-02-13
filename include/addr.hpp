#ifndef _6502VM_ADDR_H
#define _6502VM_ADDR_H

#include "register.hpp"
#include <cstdint>

namespace vm6502
{

enum operand_addressing_type
{
    op_addr_implied,
    op_addr_acc,
    op_addr_relative,
    op_addr_immidiate,
    op_addr_zeropage,
    op_addr_zeropage_x,
    op_addr_absolute,
    op_addr_absolute_x,
    op_addr_absolute_y,
    op_addr_indirect,
    op_addr_indirect_x,
    op_addr_indirect_y
};

class memory {
private:
    reg r;
    uint8_t m[0x10000];
    uint8_t implied;
public:
    uint8_t & get(operand_addressing_type type, uint16_t operand);
};

}

#endif
