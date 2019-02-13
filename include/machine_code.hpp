#ifndef _6502VM_MACHINE_CODE_H
#define _6502VM_MACHINE_CODE_H

#include "instruction.hpp"
#include "addr.hpp"
#include <functional>

namespace vm6502
{

enum operand_type
{
    op_type_zero,
    op_type_ref,
    op_type_one,
    op_type_word,
};

struct mc_instruction
{
    operand_type type;
    int size;
    operand_addressing_type addr_type;

    union {
        void (instruction :: * zero_ins) (void);
        void (instruction :: * one_ins) (uint8_t);
        void (instruction :: * ref_ins) (uint8_t &);
        void (instruction :: * word_ins) (uint16_t);
    } instruction;
};

struct mc_operator
{
    template<operand_addressing_type Addr
        , int Size
        , void (instruction :: * Ins_Impl)(void)>
    static const mc_instruction ctor()
    {
        return {
            .type = op_type_zero,
            .addr_type = Addr,
            .size = Size,
            .instruction.zero_ins = Ins_Impl
        };
    }

    template<operand_addressing_type Addr
        , int Size
        , void (instruction :: * Ins_Impl)(uint8_t)>
    static const mc_instruction ctor()
    {
        return {
            .type = op_type_one,
            .addr_type = Addr,
            .size = Size,
            .instruction.one_ins = Ins_Impl
        };
    }

    template<operand_addressing_type Addr
        , int Size
        , void (instruction :: * Ins_Impl)(uint8_t &)>
    static const mc_instruction ctor()
    {
        return {
            .type = op_type_ref,
            .addr_type = Addr,
            .size = Size,
            .instruction.ref_ins = Ins_Impl
        };
    }

