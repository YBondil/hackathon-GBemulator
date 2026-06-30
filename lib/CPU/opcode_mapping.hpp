#pragma once
#include "CPU.hpp"

const std::array<CPU::Op, 256> CPU::opcode_table = [] {
    std::array<CPU::Op, 256> t{};   // tout à nullptr = "non implémenté"

    // --- Contrôle ---
    t[0x00] = [](CPU& c){ (void)c; return Cycles(1); };                  // NOP

    // --- LD r,n8  (0x06/0x0E/0x16/0x1E/0x26/0x2E/0x3E) ---
    t[0x06] = [](CPU& c){ return c.opcode_ld_n8(c.fetch8(), c.B); };     // LD B,n8
    t[0x0E] = [](CPU& c){ return c.opcode_ld_n8(c.fetch8(), c.C); };     // LD C,n8
    t[0x16] = [](CPU& c){ return c.opcode_ld_n8(c.fetch8(), c.D); };     // LD D,n8
    t[0x1E] = [](CPU& c){ return c.opcode_ld_n8(c.fetch8(), c.E); };     // LD E,n8
    t[0x26] = [](CPU& c){ return c.opcode_ld_n8(c.fetch8(), c.H); };     // LD H,n8
    t[0x2E] = [](CPU& c){ return c.opcode_ld_n8(c.fetch8(), c.L); };     // LD L,n8
    t[0x3E] = [](CPU& c){ return c.opcode_ld_n8(c.fetch8(), c.A); };     // LD A,n8

    // --- LD rr,n16  (0x01/0x11/0x21/0x31) ---
    t[0x01] = [](CPU& c){ return c.opcode_ld_n16(c.fetch16(), c.BC); };  // LD BC,n16
    t[0x11] = [](CPU& c){ return c.opcode_ld_n16(c.fetch16(), c.DE); };  // LD DE,n16
    t[0x21] = [](CPU& c){ return c.opcode_ld_n16(c.fetch16(), c.HL); };  // LD HL,n16
    t[0x31] = [](CPU& c){ return c.opcode_ld_SP_n16(c.fetch16()); };     // LD SP,n16

    // --- LD (rr),A  et  LD A,(rr) ---
    t[0x02] = [](CPU& c){ return c.opcode_ld_A(c.BC); };                 // LD (BC),A
    t[0x12] = [](CPU& c){ return c.opcode_ld_A(c.DE); };                 // LD (DE),A
    t[0x0A] = [](CPU& c){ return c.opcode_ld_A_r16(c.BC); };             // LD A,(BC)
    t[0x1A] = [](CPU& c){ return c.opcode_ld_A_r16(c.DE); };             // LD A,(DE)

    // --- LD avec (HL+) / (HL-) ---
    t[0x22] = [](CPU& c){ return c.opcode_ld_HLI_A(); };                 // LD (HL+),A
    t[0x32] = [](CPU& c){ return c.opcode_ld_HLD_A(); };                 // LD (HL-),A
    t[0x2A] = [](CPU& c){ return c.opcode_ld_A_HLI(); };                 // LD A,(HL+)
    t[0x3A] = [](CPU& c){ return c.opcode_ld_A_HLD(); };                 // LD A,(HL-)

    // --- LD (HL),r  (0x70-0x77 sauf 0x76=HALT) ---
    t[0x70] = [](CPU& c){ return c.opcode_ld_HL(c.B); };                 // LD (HL),B
    t[0x71] = [](CPU& c){ return c.opcode_ld_HL(c.C); };                 // LD (HL),C
    t[0x72] = [](CPU& c){ return c.opcode_ld_HL(c.D); };                 // LD (HL),D
    t[0x73] = [](CPU& c){ return c.opcode_ld_HL(c.E); };                 // LD (HL),E
    t[0x74] = [](CPU& c){ return c.opcode_ld_HL(c.H); };                 // LD (HL),H
    t[0x75] = [](CPU& c){ return c.opcode_ld_HL(c.L); };                 // LD (HL),L
    t[0x77] = [](CPU& c){ return c.opcode_ld_HL(c.A); };                 // LD (HL),A

    // --- LD r,(HL)  (0x46/0x4E/0x56/0x5E/0x66/0x6E/0x7E) ---
    t[0x46] = [](CPU& c){ return c.opcode_ld_HL_r8(c.B); };              // LD B,(HL)
    t[0x4E] = [](CPU& c){ return c.opcode_ld_HL_r8(c.C); };              // LD C,(HL)
    t[0x7E] = [](CPU& c){ return c.opcode_ld_HL_r8(c.A); };              // LD A,(HL)
    // ... compléter 0x56/0x5E/0x66/0x6E sur le même modèle

    // --- Accès absolus / haute mémoire ---
    t[0xEA] = [](CPU& c){ return c.opcode_ld_n16_A(c.fetch16()); };      // LD (n16),A
    t[0xFA] = [](CPU& c){ return c.opcode_ld_A_n16(c.fetch16()); };      // LD A,(n16)
    t[0xE0] = [](CPU& c){ return c.opcode_ldh_n16_A(0xFF00 + c.fetch8()); }; // LDH (a8),A
    t[0xF0] = [](CPU& c){ return c.opcode_ldh_A_n16(0xFF00 + c.fetch8()); }; // LDH A,(a8)
    t[0xE2] = [](CPU& c){ return c.opcode_ldh_C_A(); };                  // LDH (C),A
    t[0xF2] = [](CPU& c){ return c.opcode_ldh_A_C(); };                  // LDH A,(C)
    t[0x08] = [](CPU& c){ return c.opcode_ld_n16_SP(c.fetch16()); };     // LD (n16),SP
    t[0xF8] = [](CPU& c){ return c.opcode_ld_HL_SP_s8(c.fetch_s8()); };  // LD HL,SP+s8

    // --- ADC A,r  (0x88-0x8F) ---
    t[0x88] = [](CPU& c){ return c._opcode_adc(c.B.value()); };          // ADC A,B
    t[0x89] = [](CPU& c){ return c._opcode_adc(c.C.value()); };          // ADC A,C
    t[0x8A] = [](CPU& c){ return c._opcode_adc(c.D.value()); };          // ADC A,D
    t[0x8B] = [](CPU& c){ return c._opcode_adc(c.E.value()); };          // ADC A,E
    t[0x8C] = [](CPU& c){ return c._opcode_adc(c.H.value()); };          // ADC A,H
    t[0x8D] = [](CPU& c){ return c._opcode_adc(c.L.value()); };          // ADC A,L
    t[0x8E] = [](CPU& c){ return c.opcode_adc_HL(); };                   // ADC A,(HL)
    t[0x8F] = [](CPU& c){ return c._opcode_adc(c.A.value()); };          // ADC A,A
    t[0xCE] = [](CPU& c){ return c.opcode_adc_n8(c.fetch8()); };         // ADC A,n8

    // --- ADD A,r  (0x80-0x87) ---  _opcode_add(reg, value) : A = reg + value
    t[0x80] = [](CPU& c){ return c._opcode_add(c.A.value(), c.B.value()); }; // ADD A,B
    t[0x81] = [](CPU& c){ return c._opcode_add(c.A.value(), c.C.value()); }; // ADD A,C
    t[0x82] = [](CPU& c){ return c._opcode_add(c.A.value(), c.D.value()); }; // ADD A,D
    t[0x83] = [](CPU& c){ return c._opcode_add(c.A.value(), c.E.value()); }; // ADD A,E
    t[0x84] = [](CPU& c){ return c._opcode_add(c.A.value(), c.H.value()); }; // ADD A,H
    t[0x85] = [](CPU& c){ return c._opcode_add(c.A.value(), c.L.value()); }; // ADD A,L
    t[0x86] = [](CPU& c){ return c.opcode_add_A_HL(); };                     // ADD A,(HL)
    t[0x87] = [](CPU& c){ return c._opcode_add(c.A.value(), c.A.value()); }; // ADD A,A
    // t[0xC6] ADD A,n8 : à revoir (l'ordre des args de opcode_add_n8 est buggé)

    // TODO :
    //   - bloc LD r,r' (0x40-0x7F) via _opcode_ld (après correction du
    //     passage par valeur des Register)
    //   - SUB/AND/OR/XOR/CP/INC/DEC, JP/JR/CALL/RET/PUSH/POP, EI/DI/RETI...

    return t;
}();


const std::array<CPU::Op, 256> CPU::cb_table = [] {
    std::array<CPU::Op, 256> t{};   // toutes les instructions préfixées 0xCB
    // TODO : RLC/RRC/RL/RR/SLA/SRA/SWAP/SRL (0x00-0x3F),
    //        BIT (0x40-0x7F), RES (0x80-0xBF), SET (0xC0-0xFF)
    return t;
}();
