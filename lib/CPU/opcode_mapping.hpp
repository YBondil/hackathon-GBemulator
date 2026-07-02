#pragma once

#include "CPU.hpp"

const std::array<CPU::Op, 256> CPU::opcode_table = [] {
    std::array<CPU::Op, 256> t{};
    using CC = CPU::Condition_code;

    // ---------- 0x00-0x3F : divers / LD / INC / DEC / rotations ----------
    t[0x00] = [](CPU& c){ return c.opcode_nop(); };                              // NOP
    t[0x01] = [](CPU& c){ return c.opcode_ld_r16_n16(c.fetch16(), c.BC); };          // LD BC,n16
    t[0x02] = [](CPU& c){ return c.opcode_ld_r16_A(c.BC); };                         // LD (BC),A
    t[0x03] = [](CPU& c){ return c.opcode_inc_r16(c.BC); };                      // INC BC
    t[0x04] = [](CPU& c){ return c.opcode_inc_r8(c.B); };                        // INC B
    t[0x05] = [](CPU& c){ return c.opcode_dec_r8(c.B); };                        // DEC B
    t[0x06] = [](CPU& c){ return c.opcode_ld_r8_n8(c.fetch8(), c.B); };             // LD B,n8
    t[0x07] = [](CPU& c){ return c.opcode_rlc_A(); };                            // RLCA
    t[0x08] = [](CPU& c){ return c.opcode_ld_n16_SP(c.fetch16()); };             // LD (n16),SP
    t[0x0A] = [](CPU& c){ return c.opcode_ld_A_r16(c.BC); };                     // LD A,(BC)
    t[0x0C] = [](CPU& c){ return c.opcode_inc_r8(c.C); };                        // INC C
    t[0x0D] = [](CPU& c){ return c.opcode_dec_r8(c.C); };                        // DEC C
    t[0x0E] = [](CPU& c){ return c.opcode_ld_r8_n8(c.fetch8(), c.C); };             // LD C,n8
    t[0x0F] = [](CPU& c){ return c.opcode_rrc_A(); };                            // RRCA

    t[0x11] = [](CPU& c){ return c.opcode_ld_r16_n16(c.fetch16(), c.DE); };          // LD DE,n16
    t[0x12] = [](CPU& c){ return c.opcode_ld_r16_A(c.DE); };                         // LD (DE),A
    t[0x13] = [](CPU& c){ return c.opcode_inc_r16(c.DE); };                      // INC DE
    t[0x14] = [](CPU& c){ return c.opcode_inc_r8(c.D); };                        // INC D
    t[0x15] = [](CPU& c){ return c.opcode_dec_r8(c.D); };                        // DEC D
    t[0x16] = [](CPU& c){ return c.opcode_ld_r8_n8(c.fetch8(), c.D); };             // LD D,n8
    t[0x17] = [](CPU& c){ return c.opcode_rl_A(); };                             // RLA
    t[0x1A] = [](CPU& c){ return c.opcode_ld_A_r16(c.DE); };                     // LD A,(DE)
    t[0x1C] = [](CPU& c){ return c.opcode_inc_r8(c.E); };                        // INC E
    t[0x1D] = [](CPU& c){ return c.opcode_dec_r8(c.E); };                        // DEC E
    t[0x1E] = [](CPU& c){ return c.opcode_ld_r8_n8(c.fetch8(), c.E); };             // LD E,n8
    t[0x1F] = [](CPU& c){ return c.opcode_rr_A(); };                             // RRA

    t[0x21] = [](CPU& c){ return c.opcode_ld_r16_n16(c.fetch16(), c.HL); };          // LD HL,n16
    t[0x22] = [](CPU& c){ return c.opcode_ld_HLI_A(); };                         // LD (HL+),A
    t[0x23] = [](CPU& c){ return c.opcode_inc_r16(c.HL); };                      // INC HL
    t[0x24] = [](CPU& c){ return c.opcode_inc_r8(c.H); };                        // INC H
    t[0x25] = [](CPU& c){ return c.opcode_dec_r8(c.H); };                        // DEC H
    t[0x26] = [](CPU& c){ return c.opcode_ld_r8_n8(c.fetch8(), c.H); };             // LD H,n8
    t[0x27] = [](CPU& c){ return c.opcode_daa(); };                             // DAA
    t[0x2A] = [](CPU& c){ return c.opcode_ld_A_HLI(); };                         // LD A,(HL+)
    t[0x2C] = [](CPU& c){ return c.opcode_inc_r8(c.L); };                        // INC L
    t[0x2D] = [](CPU& c){ return c.opcode_dec_r8(c.L); };                        // DEC L
    t[0x2E] = [](CPU& c){ return c.opcode_ld_r8_n8(c.fetch8(), c.L); };             // LD L,n8
    t[0x2F] = [](CPU& c){ return c.opcode_cpl(); };                             // CPL

    t[0x31] = [](CPU& c){ return c.opcode_ld_SP_n16(c.fetch16()); };            // LD SP,n16
    t[0x32] = [](CPU& c){ return c.opcode_ld_HLD_A(); };                         // LD (HL-),A
    t[0x33] = [](CPU& c){ return c.opcode_inc_SP(); };                          // INC SP  (buggé)
    t[0x34] = [](CPU& c){ return c.opcode_inc_HL(); };                          // INC (HL)
    t[0x35] = [](CPU& c){ return c.opcode_dec_hl(); };                          // DEC (HL)
    t[0x37] = [](CPU& c){ return c.opcode_scf(); };                            // SCF
    t[0x39] = [](CPU& c){ return c.opcode_add_HL_SP(); };                       // ADD HL,SP (buggé)
    t[0x3A] = [](CPU& c){ return c.opcode_ld_A_HLD(); };                         // LD A,(HL-)
    t[0x3B] = [](CPU& c){ return c.opcode_dec_SP(); };                          // DEC SP  (buggé)
    t[0x3C] = [](CPU& c){ return c.opcode_inc_r8(c.A); };                        // INC A
    t[0x3D] = [](CPU& c){ return c.opcode_dec_r8(c.A); };                        // DEC A
    t[0x3E] = [](CPU& c){ return c.opcode_ld_r8_n8(c.fetch8(), c.A); };             // LD A,n8
    t[0x3F] = [](CPU& c){ return c.opcode_ccf(); };                            // CCF

    // ---------- 0x40-0x7F : LD r,r'  ( opcode_ld_r8_r8(src, dest) ) ----------
    // dest B
    t[0x40]=[](CPU&c){return c.opcode_ld_r8_r8(c.B,c.B);}; t[0x41]=[](CPU&c){return c.opcode_ld_r8_r8(c.C,c.B);};
    t[0x42]=[](CPU&c){return c.opcode_ld_r8_r8(c.D,c.B);}; t[0x43]=[](CPU&c){return c.opcode_ld_r8_r8(c.E,c.B);};
    t[0x44]=[](CPU&c){return c.opcode_ld_r8_r8(c.H,c.B);}; t[0x45]=[](CPU&c){return c.opcode_ld_r8_r8(c.L,c.B);};
    t[0x46]=[](CPU&c){return c.opcode_ld_r8_HL(c.B);}; t[0x47]=[](CPU&c){return c.opcode_ld_r8_r8(c.A,c.B);};
    // dest C
    t[0x48]=[](CPU&c){return c.opcode_ld_r8_r8(c.B,c.C);}; t[0x49]=[](CPU&c){return c.opcode_ld_r8_r8(c.C,c.C);};
    t[0x4A]=[](CPU&c){return c.opcode_ld_r8_r8(c.D,c.C);}; t[0x4B]=[](CPU&c){return c.opcode_ld_r8_r8(c.E,c.C);};
    t[0x4C]=[](CPU&c){return c.opcode_ld_r8_r8(c.H,c.C);}; t[0x4D]=[](CPU&c){return c.opcode_ld_r8_r8(c.L,c.C);};
    t[0x4E]=[](CPU&c){return c.opcode_ld_r8_HL(c.C);}; t[0x4F]=[](CPU&c){return c.opcode_ld_r8_r8(c.A,c.C);};
    // dest D
    t[0x50]=[](CPU&c){return c.opcode_ld_r8_r8(c.B,c.D);}; t[0x51]=[](CPU&c){return c.opcode_ld_r8_r8(c.C,c.D);};
    t[0x52]=[](CPU&c){return c.opcode_ld_r8_r8(c.D,c.D);}; t[0x53]=[](CPU&c){return c.opcode_ld_r8_r8(c.E,c.D);};
    t[0x54]=[](CPU&c){return c.opcode_ld_r8_r8(c.H,c.D);}; t[0x55]=[](CPU&c){return c.opcode_ld_r8_r8(c.L,c.D);};
    t[0x56]=[](CPU&c){return c.opcode_ld_r8_HL(c.D);}; t[0x57]=[](CPU&c){return c.opcode_ld_r8_r8(c.A,c.D);};
    // dest E
    t[0x58]=[](CPU&c){return c.opcode_ld_r8_r8(c.B,c.E);}; t[0x59]=[](CPU&c){return c.opcode_ld_r8_r8(c.C,c.E);};
    t[0x5A]=[](CPU&c){return c.opcode_ld_r8_r8(c.D,c.E);}; t[0x5B]=[](CPU&c){return c.opcode_ld_r8_r8(c.E,c.E);};
    t[0x5C]=[](CPU&c){return c.opcode_ld_r8_r8(c.H,c.E);}; t[0x5D]=[](CPU&c){return c.opcode_ld_r8_r8(c.L,c.E);};
    t[0x5E]=[](CPU&c){return c.opcode_ld_r8_HL(c.E);}; t[0x5F]=[](CPU&c){return c.opcode_ld_r8_r8(c.A,c.E);};
    // dest H
    t[0x60]=[](CPU&c){return c.opcode_ld_r8_r8(c.B,c.H);}; t[0x61]=[](CPU&c){return c.opcode_ld_r8_r8(c.C,c.H);};
    t[0x62]=[](CPU&c){return c.opcode_ld_r8_r8(c.D,c.H);}; t[0x63]=[](CPU&c){return c.opcode_ld_r8_r8(c.E,c.H);};
    t[0x64]=[](CPU&c){return c.opcode_ld_r8_r8(c.H,c.H);}; t[0x65]=[](CPU&c){return c.opcode_ld_r8_r8(c.L,c.H);};
    t[0x66]=[](CPU&c){return c.opcode_ld_r8_HL(c.H);}; t[0x67]=[](CPU&c){return c.opcode_ld_r8_r8(c.A,c.H);};
    // dest L
    t[0x68]=[](CPU&c){return c.opcode_ld_r8_r8(c.B,c.L);}; t[0x69]=[](CPU&c){return c.opcode_ld_r8_r8(c.C,c.L);};
    t[0x6A]=[](CPU&c){return c.opcode_ld_r8_r8(c.D,c.L);}; t[0x6B]=[](CPU&c){return c.opcode_ld_r8_r8(c.E,c.L);};
    t[0x6C]=[](CPU&c){return c.opcode_ld_r8_r8(c.H,c.L);}; t[0x6D]=[](CPU&c){return c.opcode_ld_r8_r8(c.L,c.L);};
    t[0x6E]=[](CPU&c){return c.opcode_ld_r8_HL(c.L);}; t[0x6F]=[](CPU&c){return c.opcode_ld_r8_r8(c.A,c.L);};
    // dest (HL)   (0x76 = HALT, non implémenté)
    t[0x70]=[](CPU&c){return c.opcode_ld_HL_r8(c.B);}; t[0x71]=[](CPU&c){return c.opcode_ld_HL_r8(c.C);};
    t[0x72]=[](CPU&c){return c.opcode_ld_HL_r8(c.D);}; t[0x73]=[](CPU&c){return c.opcode_ld_HL_r8(c.E);};
    t[0x74]=[](CPU&c){return c.opcode_ld_HL_r8(c.H);}; t[0x75]=[](CPU&c){return c.opcode_ld_HL_r8(c.L);};
    t[0x77]=[](CPU&c){return c.opcode_ld_HL_r8(c.A);};
    // dest A
    t[0x78]=[](CPU&c){return c.opcode_ld_r8_r8(c.B,c.A);}; t[0x79]=[](CPU&c){return c.opcode_ld_r8_r8(c.C,c.A);};
    t[0x7A]=[](CPU&c){return c.opcode_ld_r8_r8(c.D,c.A);}; t[0x7B]=[](CPU&c){return c.opcode_ld_r8_r8(c.E,c.A);};
    t[0x7C]=[](CPU&c){return c.opcode_ld_r8_r8(c.H,c.A);}; t[0x7D]=[](CPU&c){return c.opcode_ld_r8_r8(c.L,c.A);};
    t[0x7E]=[](CPU&c){return c.opcode_ld_r8_HL(c.A);}; t[0x7F]=[](CPU&c){return c.opcode_ld_r8_r8(c.A,c.A);};

    // ---------- 0x80-0xBF : ALU A,r8 ----------
    // ADD A,r  (_opcode_add(A, r))
    t[0x80]=[](CPU&c){return c._opcode_add(c.A.value(),c.B.value());};
    t[0x81]=[](CPU&c){return c._opcode_add(c.A.value(),c.C.value());};
    t[0x82]=[](CPU&c){return c._opcode_add(c.A.value(),c.D.value());};
    t[0x83]=[](CPU&c){return c._opcode_add(c.A.value(),c.E.value());};
    t[0x84]=[](CPU&c){return c._opcode_add(c.A.value(),c.H.value());};
    t[0x85]=[](CPU&c){return c._opcode_add(c.A.value(),c.L.value());};
    t[0x86]=[](CPU&c){return c.opcode_add_A_HL();};
    t[0x87]=[](CPU&c){return c._opcode_add(c.A.value(),c.A.value());};
    // ADC A,r  (_opcode_adc(r))
    t[0x88]=[](CPU&c){return c._opcode_adc(c.B.value());}; t[0x89]=[](CPU&c){return c._opcode_adc(c.C.value());};
    t[0x8A]=[](CPU&c){return c._opcode_adc(c.D.value());}; t[0x8B]=[](CPU&c){return c._opcode_adc(c.E.value());};
    t[0x8C]=[](CPU&c){return c._opcode_adc(c.H.value());}; t[0x8D]=[](CPU&c){return c._opcode_adc(c.L.value());};
    t[0x8E]=[](CPU&c){return c.opcode_adc_HL();};          t[0x8F]=[](CPU&c){return c._opcode_adc(c.A.value());};
    // SUB A,r
    t[0x90]=[](CPU&c){return c.opcode_sub_a_r8(c.B);}; t[0x91]=[](CPU&c){return c.opcode_sub_a_r8(c.C);};
    t[0x92]=[](CPU&c){return c.opcode_sub_a_r8(c.D);}; t[0x93]=[](CPU&c){return c.opcode_sub_a_r8(c.E);};
    t[0x94]=[](CPU&c){return c.opcode_sub_a_r8(c.H);}; t[0x95]=[](CPU&c){return c.opcode_sub_a_r8(c.L);};
    t[0x96]=[](CPU&c){return c.opcode_sub_a_hl();};    t[0x97]=[](CPU&c){return c.opcode_sub_a_r8(c.A);};
    // SBC A,r
    t[0x98]=[](CPU&c){return c.opcode_sbc_a_r8(c.B);}; t[0x99]=[](CPU&c){return c.opcode_sbc_a_r8(c.C);};
    t[0x9A]=[](CPU&c){return c.opcode_sbc_a_r8(c.D);}; t[0x9B]=[](CPU&c){return c.opcode_sbc_a_r8(c.E);};
    t[0x9C]=[](CPU&c){return c.opcode_sbc_a_r8(c.H);}; t[0x9D]=[](CPU&c){return c.opcode_sbc_a_r8(c.L);};
    t[0x9E]=[](CPU&c){return c.opcode_sbc_a_hl();};    t[0x9F]=[](CPU&c){return c.opcode_sbc_a_r8(c.A);};
    // AND A,r
    t[0xA0]=[](CPU&c){return c.opcode_and_A_r8(c.B);}; t[0xA1]=[](CPU&c){return c.opcode_and_A_r8(c.C);};
    t[0xA2]=[](CPU&c){return c.opcode_and_A_r8(c.D);}; t[0xA3]=[](CPU&c){return c.opcode_and_A_r8(c.E);};
    t[0xA4]=[](CPU&c){return c.opcode_and_A_r8(c.H);}; t[0xA5]=[](CPU&c){return c.opcode_and_A_r8(c.L);};
    t[0xA6]=[](CPU&c){return c.opcode_and_A_HL();};    t[0xA7]=[](CPU&c){return c.opcode_and_A_r8(c.A);};
    // XOR A,r
    t[0xA8]=[](CPU&c){return c.opcode_xor_A_r8(c.B);}; t[0xA9]=[](CPU&c){return c.opcode_xor_A_r8(c.C);};
    t[0xAA]=[](CPU&c){return c.opcode_xor_A_r8(c.D);}; t[0xAB]=[](CPU&c){return c.opcode_xor_A_r8(c.E);};
    t[0xAC]=[](CPU&c){return c.opcode_xor_A_r8(c.H);}; t[0xAD]=[](CPU&c){return c.opcode_xor_A_r8(c.L);};
    t[0xAE]=[](CPU&c){return c.opcode_xor_A_HL();};    t[0xAF]=[](CPU&c){return c.opcode_xor_A_r8(c.A);};
    // OR A,r
    t[0xB0]=[](CPU&c){return c.opcode_or_A_r8(c.B);}; t[0xB1]=[](CPU&c){return c.opcode_or_A_r8(c.C);};
    t[0xB2]=[](CPU&c){return c.opcode_or_A_r8(c.D);}; t[0xB3]=[](CPU&c){return c.opcode_or_A_r8(c.E);};
    t[0xB4]=[](CPU&c){return c.opcode_or_A_r8(c.H);}; t[0xB5]=[](CPU&c){return c.opcode_or_A_r8(c.L);};
    t[0xB6]=[](CPU&c){return c.opcode_or_A_HL();};    t[0xB7]=[](CPU&c){return c.opcode_or_A_r8(c.A);};
    // CP A,r
    t[0xB8]=[](CPU&c){return c.opcode_cp_a_r8(c.B);}; t[0xB9]=[](CPU&c){return c.opcode_cp_a_r8(c.C);};
    t[0xBA]=[](CPU&c){return c.opcode_cp_a_r8(c.D);}; t[0xBB]=[](CPU&c){return c.opcode_cp_a_r8(c.E);};
    t[0xBC]=[](CPU&c){return c.opcode_cp_a_r8(c.H);}; t[0xBD]=[](CPU&c){return c.opcode_cp_a_r8(c.L);};
    t[0xBE]=[](CPU&c){return c.opcode_cp_a_hl();};    t[0xBF]=[](CPU&c){return c.opcode_cp_a_r8(c.A);};

    // ---------- 0xC0-0xFF ----------
    t[0xC0]=[](CPU&c){return c.opcode_ret_cc(CC::NZ);};                          // RET NZ
    t[0xC1]=[](CPU&c){return c.opcode_pop_r16(c.BC);};                          // POP BC
    t[0xC4]=[](CPU&c){return c.opcode_call_cc_n16(c.fetch16(),CC::NZ);};            // CALL NZ,n16
    t[0xC5]=[](CPU&c){return c.opcode_push_r16(c.BC);};                         // PUSH BC
    t[0xC6]=[](CPU&c){return c.opcode_add_A_n8(c.A.value(),c.fetch8());};       // ADD A,n8
    t[0xC7]=[](CPU&c){return c.opcode_rst_vec(RST::rst1);};                     // RST 00
    t[0xC8]=[](CPU&c){return c.opcode_ret_cc(CC::Z);};                          // RET Z
    t[0xC9]=[](CPU&c){return c.opcode_ret();};                                  // RET
    t[0xCC]=[](CPU&c){return c.opcode_call_cc_n16(c.fetch16(),CC::Z);};             // CALL Z,n16
    t[0xCD]=[](CPU&c){return c.opcode_call_n16(c.fetch16());};                      // CALL n16
    t[0xCE]=[](CPU&c){return c.opcode_adc_n8(c.fetch8());};                     // ADC A,n8
    t[0xCF]=[](CPU&c){return c.opcode_rst_vec(RST::rst2);};                     // RST 08

    t[0xD0]=[](CPU&c){return c.opcode_ret_cc(CC::NC);};                         // RET NC
    t[0xD1]=[](CPU&c){return c.opcode_pop_r16(c.DE);};                          // POP DE
    t[0xD4]=[](CPU&c){return c.opcode_call_cc_n16(c.fetch16(),CC::NC);};            // CALL NC,n16
    t[0xD5]=[](CPU&c){return c.opcode_push_r16(c.DE);};                         // PUSH DE
    t[0xD6]=[](CPU&c){return c.opcode_sub_a_n8(c.fetch8());};                   // SUB A,n8
    t[0xD7]=[](CPU&c){return c.opcode_rst_vec(RST::rst3);};                     // RST 10
    t[0xD8]=[](CPU&c){return c.opcode_ret_cc(CC::C);};                          // RET C
    t[0xD9]=[](CPU&c){return c.opcode_reti();};                                 // RETI
    t[0xDC]=[](CPU&c){return c.opcode_call_cc_n16(c.fetch16(),CC::C);};             // CALL C,n16
    t[0xDE]=[](CPU&c){return c.opcode_sbc_a_n8(c.fetch8());};                   // SBC A,n8
    t[0xDF]=[](CPU&c){return c.opcode_rst_vec(RST::rst4);};                     // RST 18

    t[0xE0]=[](CPU&c){return c.opcode_ldh_n16_A(0xFF00 + c.fetch8());};         // LDH (a8),A
    t[0xE1]=[](CPU&c){return c.opcode_pop_r16(c.HL);};                          // POP HL
    t[0xE2]=[](CPU&c){return c.opcode_ldh_C_A();};                             // LDH (C),A
    t[0xE5]=[](CPU&c){return c.opcode_push_r16(c.HL);};                         // PUSH HL
    t[0xE6]=[](CPU&c){return c.opcode_and_A_n8(c.fetch8());};                   // AND A,n8
    t[0xE7]=[](CPU&c){return c.opcode_rst_vec(RST::rst5);};                     // RST 20
    t[0xE8]=[](CPU&c){return c.opcode_add_SP_s8(c.fetch_s8());};                // ADD SP,e8 (buggé)
    t[0xEA]=[](CPU&c){return c.opcode_ld_n16_A(c.fetch16());};                  // LD (n16),A
    t[0xEE]=[](CPU&c){return c.opcode_xor_A_n8(c.fetch8());};                   // XOR A,n8
    t[0xEF]=[](CPU&c){return c.opcode_rst_vec(RST::rst6);};                     // RST 28

    t[0xF0]=[](CPU&c){return c.opcode_ldh_A_n16(0xFF00 + c.fetch8());};         // LDH A,(a8) (buggé)
    t[0xF1]=[](CPU&c){return c.opcode_pop_AF();};                              // POP AF
    t[0xF2]=[](CPU&c){return c.opcode_ldh_A_C();};                             // LDH A,(C)
    t[0xF5]=[](CPU&c){return c.opcode_push_AF();};                             // PUSH AF
    t[0xF6]=[](CPU&c){return c.opcode_or_A_n8(c.fetch8());};                    // OR A,n8
    t[0xF7]=[](CPU&c){return c.opcode_rst_vec(RST::rst7);};                     // RST 30
    t[0xF8]=[](CPU&c){return c.opcode_ld_HL_SP_s8(c.fetch_s8());};              // LD HL,SP+e8 (buggé)
    t[0xF9]=[](CPU&c){return c.opcode_ld_SP_HL();};                            // LD SP,HL
    t[0xFA]=[](CPU&c){return c.opcode_ld_A_n16(c.fetch16());};                  // LD A,(n16)
    t[0xFB]=[](CPU&c){return c.opcode_ei();};                                  // EI
    t[0xFE]=[](CPU&c){return c.opcode_cp_a_n8(c.fetch8());};                    // CP A,n8
    t[0xFF]=[](CPU&c){return c.opcode_rst_vec(RST::rst8);};                     // RST 38

    // NON MAPPÉS (non implémentés) : JP/JR (0x18,0x20,0x28,0x30,0xC2,0xC3,0xCA,
    // 0xD2,0xDA,0xE9), DI (0xF3), HALT (0x76), STOP (0x10), DEC r16 (0x0B/1B/2B),
    // ADD HL,BC/DE/HL (0x09/19/29), LD (HL),n8 (0x36).

    return t;
}();


