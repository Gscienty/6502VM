#include "addr.hpp"

namespace vm6502
{

uint8_t & memory::get(operand_addressing_type type, uint16_t operand)
{
    switch (type) {
    case op_addr_zeropage:
        return this->m[operand];
    case op_addr_zeropage_x:
        return this->m[(uint8_t) (operand + this->r.x)];
    case op_addr_absolute:
        return this->m[operand];
    case op_addr_absolute_x:
        return this->m[operand + this->r.x];
    case op_addr_absolute_y:
        return this->m[operand + this->r.y];
    case op_addr_indirect:
        return this->m[*reinterpret_cast<uint16_t *>(operand)];
    case op_addr_indirect_x:
        return this->m[*reinterpret_cast<uint16_t *>(this->r.x + operand)];
    case op_addr_indirect_y:
        return this->m[*reinterpret_cast<uint16_t *>(operand) + this->r.y];
    default:
        return this->implied;
    }
}

}
