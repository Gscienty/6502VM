#include "instruction.hpp"
#include "signed.hpp"
#include "sr_flags.hpp"

namespace vm6502 {

instruction::instruction(uint8_t * stack, reg & r) : stack(stack), r(r) { }

void instruction::__sr_negative(uint8_t result)
{
    if (signed_operator<byte_meta>::is_negative(result)) {
        sr_set<sr_flags_negative>();
    }
    else {
        sr_clear<sr_flags_negative>();
    }
}

void instruction::__sr_zero(uint8_t result)
{
    if (result == 0x00) {
        sr_set<sr_flags_zero>();
    }
    else {
        sr_clear<sr_flags_zero>();
    }
}

void instruction::__sr_carry(uint8_t result, uint8_t a, uint8_t b)
{
    if (result < a && result < b) {
        sr_set<sr_flags_carry>();
    }
    else {
        sr_clear<sr_flags_carry>();
    }
}

void instruction::__sr_overflow(uint8_t result, uint8_t a, uint8_t b)
{
    if ((signed_operator<byte_meta>::is_negative(a)
         && signed_operator<byte_meta>::is_negative(b)
         && !signed_operator<byte_meta>::is_negative(result))
        ||
        (!signed_operator<byte_meta>::is_negative(a)
         && !signed_operator<byte_meta>::is_negative(b)
         && signed_operator<byte_meta>::is_negative(result))) {
        sr_set<sr_flags_overflow>();
    }
    else {
        sr_clear<sr_flags_overflow>();
    }
}

void instruction::__relative_branch(uint8_t m)
{
    this->r.pc &= 0xFF00;
    this->r.pc |= m;
}

void instruction::__push(uint8_t v)
{
    *(this->stack + this->r.sp) = v;
    this->r.sp--;
}

uint8_t instruction::__pull()
{
    this->r.sp--;
    return *(this->stack + this->r.sp);
}

void instruction::ins_adc(uint8_t m)
{
    uint8_t result = this->r.ac + m
        + (__sr_is_set<sr_flags_carry>() ? 1 : 0);

    __sr_negative(result);
    __sr_zero(result);
    __sr_carry(result, this->r.ac, m);
    __sr_overflow(result, this->r.ac, m);

    this->r.ac = result;
}

void instruction::ins_and(uint8_t m)
{
    uint8_t result = this->r.ac & m;

    __sr_negative(result);
    __sr_zero(result);

    this->r.ac = result;
}

void instruction::ins_asl(uint8_t & m)
{
    uint8_t result = m << 1;

    __sr_carry(result, m, m);
    __sr_negative(result);
    __sr_zero(result);

    m = result;
}

void instruction::ins_bit(uint8_t m)
{
    uint8_t result = this->r.ac & m;

    if ((m & 0x80) != 0) {
        sr_set<sr_flags_negative>();
    }
    else {
        sr_clear<sr_flags_negative>();
    }
    if ((m & 0x40) != 0) {
        sr_set<sr_flags_overflow>();
    }
    else {
        sr_clear<sr_flags_overflow>();
    }
    __sr_negative(result);
}

void instruction::ins_cmd(uint8_t m)
{
    uint8_t result = this->r.ac - m;

    __sr_negative(result);
    __sr_zero(result);
    __sr_carry(result, this->r.ac, ~m + 1);
}

void instruction::ins_cpx(uint8_t m)
{
    uint8_t result = this->r.x - m;

    __sr_negative(result);
    __sr_zero(result);
    __sr_carry(result, this->r.x, ~m + 1);
}

void instruction::ins_cpy(uint8_t m)
{
    uint8_t result = this->r.y - m;

    __sr_negative(result);
    __sr_zero(result);
    __sr_carry(result, this->r.y, ~m + 1);
}

void instruction::ins_dec(uint8_t & m)
{
    m--;

    __sr_negative(m);
    __sr_zero(m);
}

void instruction::ins_dex()
{
    this->r.x--;
    
    __sr_negative(this->r.x);
    __sr_zero(this->r.x);
}

void instruction::ins_dey()
{
    this->r.y--;
    
    __sr_negative(this->r.y);
    __sr_zero(this->r.y);
}

void instruction::ins_eor(uint8_t m)
{
    this->r.ac ^= m;

    __sr_negative(this->r.ac);
    __sr_zero(this->r.ac);
}

void instruction::ins_inc(uint8_t & m)
{
    m++;

    __sr_negative(m);
    __sr_zero(m);
}

void instruction::ins_inx()
{
    this->r.x++;

    __sr_negative(this->r.x);
    __sr_zero(this->r.x);
}

void instruction::ins_iny()
{
    this->r.y++;

    __sr_negative(this->r.y);
    __sr_zero(this->r.y);
}

void instruction::ins_jmp(uint16_t m)
{
    this->r.pc = m;
}

void instruction::ins_jsr(uint16_t m)
{
    __push(((this->r.pc + 2) & 0xFF00) >> 8);
    __push((this->r.pc + 2) & 0x00FF);

    this->r.pc = m;
}

void instruction::ins_lda(uint8_t m)
{
    this->r.ac = m;

    __sr_negative(m);
    __sr_zero(m);
}
void instruction::ins_ldx(uint8_t m)
{
    this->r.x = m;

    __sr_negative(m);
    __sr_zero(m);
}
void instruction::ins_ldy(uint8_t m)
{
    this->r.y = m;

    __sr_negative(m);
    __sr_zero(m);
}

void instruction::ins_lsr(uint8_t & m)
{
    uint8_t result = m >> 1;

    __sr_zero(result);
    if ((m & 0x01) != 0) {
        sr_set<sr_flags_carry>();
    }
    else {
        sr_clear<sr_flags_carry>();
    }

    m = result;
}

void instruction::ins_ora(uint8_t m)
{
    this->r.ac |= m;

    __sr_negative(this->r.ac);
    __sr_zero(this->r.ac);
}

void instruction::ins_pha()
{
    __push(this->r.ac);
}

void instruction::ins_php()
{
    __push(this->r.sr);
}

void instruction::ins_pla()
{
    this->r.ac = __pull();
}

void instruction::ins_plp()
{
    this->r.sr = __pull();
}

void instruction::ins_rol(uint8_t & m)
{
    uint8_t result = ((m & 0x80) >> 7) | (m << 1);

    if ((m & 0x80) != 0) {
        sr_set<sr_flags_carry>();
    }
    else {
        sr_clear<sr_flags_carry>();
    }
    __sr_negative(result);
    __sr_zero(result);

    m = result;
}

void instruction::ins_ror(uint8_t & m)
{
    uint8_t result = ((m & 0x01) << 7) | (m >> 1);

    if ((m & 0x01) != 0) {
        sr_set<sr_flags_carry>();
    }
    else {
        sr_clear<sr_flags_carry>();
    }
    __sr_negative(result);
    __sr_zero(result);

    m = result;
}

void instruction::ins_rti()
{
    this->r.sr = __pull();
    this->r.pc = 0x0000;
    this->r.pc = __pull();
    this->r.pc |= __pull() << 8;
}

void instruction::ins_rts()
{
    this->r.pc = 0x0000;
    this->r.pc = __pull();
    this->r.pc |= __pull() << 8;
    this->r.pc++;
}

void instruction::ins_sbc(uint8_t m)
{
    uint8_t result = this->r.ac + (~m + 1)
        - (__sr_is_set<sr_flags_carry>() ? 1 : 0);

    __sr_negative(result);
    __sr_zero(result);
    __sr_carry(result, this->r.ac, ~m + 1);
    __sr_overflow(result, this->r.ac, ~m + 1);

    this->r.ac = result;
}

void instruction::ins_sta(uint8_t & m)
{
    m = this->r.ac;
}

void instruction::ins_stx(uint8_t & m)
{
    m = this->r.x;
}

void instruction::ins_sty(uint8_t & m)
{
    m = this->r.y;
}

void instruction::ins_tax()
{
    this->r.x = this->r.ac;

    __sr_negative(this->r.ac);
    __sr_zero(this->r.ac);
}

void instruction::ins_tay()
{
    this->r.y = this->r.ac;

    __sr_negative(this->r.ac);
    __sr_zero(this->r.ac);
}

void instruction::ins_tsx()
{
    this->r.x = this->r.sp;

    __sr_negative(this->r.x);
    __sr_zero(this->r.x);
}

void instruction::ins_txa()
{
    this->r.ac = this->r.x;

    __sr_negative(this->r.ac);
    __sr_zero(this->r.ac);
}

void instruction::ins_txs()
{
    this->r.sp = this->r.x;

    __sr_negative(this->r.sp);
    __sr_zero(this->r.sp);
}

void instruction::ins_tya()
{
    this->r.ac = this->r.y;

    __sr_negative(this->r.ac);
    __sr_zero(this->r.ac);
}

}
