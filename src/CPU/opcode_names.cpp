#include "opcode_names.hpp"

// Rempli pour les opcodes actuellement mappés dans opcode_mapping.hpp.
// Les autres restent nullptr -> affichés en "DB 0xXX".
// (Remplissage par lambda : les initialiseurs désignés de tableau ne sont pas
//  portables en C++ — g++ les refuse.)
const std::array<const char*, 256> OPCODE_NAMES = [] {
    std::array<const char*, 256> t{};   // nullptr partout

    t[0x00] = "NOP";

    // LD r,n8
    t[0x06] = "LD B,n8"; t[0x0E] = "LD C,n8"; t[0x16] = "LD D,n8"; t[0x1E] = "LD E,n8";
    t[0x26] = "LD H,n8"; t[0x2E] = "LD L,n8"; t[0x3E] = "LD A,n8";

    // LD rr,n16
    t[0x01] = "LD BC,n16"; t[0x11] = "LD DE,n16"; t[0x21] = "LD HL,n16"; t[0x31] = "LD SP,n16";

    // LD (rr),A / LD A,(rr)
    t[0x02] = "LD (BC),A"; t[0x12] = "LD (DE),A"; t[0x0A] = "LD A,(BC)"; t[0x1A] = "LD A,(DE)";

    // (HL+/-)
    t[0x22] = "LD (HL+),A"; t[0x32] = "LD (HL-),A"; t[0x2A] = "LD A,(HL+)"; t[0x3A] = "LD A,(HL-)";

    // LD (HL),r
    t[0x70] = "LD (HL),B"; t[0x71] = "LD (HL),C"; t[0x72] = "LD (HL),D"; t[0x73] = "LD (HL),E";
    t[0x74] = "LD (HL),H"; t[0x75] = "LD (HL),L"; t[0x77] = "LD (HL),A";

    // LD r,(HL)
    t[0x46] = "LD B,(HL)"; t[0x4E] = "LD C,(HL)"; t[0x7E] = "LD A,(HL)";

    // accès absolus / haute mémoire
    t[0xEA] = "LD (a16),A"; t[0xFA] = "LD A,(a16)";
    t[0xE0] = "LDH (a8),A"; t[0xF0] = "LDH A,(a8)";
    t[0xE2] = "LDH (C),A";  t[0xF2] = "LDH A,(C)";
    t[0x08] = "LD (a16),SP"; t[0xF8] = "LD HL,SP+e8";

    // ADC A,r + n8
    t[0x88] = "ADC A,B"; t[0x89] = "ADC A,C"; t[0x8A] = "ADC A,D"; t[0x8B] = "ADC A,E";
    t[0x8C] = "ADC A,H"; t[0x8D] = "ADC A,L"; t[0x8E] = "ADC A,(HL)"; t[0x8F] = "ADC A,A";
    t[0xCE] = "ADC A,n8";

    // ADD A,r
    t[0x80] = "ADD A,B"; t[0x81] = "ADD A,C"; t[0x82] = "ADD A,D"; t[0x83] = "ADD A,E";
    t[0x84] = "ADD A,H"; t[0x85] = "ADD A,L"; t[0x86] = "ADD A,(HL)"; t[0x87] = "ADD A,A";

    return t;
}();

const std::array<const char*, 256> CB_OPCODE_NAMES = [] {
    std::array<const char*, 256> t{};
    // à remplir quand les instructions CB seront implémentées
    return t;
}();
