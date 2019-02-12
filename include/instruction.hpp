#ifndef _6502VM_INSTRUCTION_H
#define _6502VM_INSTRUCTION_H

#include "register.hpp"
#include "sr_flags.hpp"

namespace vm6502 {

class instruction {
private:
    uint8_t * stack;
    reg & r;

    template <sr_flags Flag>
    bool __sr_is_set()
    {
        return sr_operator<Flag>::is_set(this->r.sr);
    }

    template <sr_flags Flag>
    bool __sr_is_clear()
    {
        return sr_operator<Flag>::is_clear(this->r.sr);
    }

    void __sr_negative(uint8_t result);
    void __sr_zero(uint8_t result);
    void __sr_carry(uint8_t result, uint8_t a, uint8_t b);
    void __sr_overflow(uint8_t result, uint8_t a, uint8_t b);

    void __relative_branch(uint8_t m);

    void __push(uint8_t v);
    uint8_t __pull();

public:
    instruction(uint8_t * stack, reg & r);

    void ins_adc(uint8_t m);
    void ins_and(uint8_t m);
    void ins_asl(uint8_t & m);
    void ins_bit(uint8_t m);

    template <sr_flags Flag, uint8_t Operator>
    void branch(uint8_t m)
    {
        if (Operator != 0 && __sr_is_set<Flag>()) {
            __relative_branch(m);
            return;
        }

        if (Operator == 0 && __sr_is_clear<Flag>()) {
            __relative_branch(m);
            return;
        }
    }

    template <sr_flags Flag>
    void sr_set()
    {
        sr_operator<Flag>::set(this->r.sr);
    }

    template <sr_flags Flag>
    void sr_clear()
    {
        sr_operator<Flag>::clear(this->r.sr);
    }

    void ins_cmd(uint8_t m);
    void ins_cpx(uint8_t m);
    void ins_cpy(uint8_t m);
    void ins_dec(uint8_t & m);
    void ins_dex();
    void ins_dey();
    void ins_eor(uint8_t m);
    void ins_inc(uint8_t & m);
    void ins_inx();
    void ins_iny();
    void ins_jmp(uint16_t m);
    void ins_jsr(uint16_t m);
    void ins_lda(uint8_t m);
    void ins_ldx(uint8_t m);
    void ins_ldy(uint8_t m);
    void ins_lsr(uint8_t & m);
    void ins_ora(uint8_t m);
    void ins_pha();
    void ins_php();
    void ins_pla();
    void ins_plp();
    void ins_rol(uint8_t & m);
    void ins_ror(uint8_t & m);
    void ins_rti();
    void ins_rts();
    void ins_sbc(uint8_t m);
    void ins_sta(uint8_t & m);
    void ins_stx(uint8_t & m);
    void ins_sty(uint8_t & m);
    void ins_tax();
    void ins_tay();
    void ins_tsx();
    void ins_txa();
    void ins_txs();
    void ins_tya();
};

}

#endif