    template<operand_addressing_type Addr
        , int Size
        , void (instruction :: * Ins_Impl)(uint16_t)>
    static const mc_instruction ctor()
    {
        return {
            .type = op_type_ref,
            .addr_type = Addr,
            .size = Size,
            .instruction.word_ins = Ins_Impl
        };
    }
};

mc_instruction mc_mapper[] = {

    [0x69] = mc_operator::ctor<op_addr_immidiate,  2, &instruction::ins_adc>(),
    [0x65] = mc_operator::ctor<op_addr_zeropage,   2, &instruction::ins_adc>(),
    [0x75] = mc_operator::ctor<op_addr_zeropage_x, 2, &instruction::ins_adc>(),
    [0x6D] = mc_operator::ctor<op_addr_absolute,   3, &instruction::ins_adc>(),
    [0x7D] = mc_operator::ctor<op_addr_absolute_x, 3, &instruction::ins_adc>(),
    [0x79] = mc_operator::ctor<op_addr_absolute_y, 3, &instruction::ins_adc>(),
    [0x61] = mc_operator::ctor<op_addr_indirect_x, 2, &instruction::ins_adc>(),
    [0x71] = mc_operator::ctor<op_addr_indirect_y, 2, &instruction::ins_adc>(),

    [0x29] = mc_operator::ctor<op_addr_immidiate,  2, &instruction::ins_and>(),
    [0x25] = mc_operator::ctor<op_addr_zeropage,   2, &instruction::ins_and>(),
    [0x35] = mc_operator::ctor<op_addr_zeropage_x, 2, &instruction::ins_and>(),
    [0x2D] = mc_operator::ctor<op_addr_absolute,   3, &instruction::ins_and>(),
    [0x3D] = mc_operator::ctor<op_addr_absolute_x, 3, &instruction::ins_and>(),
    [0x39] = mc_operator::ctor<op_addr_absolute_y, 3, &instruction::ins_and>(),
    [0x21] = mc_operator::ctor<op_addr_indirect_x, 2, &instruction::ins_and>(),
    [0x31] = mc_operator::ctor<op_addr_indirect_y, 2, &instruction::ins_and>(),

    [0x0A] = mc_operator::ctor<op_addr_acc,        1, &instruction::ins_asl>(),
    [0x06] = mc_operator::ctor<op_addr_zeropage,   2, &instruction::ins_asl>(),
    [0x16] = mc_operator::ctor<op_addr_zeropage_x, 2, &instruction::ins_asl>(),
    [0x0E] = mc_operator::ctor<op_addr_absolute,   3, &instruction::ins_asl>(),
    [0x1E] = mc_operator::ctor<op_addr_absolute_x, 3, &instruction::ins_asl>(),

    [0x90] = mc_operator::ctor<op_addr_relative, 2, &instruction::branch<sr_flags_carry, 0>>(),

    [0xB0] = mc_operator::ctor<op_addr_relative, 2, &instruction::branch<sr_flags_carry, 1>>(),

    [0xF0] = mc_operator::ctor<op_addr_relative, 2, &instruction::branch<sr_flags_zero, 1>>(),

    [0x24] = mc_operator::ctor<op_addr_zeropage, 2, &instruction::ins_bit>(),
    [0x2C] = mc_operator::ctor<op_addr_absolute, 3, &instruction::ins_bit>(),

    [0x30] = mc_operator::ctor<op_addr_relative, 2, &instruction::branch<sr_flags_negative, 1>>(),

    [0xD0] = mc_operator::ctor<op_addr_relative, 2, &instruction::branch<sr_flags_zero, 0>>(),

    [0x10] = mc_operator::ctor<op_addr_relative, 2, &instruction::branch<sr_flags_negative, 0>>(),

    [0x00] = mc_operator::ctor<op_addr_implied, 1, &instruction::ins_brk>(),

    [0x50] = mc_operator::ctor<op_addr_relative, 2, &instruction::branch<sr_flags_overflow, 0>>(),

    [0x70] = mc_operator::ctor<op_addr_relative, 2, &instruction::branch<sr_flags_overflow, 1>>(),

    [0x18] = mc_operator::ctor<op_addr_implied, 1, &instruction::sr_clear<sr_flags_carry>>(),

    [0xD8] = mc_operator::ctor<op_addr_implied, 1, &instruction::sr_clear<sr_flags_decimal>>(),
    
    [0x58] = mc_operator::ctor<op_addr_implied, 1, &instruction::sr_clear<sr_flags_interrupt>>(),

    [0xB8] = mc_operator::ctor<op_addr_implied, 1, &instruction::sr_clear<sr_flags_overflow>>(),

    [0xC9] = mc_operator::ctor<op_addr_immidiate,  2, &instruction::ins_cmp>(),
    [0xC5] = mc_operator::ctor<op_addr_zeropage,   2, &instruction::ins_cmp>(),
    [0xD5] = mc_operator::ctor<op_addr_zeropage_x, 2, &instruction::ins_cmp>(),
    [0xCD] = mc_operator::ctor<op_addr_absolute,   3, &instruction::ins_cmp>(),
    [0xDD] = mc_operator::ctor<op_addr_absolute_x, 3, &instruction::ins_cmp>(),
    [0xD9] = mc_operator::ctor<op_addr_absolute_y, 3, &instruction::ins_cmp>(),
    [0xC1] = mc_operator::ctor<op_addr_indirect_x, 2, &instruction::ins_cmp>(),
    [0xD1] = mc_operator::ctor<op_addr_indirect_y, 2, &instruction::ins_cmp>(),

    [0xE0] = mc_operator::ctor<op_addr_immidiate, 2, &instruction::ins_cpx>(),
    [0xE4] = mc_operator::ctor<op_addr_zeropage,  2, &instruction::ins_cpx>(),
    [0xEC] = mc_operator::ctor<op_addr_absolute,  3, &instruction::ins_cpx>(),

    [0xC0] = mc_operator::ctor<op_addr_immidiate, 2, &instruction::ins_cpy>(),
    [0xC4] = mc_operator::ctor<op_addr_zeropage,  2, &instruction::ins_cpy>(),
    [0xCC] = mc_operator::ctor<op_addr_absolute,  3, &instruction::ins_cpy>(),

    [0xC6] = mc_operator::ctor<op_addr_zeropage,   2, &instruction::ins_dec>(),
    [0xD6] = mc_operator::ctor<op_addr_zeropage_x, 2, &instruction::ins_dec>(),
    [0xCE] = mc_operator::ctor<op_addr_absolute,   3, &instruction::ins_dec>(),
    [0xDE] = mc_operator::ctor<op_addr_absolute_x, 3, &instruction::ins_dec>(),

    [0xCA] = mc_operator::ctor<op_addr_implied, 1, &instruction::ins_dex>(),

    [0x88] = mc_operator::ctor<op_addr_implied, 1, &instruction::ins_dey>(),

    [0x49] = mc_operator::ctor<op_addr_immidiate,  2, &instruction::ins_eor>(),
    [0x45] = mc_operator::ctor<op_addr_zeropage,   2, &instruction::ins_eor>(),
    [0x55] = mc_operator::ctor<op_addr_zeropage_x, 2, &instruction::ins_eor>(),
    [0x4D] = mc_operator::ctor<op_addr_absolute,   3, &instruction::ins_eor>(),
    [0x5D] = mc_operator::ctor<op_addr_absolute_x, 3, &instruction::ins_eor>(),
    [0x59] = mc_operator::ctor<op_addr_absolute_y, 3, &instruction::ins_eor>(),
    [0x41] = mc_operator::ctor<op_addr_indirect_x, 2, &instruction::ins_eor>(),
    [0x51] = mc_operator::ctor<op_addr_indirect_y, 2, &instruction::ins_eor>(),

    [0xE6] = mc_operator::ctor<op_addr_zeropage,   2, &instruction::ins_inc>(),
    [0xF6] = mc_operator::ctor<op_addr_zeropage_x, 2, &instruction::ins_inc>(),
    [0xEE] = mc_operator::ctor<op_addr_absolute,   3, &instruction::ins_inc>(),
    [0xFE] = mc_operator::ctor<op_addr_zeropage_x, 3, &instruction::ins_inc>(),

    [0xE8] = mc_operator::ctor<op_addr_implied, 1, &instruction::ins_inx>(),

    [0xC8] = mc_operator::ctor<op_addr_implied, 1, &instruction::ins_iny>(),

    [0x4C] = mc_operator::ctor<op_addr_absolute, 3, &instruction::ins_jmp>(),
    [0x6C] = mc_operator::ctor<op_addr_indirect, 3, &instruction::ins_jmp>(),

    [0x20] = mc_operator::ctor<op_addr_absolute, 3, &instruction::ins_jsr>(),

    [0xA9] = mc_operator::ctor<op_addr_immidiate,  2, &instruction::ins_lda>(),
    [0xA5] = mc_operator::ctor<op_addr_zeropage,   2, &instruction::ins_lda>(),
    [0xB5] = mc_operator::ctor<op_addr_zeropage_x, 2, &instruction::ins_lda>(),
    [0xAD] = mc_operator::ctor<op_addr_absolute,   3, &instruction::ins_lda>(),
    [0xBD] = mc_operator::ctor<op_addr_absolute_x, 3, &instruction::ins_lda>(),
    [0xB9] = mc_operator::ctor<op_addr_absolute_y, 3, &instruction::ins_lda>(),
    [0xA1] = mc_operator::ctor<op_addr_indirect_x, 2, &instruction::ins_lda>(),
    [0xB1] = mc_operator::ctor<op_addr_indirect_y, 2, &instruction::ins_lda>(),

    [0xA2] = mc_operator::ctor<op_addr_immidiate,  2, &instruction::ins_ldx>(),
    [0xA6] = mc_operator::ctor<op_addr_zeropage,   2, &instruction::ins_ldx>(),
    [0xB6] = mc_operator::ctor<op_addr_zeropage_y, 2, &instruction::ins_ldx>(),
    [0xAE] = mc_operator::ctor<op_addr_absolute,   3, &instruction::ins_ldx>(),
    [0xBE] = mc_operator::ctor<op_addr_absolute_y, 3, &instruction::ins_ldx>(),

    [0xA0] = mc_operator::ctor<op_addr_immidiate,  2, &instruction::ins_ldy>(),
    [0xA4] = mc_operator::ctor<op_addr_zeropage,   2, &instruction::ins_ldy>(),
    [0xB4] = mc_operator::ctor<op_addr_zeropage_x, 2, &instruction::ins_ldy>(),
    [0xAC] = mc_operator::ctor<op_addr_absolute,   3, &instruction::ins_ldy>(),
    [0xBC] = mc_operator::ctor<op_addr_absolute_x, 3, &instruction::ins_ldy>(),

    [0x4A] = mc_operator::ctor<op_addr_acc,        1, &instruction::ins_lsr>(),
    [0x46] = mc_operator::ctor<op_addr_zeropage,   2, &instruction::ins_lsr>(),
    [0x56] = mc_operator::ctor<op_addr_zeropage_x, 2, &instruction::ins_lsr>(),
    [0x4E] = mc_operator::ctor<op_addr_absolute,   3, &instruction::ins_lsr>(),
    [0x5E] = mc_operator::ctor<op_addr_absolute_x, 3, &instruction::ins_lsr>(),

    [0xEA] = mc_operator::ctor<op_addr_implied, 1, &instruction::ins_nop>(),
    
    [0x09] = mc_operator::ctor<op_addr_immidiate,  2, &instruction::ins_ora>(),
    [0x05] = mc_operator::ctor<op_addr_zeropage,   2, &instruction::ins_ora>(),
    [0x15] = mc_operator::ctor<op_addr_zeropage_x, 2, &instruction::ins_ora>(),
    [0x0D] = mc_operator::ctor<op_addr_absolute,   3, &instruction::ins_ora>(),
    [0x1D] = mc_operator::ctor<op_addr_absolute_x, 3, &instruction::ins_ora>(),
    [0x19] = mc_operator::ctor<op_addr_absolute_y, 3, &instruction::ins_ora>(),
    [0x01] = mc_operator::ctor<op_addr_indirect_x, 2, &instruction::ins_ora>(),
    [0x11] = mc_operator::ctor<op_addr_indirect_y, 2, &instruction::ins_ora>(),

    [0x48] = mc_operator::ctor<op_addr_implied, 1, &instruction::ins_pha>(),

    [0x08] = mc_operator::ctor<op_addr_implied, 1, &instruction::ins_php>(),

    [0x68] = mc_operator::ctor<op_addr_implied, 1, &instruction::ins_pla>(),

    [0x28] = mc_operator::ctor<op_addr_implied, 1, &instruction::ins_plp>(),

    [0x2A] = mc_operator::ctor<op_addr_implied,    1, &instruction::ins_rol>(),
    [0x26] = mc_operator::ctor<op_addr_zeropage,   2, &instruction::ins_rol>(),
    [0x36] = mc_operator::ctor<op_addr_zeropage_x, 2, &instruction::ins_rol>(),
    [0x2E] = mc_operator::ctor<op_addr_absolute,   3, &instruction::ins_rol>(),
    [0x3E] = mc_operator::ctor<op_addr_absolute_x, 3, &instruction::ins_rol>(),

    [0x6A] = mc_operator::ctor<op_addr_implied,    1, &instruction::ins_ror>(),
    [0x66] = mc_operator::ctor<op_addr_zeropage,   2, &instruction::ins_ror>(),
    [0x76] = mc_operator::ctor<op_addr_zeropage_x, 2, &instruction::ins_ror>(),
    [0x6E] = mc_operator::ctor<op_addr_absolute,   3, &instruction::ins_ror>(),
    [0x7E] = mc_operator::ctor<op_addr_absolute_x, 3, &instruction::ins_ror>(),

    [0x40] = mc_operator::ctor<op_addr_implied, 1, &instruction::ins_rti>(),

    [0x60] = mc_operator::ctor<op_addr_implied, 1, &instruction::ins_rts>(),

    [0xE9] = mc_operator::ctor<op_addr_immidiate,  2, &instruction::ins_sbc>(),
    [0xE5] = mc_operator::ctor<op_addr_zeropage,   2, &instruction::ins_sbc>(),
    [0xF5] = mc_operator::ctor<op_addr_zeropage_x, 2, &instruction::ins_sbc>(),
    [0xED] = mc_operator::ctor<op_addr_absolute,   3, &instruction::ins_sbc>(),
    [0xFD] = mc_operator::ctor<op_addr_absolute_x, 3, &instruction::ins_sbc>(),
    [0xF9] = mc_operator::ctor<op_addr_absolute_y, 3, &instruction::ins_sbc>(),
    [0xE1] = mc_operator::ctor<op_addr_indirect_x, 2, &instruction::ins_sbc>(),
    [0xF1] = mc_operator::ctor<op_addr_indirect_y, 2, &instruction::ins_sbc>(),

    [0x38] = mc_operator::ctor<op_addr_implied, 1, &instruction::sr_set<sr_flags_carry>>(),

    [0xF8] = mc_operator::ctor<op_addr_implied, 1, &instruction::sr_set<sr_flags_decimal>>(),

    [0x78] = mc_operator::ctor<op_addr_implied, 1, &instruction::sr_set<sr_flags_interrupt>>(),

    [0x85] = mc_operator::ctor<op_addr_zeropage,   2, &instruction::ins_sta>(),
    [0x95] = mc_operator::ctor<op_addr_zeropage_x, 2, &instruction::ins_sta>(),
    [0x8D] = mc_operator::ctor<op_addr_absolute,   3, &instruction::ins_sta>(),
    [0x9D] = mc_operator::ctor<op_addr_absolute_x, 3, &instruction::ins_sta>(),
    [0x99] = mc_operator::ctor<op_addr_absolute_y, 3, &instruction::ins_sta>(),
    [0x81] = mc_operator::ctor<op_addr_indirect_x, 2, &instruction::ins_sta>(),
    [0x91] = mc_operator::ctor<op_addr_indirect_y, 2, &instruction::ins_sta>(),
    
    [0x86] = mc_operator::ctor<op_addr_zeropage,   2, &instruction::ins_stx>(),
    [0x96] = mc_operator::ctor<op_addr_zeropage_y, 2, &instruction::ins_stx>(),
    [0x8E] = mc_operator::ctor<op_addr_absolute,   3, &instruction::ins_stx>(),

    [0x84] = mc_operator::ctor<op_addr_zeropage,   2, &instruction::ins_sty>(),
    [0x94] = mc_operator::ctor<op_addr_zeropage_x, 2, &instruction::ins_sty>(),
    [0x8C] = mc_operator::ctor<op_addr_absolute,   3, &instruction::ins_sty>(),

    [0xAA] = mc_operator::ctor<op_addr_implied, 1, &instruction::ins_tax>(),
    
    [0xA8] = mc_operator::ctor<op_addr_implied, 1, &instruction::ins_tay>(),
    
    [0xBA] = mc_operator::ctor<op_addr_implied, 1, &instruction::ins_tsx>(),
    
    [0x8A] = mc_operator::ctor<op_addr_implied, 1, &instruction::ins_txa>(),

    [0x9A] = mc_operator::ctor<op_addr_implied, 1, &instruction::ins_txs>(),

    [0x98] = mc_operator::ctor<op_addr_implied, 1, &instruction::ins_tya>(),
};

}

#endif