const std::array<CPU::Op, 256> CPU::cb_table = [] {
    std::array<CPU::Op, 256> t{};

    // RLC (0x00-0x07)
    t[0x00]=[](CPU&c){return c.opcode_rlc_r8(c.B);}; t[0x01]=[](CPU&c){return c.opcode_rlc_r8(c.C);};
    t[0x02]=[](CPU&c){return c.opcode_rlc_r8(c.D);}; t[0x03]=[](CPU&c){return c.opcode_rlc_r8(c.E);};
    t[0x04]=[](CPU&c){return c.opcode_rlc_r8(c.H);}; t[0x05]=[](CPU&c){return c.opcode_rlc_r8(c.L);};
    t[0x06]=[](CPU&c){return c.opcode_rlc_HL();};    t[0x07]=[](CPU&c){return c.opcode_rlc_r8(c.A);};
    // RRC (0x08-0x0F)
    t[0x08]=[](CPU&c){return c.opcode_rrc_r8(c.B);}; t[0x09]=[](CPU&c){return c.opcode_rrc_r8(c.C);};
    t[0x0A]=[](CPU&c){return c.opcode_rrc_r8(c.D);}; t[0x0B]=[](CPU&c){return c.opcode_rrc_r8(c.E);};
    t[0x0C]=[](CPU&c){return c.opcode_rrc_r8(c.H);}; t[0x0D]=[](CPU&c){return c.opcode_rrc_r8(c.L);};
    t[0x0E]=[](CPU&c){return c.opcode_rrc_HL();};    t[0x0F]=[](CPU&c){return c.opcode_rrc_r8(c.A);};
    // RL (0x10-0x17)
    t[0x10]=[](CPU&c){return c.opcode_rl_r8(c.B);}; t[0x11]=[](CPU&c){return c.opcode_rl_r8(c.C);};
    t[0x12]=[](CPU&c){return c.opcode_rl_r8(c.D);}; t[0x13]=[](CPU&c){return c.opcode_rl_r8(c.E);};
    t[0x14]=[](CPU&c){return c.opcode_rl_r8(c.H);}; t[0x15]=[](CPU&c){return c.opcode_rl_r8(c.L);};
    t[0x16]=[](CPU&c){return c.opcode_rl_HL();};    t[0x17]=[](CPU&c){return c.opcode_rl_r8(c.A);};
    // RR (0x18-0x1F)
    t[0x18]=[](CPU&c){return c.opcode_rr_r8(c.B);}; t[0x19]=[](CPU&c){return c.opcode_rr_r8(c.C);};
    t[0x1A]=[](CPU&c){return c.opcode_rr_r8(c.D);}; t[0x1B]=[](CPU&c){return c.opcode_rr_r8(c.E);};
    t[0x1C]=[](CPU&c){return c.opcode_rr_r8(c.H);}; t[0x1D]=[](CPU&c){return c.opcode_rr_r8(c.L);};
    t[0x1E]=[](CPU&c){return c.opcode_rr_HL();};    t[0x1F]=[](CPU&c){return c.opcode_rr_r8(c.A);};
    // SLA (0x20-0x27)
    t[0x20]=[](CPU&c){return c.opcode_sla_r8(c.B);}; t[0x21]=[](CPU&c){return c.opcode_sla_r8(c.C);};
    t[0x22]=[](CPU&c){return c.opcode_sla_r8(c.D);}; t[0x23]=[](CPU&c){return c.opcode_sla_r8(c.E);};
    t[0x24]=[](CPU&c){return c.opcode_sla_r8(c.H);}; t[0x25]=[](CPU&c){return c.opcode_sla_r8(c.L);};
    t[0x26]=[](CPU&c){return c.opcode_sla_HL();};    t[0x27]=[](CPU&c){return c.opcode_sla_r8(c.A);};
    // SRA (0x28-0x2F)
    t[0x28]=[](CPU&c){return c.opcode_sra_r8(c.B);}; t[0x29]=[](CPU&c){return c.opcode_sra_r8(c.C);};
    t[0x2A]=[](CPU&c){return c.opcode_sra_r8(c.D);}; t[0x2B]=[](CPU&c){return c.opcode_sra_r8(c.E);};
    t[0x2C]=[](CPU&c){return c.opcode_sra_r8(c.H);}; t[0x2D]=[](CPU&c){return c.opcode_sra_r8(c.L);};
    t[0x2E]=[](CPU&c){return c.opcode_sra_HL();};    t[0x2F]=[](CPU&c){return c.opcode_sra_r8(c.A);};
    // SWAP (0x30-0x37) : NON IMPLÉMENTÉ -> nullptr
    // SRL (0x38-0x3F)
    t[0x38]=[](CPU&c){return c.opcode_srl_r8(c.B);}; t[0x39]=[](CPU&c){return c.opcode_srl_r8(c.C);};
    t[0x3A]=[](CPU&c){return c.opcode_srl_r8(c.D);}; t[0x3B]=[](CPU&c){return c.opcode_srl_r8(c.E);};
    t[0x3C]=[](CPU&c){return c.opcode_srl_r8(c.H);}; t[0x3D]=[](CPU&c){return c.opcode_srl_r8(c.L);};
    t[0x3E]=[](CPU&c){return c.opcode_srl_HL();};    t[0x3F]=[](CPU&c){return c.opcode_srl_r8(c.A);};

    // BIT/RES/SET u3,r8 : bit = (op>>3)&7, registre = op&7 (B,C,D,E,H,L,(HL),A)
    // BIT (0x40-0x7F)
    t[0x40]=[](CPU&c){return c.opcode_bit_u3_r8(0,c.B);}; t[0x41]=[](CPU&c){return c.opcode_bit_u3_r8(0,c.C);};
    t[0x42]=[](CPU&c){return c.opcode_bit_u3_r8(0,c.D);}; t[0x43]=[](CPU&c){return c.opcode_bit_u3_r8(0,c.E);};
    t[0x44]=[](CPU&c){return c.opcode_bit_u3_r8(0,c.H);}; t[0x45]=[](CPU&c){return c.opcode_bit_u3_r8(0,c.L);};
    t[0x46]=[](CPU&c){return c.opcode_bit_u3_HL(0);};     t[0x47]=[](CPU&c){return c.opcode_bit_u3_r8(0,c.A);};
    t[0x48]=[](CPU&c){return c.opcode_bit_u3_r8(1,c.B);}; t[0x49]=[](CPU&c){return c.opcode_bit_u3_r8(1,c.C);};
    t[0x4A]=[](CPU&c){return c.opcode_bit_u3_r8(1,c.D);}; t[0x4B]=[](CPU&c){return c.opcode_bit_u3_r8(1,c.E);};
    t[0x4C]=[](CPU&c){return c.opcode_bit_u3_r8(1,c.H);}; t[0x4D]=[](CPU&c){return c.opcode_bit_u3_r8(1,c.L);};
    t[0x4E]=[](CPU&c){return c.opcode_bit_u3_HL(1);};     t[0x4F]=[](CPU&c){return c.opcode_bit_u3_r8(1,c.A);};
    t[0x50]=[](CPU&c){return c.opcode_bit_u3_r8(2,c.B);}; t[0x51]=[](CPU&c){return c.opcode_bit_u3_r8(2,c.C);};
    t[0x52]=[](CPU&c){return c.opcode_bit_u3_r8(2,c.D);}; t[0x53]=[](CPU&c){return c.opcode_bit_u3_r8(2,c.E);};
    t[0x54]=[](CPU&c){return c.opcode_bit_u3_r8(2,c.H);}; t[0x55]=[](CPU&c){return c.opcode_bit_u3_r8(2,c.L);};
    t[0x56]=[](CPU&c){return c.opcode_bit_u3_HL(2);};     t[0x57]=[](CPU&c){return c.opcode_bit_u3_r8(2,c.A);};
    t[0x58]=[](CPU&c){return c.opcode_bit_u3_r8(3,c.B);}; t[0x59]=[](CPU&c){return c.opcode_bit_u3_r8(3,c.C);};
    t[0x5A]=[](CPU&c){return c.opcode_bit_u3_r8(3,c.D);}; t[0x5B]=[](CPU&c){return c.opcode_bit_u3_r8(3,c.E);};
    t[0x5C]=[](CPU&c){return c.opcode_bit_u3_r8(3,c.H);}; t[0x5D]=[](CPU&c){return c.opcode_bit_u3_r8(3,c.L);};
    t[0x5E]=[](CPU&c){return c.opcode_bit_u3_HL(3);};     t[0x5F]=[](CPU&c){return c.opcode_bit_u3_r8(3,c.A);};
    t[0x60]=[](CPU&c){return c.opcode_bit_u3_r8(4,c.B);}; t[0x61]=[](CPU&c){return c.opcode_bit_u3_r8(4,c.C);};
    t[0x62]=[](CPU&c){return c.opcode_bit_u3_r8(4,c.D);}; t[0x63]=[](CPU&c){return c.opcode_bit_u3_r8(4,c.E);};
    t[0x64]=[](CPU&c){return c.opcode_bit_u3_r8(4,c.H);}; t[0x65]=[](CPU&c){return c.opcode_bit_u3_r8(4,c.L);};
    t[0x66]=[](CPU&c){return c.opcode_bit_u3_HL(4);};     t[0x67]=[](CPU&c){return c.opcode_bit_u3_r8(4,c.A);};
    t[0x68]=[](CPU&c){return c.opcode_bit_u3_r8(5,c.B);}; t[0x69]=[](CPU&c){return c.opcode_bit_u3_r8(5,c.C);};
    t[0x6A]=[](CPU&c){return c.opcode_bit_u3_r8(5,c.D);}; t[0x6B]=[](CPU&c){return c.opcode_bit_u3_r8(5,c.E);};
    t[0x6C]=[](CPU&c){return c.opcode_bit_u3_r8(5,c.H);}; t[0x6D]=[](CPU&c){return c.opcode_bit_u3_r8(5,c.L);};
    t[0x6E]=[](CPU&c){return c.opcode_bit_u3_HL(5);};     t[0x6F]=[](CPU&c){return c.opcode_bit_u3_r8(5,c.A);};
    t[0x70]=[](CPU&c){return c.opcode_bit_u3_r8(6,c.B);}; t[0x71]=[](CPU&c){return c.opcode_bit_u3_r8(6,c.C);};
    t[0x72]=[](CPU&c){return c.opcode_bit_u3_r8(6,c.D);}; t[0x73]=[](CPU&c){return c.opcode_bit_u3_r8(6,c.E);};
    t[0x74]=[](CPU&c){return c.opcode_bit_u3_r8(6,c.H);}; t[0x75]=[](CPU&c){return c.opcode_bit_u3_r8(6,c.L);};
    t[0x76]=[](CPU&c){return c.opcode_bit_u3_HL(6);};     t[0x77]=[](CPU&c){return c.opcode_bit_u3_r8(6,c.A);};
    t[0x78]=[](CPU&c){return c.opcode_bit_u3_r8(7,c.B);}; t[0x79]=[](CPU&c){return c.opcode_bit_u3_r8(7,c.C);};
    t[0x7A]=[](CPU&c){return c.opcode_bit_u3_r8(7,c.D);}; t[0x7B]=[](CPU&c){return c.opcode_bit_u3_r8(7,c.E);};
    t[0x7C]=[](CPU&c){return c.opcode_bit_u3_r8(7,c.H);}; t[0x7D]=[](CPU&c){return c.opcode_bit_u3_r8(7,c.L);};
    t[0x7E]=[](CPU&c){return c.opcode_bit_u3_HL(7);};     t[0x7F]=[](CPU&c){return c.opcode_bit_u3_r8(7,c.A);};
    // RES (0x80-0xBF)
    t[0x80]=[](CPU&c){return c.opcode_res_u3_r8(0,c.B);}; t[0x81]=[](CPU&c){return c.opcode_res_u3_r8(0,c.C);};
    t[0x82]=[](CPU&c){return c.opcode_res_u3_r8(0,c.D);}; t[0x83]=[](CPU&c){return c.opcode_res_u3_r8(0,c.E);};
    t[0x84]=[](CPU&c){return c.opcode_res_u3_r8(0,c.H);}; t[0x85]=[](CPU&c){return c.opcode_res_u3_r8(0,c.L);};
    t[0x86]=[](CPU&c){return c.opcode_res_u3_HL(0);};     t[0x87]=[](CPU&c){return c.opcode_res_u3_r8(0,c.A);};
    t[0x88]=[](CPU&c){return c.opcode_res_u3_r8(1,c.B);}; t[0x89]=[](CPU&c){return c.opcode_res_u3_r8(1,c.C);};
    t[0x8A]=[](CPU&c){return c.opcode_res_u3_r8(1,c.D);}; t[0x8B]=[](CPU&c){return c.opcode_res_u3_r8(1,c.E);};
    t[0x8C]=[](CPU&c){return c.opcode_res_u3_r8(1,c.H);}; t[0x8D]=[](CPU&c){return c.opcode_res_u3_r8(1,c.L);};
    t[0x8E]=[](CPU&c){return c.opcode_res_u3_HL(1);};     t[0x8F]=[](CPU&c){return c.opcode_res_u3_r8(1,c.A);};
    t[0x90]=[](CPU&c){return c.opcode_res_u3_r8(2,c.B);}; t[0x91]=[](CPU&c){return c.opcode_res_u3_r8(2,c.C);};
    t[0x92]=[](CPU&c){return c.opcode_res_u3_r8(2,c.D);}; t[0x93]=[](CPU&c){return c.opcode_res_u3_r8(2,c.E);};
    t[0x94]=[](CPU&c){return c.opcode_res_u3_r8(2,c.H);}; t[0x95]=[](CPU&c){return c.opcode_res_u3_r8(2,c.L);};
    t[0x96]=[](CPU&c){return c.opcode_res_u3_HL(2);};     t[0x97]=[](CPU&c){return c.opcode_res_u3_r8(2,c.A);};
    t[0x98]=[](CPU&c){return c.opcode_res_u3_r8(3,c.B);}; t[0x99]=[](CPU&c){return c.opcode_res_u3_r8(3,c.C);};
    t[0x9A]=[](CPU&c){return c.opcode_res_u3_r8(3,c.D);}; t[0x9B]=[](CPU&c){return c.opcode_res_u3_r8(3,c.E);};
    t[0x9C]=[](CPU&c){return c.opcode_res_u3_r8(3,c.H);}; t[0x9D]=[](CPU&c){return c.opcode_res_u3_r8(3,c.L);};
    t[0x9E]=[](CPU&c){return c.opcode_res_u3_HL(3);};     t[0x9F]=[](CPU&c){return c.opcode_res_u3_r8(3,c.A);};
    t[0xA0]=[](CPU&c){return c.opcode_res_u3_r8(4,c.B);}; t[0xA1]=[](CPU&c){return c.opcode_res_u3_r8(4,c.C);};
    t[0xA2]=[](CPU&c){return c.opcode_res_u3_r8(4,c.D);}; t[0xA3]=[](CPU&c){return c.opcode_res_u3_r8(4,c.E);};
    t[0xA4]=[](CPU&c){return c.opcode_res_u3_r8(4,c.H);}; t[0xA5]=[](CPU&c){return c.opcode_res_u3_r8(4,c.L);};
    t[0xA6]=[](CPU&c){return c.opcode_res_u3_HL(4);};     t[0xA7]=[](CPU&c){return c.opcode_res_u3_r8(4,c.A);};
    t[0xA8]=[](CPU&c){return c.opcode_res_u3_r8(5,c.B);}; t[0xA9]=[](CPU&c){return c.opcode_res_u3_r8(5,c.C);};
    t[0xAA]=[](CPU&c){return c.opcode_res_u3_r8(5,c.D);}; t[0xAB]=[](CPU&c){return c.opcode_res_u3_r8(5,c.E);};
    t[0xAC]=[](CPU&c){return c.opcode_res_u3_r8(5,c.H);}; t[0xAD]=[](CPU&c){return c.opcode_res_u3_r8(5,c.L);};
    t[0xAE]=[](CPU&c){return c.opcode_res_u3_HL(5);};     t[0xAF]=[](CPU&c){return c.opcode_res_u3_r8(5,c.A);};
    t[0xB0]=[](CPU&c){return c.opcode_res_u3_r8(6,c.B);}; t[0xB1]=[](CPU&c){return c.opcode_res_u3_r8(6,c.C);};
    t[0xB2]=[](CPU&c){return c.opcode_res_u3_r8(6,c.D);}; t[0xB3]=[](CPU&c){return c.opcode_res_u3_r8(6,c.E);};
    t[0xB4]=[](CPU&c){return c.opcode_res_u3_r8(6,c.H);}; t[0xB5]=[](CPU&c){return c.opcode_res_u3_r8(6,c.L);};
    t[0xB6]=[](CPU&c){return c.opcode_res_u3_HL(6);};     t[0xB7]=[](CPU&c){return c.opcode_res_u3_r8(6,c.A);};
    t[0xB8]=[](CPU&c){return c.opcode_res_u3_r8(7,c.B);}; t[0xB9]=[](CPU&c){return c.opcode_res_u3_r8(7,c.C);};
    t[0xBA]=[](CPU&c){return c.opcode_res_u3_r8(7,c.D);}; t[0xBB]=[](CPU&c){return c.opcode_res_u3_r8(7,c.E);};
    t[0xBC]=[](CPU&c){return c.opcode_res_u3_r8(7,c.H);}; t[0xBD]=[](CPU&c){return c.opcode_res_u3_r8(7,c.L);};
    t[0xBE]=[](CPU&c){return c.opcode_res_u3_HL(7);};     t[0xBF]=[](CPU&c){return c.opcode_res_u3_r8(7,c.A);};
    // SET (0xC0-0xFF)
    t[0xC0]=[](CPU&c){return c.opcode_set_u3_r8(0,c.B);}; t[0xC1]=[](CPU&c){return c.opcode_set_u3_r8(0,c.C);};
    t[0xC2]=[](CPU&c){return c.opcode_set_u3_r8(0,c.D);}; t[0xC3]=[](CPU&c){return c.opcode_set_u3_r8(0,c.E);};
    t[0xC4]=[](CPU&c){return c.opcode_set_u3_r8(0,c.H);}; t[0xC5]=[](CPU&c){return c.opcode_set_u3_r8(0,c.L);};
    t[0xC6]=[](CPU&c){return c.opcode_set_u3_HL(0);};     t[0xC7]=[](CPU&c){return c.opcode_set_u3_r8(0,c.A);};
    t[0xC8]=[](CPU&c){return c.opcode_set_u3_r8(1,c.B);}; t[0xC9]=[](CPU&c){return c.opcode_set_u3_r8(1,c.C);};
    t[0xCA]=[](CPU&c){return c.opcode_set_u3_r8(1,c.D);}; t[0xCB]=[](CPU&c){return c.opcode_set_u3_r8(1,c.E);};
    t[0xCC]=[](CPU&c){return c.opcode_set_u3_r8(1,c.H);}; t[0xCD]=[](CPU&c){return c.opcode_set_u3_r8(1,c.L);};
    t[0xCE]=[](CPU&c){return c.opcode_set_u3_HL(1);};     t[0xCF]=[](CPU&c){return c.opcode_set_u3_r8(1,c.A);};
    t[0xD0]=[](CPU&c){return c.opcode_set_u3_r8(2,c.B);}; t[0xD1]=[](CPU&c){return c.opcode_set_u3_r8(2,c.C);};
    t[0xD2]=[](CPU&c){return c.opcode_set_u3_r8(2,c.D);}; t[0xD3]=[](CPU&c){return c.opcode_set_u3_r8(2,c.E);};
    t[0xD4]=[](CPU&c){return c.opcode_set_u3_r8(2,c.H);}; t[0xD5]=[](CPU&c){return c.opcode_set_u3_r8(2,c.L);};
    t[0xD6]=[](CPU&c){return c.opcode_set_u3_HL(2);};     t[0xD7]=[](CPU&c){return c.opcode_set_u3_r8(2,c.A);};
    t[0xD8]=[](CPU&c){return c.opcode_set_u3_r8(3,c.B);}; t[0xD9]=[](CPU&c){return c.opcode_set_u3_r8(3,c.C);};
    t[0xDA]=[](CPU&c){return c.opcode_set_u3_r8(3,c.D);}; t[0xDB]=[](CPU&c){return c.opcode_set_u3_r8(3,c.E);};
    t[0xDC]=[](CPU&c){return c.opcode_set_u3_r8(3,c.H);}; t[0xDD]=[](CPU&c){return c.opcode_set_u3_r8(3,c.L);};
    t[0xDE]=[](CPU&c){return c.opcode_set_u3_HL(3);};     t[0xDF]=[](CPU&c){return c.opcode_set_u3_r8(3,c.A);};
    t[0xE0]=[](CPU&c){return c.opcode_set_u3_r8(4,c.B);}; t[0xE1]=[](CPU&c){return c.opcode_set_u3_r8(4,c.C);};
    t[0xE2]=[](CPU&c){return c.opcode_set_u3_r8(4,c.D);}; t[0xE3]=[](CPU&c){return c.opcode_set_u3_r8(4,c.E);};
    t[0xE4]=[](CPU&c){return c.opcode_set_u3_r8(4,c.H);}; t[0xE5]=[](CPU&c){return c.opcode_set_u3_r8(4,c.L);};
    t[0xE6]=[](CPU&c){return c.opcode_set_u3_HL(4);};     t[0xE7]=[](CPU&c){return c.opcode_set_u3_r8(4,c.A);};
    t[0xE8]=[](CPU&c){return c.opcode_set_u3_r8(5,c.B);}; t[0xE9]=[](CPU&c){return c.opcode_set_u3_r8(5,c.C);};
    t[0xEA]=[](CPU&c){return c.opcode_set_u3_r8(5,c.D);}; t[0xEB]=[](CPU&c){return c.opcode_set_u3_r8(5,c.E);};
    t[0xEC]=[](CPU&c){return c.opcode_set_u3_r8(5,c.H);}; t[0xED]=[](CPU&c){return c.opcode_set_u3_r8(5,c.L);};
    t[0xEE]=[](CPU&c){return c.opcode_set_u3_HL(5);};     t[0xEF]=[](CPU&c){return c.opcode_set_u3_r8(5,c.A);};
    t[0xF0]=[](CPU&c){return c.opcode_set_u3_r8(6,c.B);}; t[0xF1]=[](CPU&c){return c.opcode_set_u3_r8(6,c.C);};
    t[0xF2]=[](CPU&c){return c.opcode_set_u3_r8(6,c.D);}; t[0xF3]=[](CPU&c){return c.opcode_set_u3_r8(6,c.E);};
    t[0xF4]=[](CPU&c){return c.opcode_set_u3_r8(6,c.H);}; t[0xF5]=[](CPU&c){return c.opcode_set_u3_r8(6,c.L);};
    t[0xF6]=[](CPU&c){return c.opcode_set_u3_HL(6);};     t[0xF7]=[](CPU&c){return c.opcode_set_u3_r8(6,c.A);};
    t[0xF8]=[](CPU&c){return c.opcode_set_u3_r8(7,c.B);}; t[0xF9]=[](CPU&c){return c.opcode_set_u3_r8(7,c.C);};
    t[0xFA]=[](CPU&c){return c.opcode_set_u3_r8(7,c.D);}; t[0xFB]=[](CPU&c){return c.opcode_set_u3_r8(7,c.E);};
    t[0xFC]=[](CPU&c){return c.opcode_set_u3_r8(7,c.H);}; t[0xFD]=[](CPU&c){return c.opcode_set_u3_r8(7,c.L);};
    t[0xFE]=[](CPU&c){return c.opcode_set_u3_HL(7);};     t[0xFF]=[](CPU&c){return c.opcode_set_u3_r8(7,c.A);};

    return t;
}();
