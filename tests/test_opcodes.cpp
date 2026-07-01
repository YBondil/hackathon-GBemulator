// Test des opcodes via le VRAI run_opcode (décodage + fetch + opération).
//
// Pour chaque instruction : on écrit son encodage binaire en mémoire à 0x0100,
// puis on exécute exactement comme run() : fetch8() de l'opcode -> run_opcode().
// On vérifie ensuite l'effet (registre / mémoire).
//
// Certaines assertions ÉCHOUENT volontairement : elles pointent des bugs connus
// (Register passé par valeur, A.set(adresse) au lieu du contenu, double PC...).
#include "CPU/CPU.hpp"
#include "fakememory.hpp"
#include <vector>
#include <cstdio>

struct CpuTest {
    FakeMemory mem;
    CPU cpu{mem};

    Register&     A()  { return cpu.A; }
    Register&     B()  { return cpu.B; }
    Register&     C()  { return cpu.C; }
    Register&     D()  { return cpu.D; }
    Register&     E()  { return cpu.E; }
    Register&     H()  { return cpu.H; }
    Register&     L()  { return cpu.L; }
    FlagRegister& F()  { return cpu.F; }
    RegisterPair& BC() { return cpu.BC; }
    RegisterPair& DE() { return cpu.DE; }
    RegisterPair& HL() { return cpu.HL; }
    Register16&   SP() { return cpu.SP; }
    u16&          PC() { return cpu.PC; }

    // charge l'instruction à 0x0100 puis l'exécute via le dispatch réel
    Cycles exec(std::initializer_list<u8> bytes) {
        cpu.PC = 0x0100;
        mem.load(0x0100, std::vector<u8>(bytes));
        u8 op = cpu.fetch8();         // comme run() : lit l'opcode (PC++)
        return cpu.run_opcode(op);    // -> opcode_table / cb_table
    }
};

static int pass = 0, fail = 0;
#define CHECK(c) do { if (c) ++pass; else { ++fail; \
    std::printf("  [FAIL] %s:%d  %s\n", __FILE__, __LINE__, #c); } } while (0)

int main() {
    // ===== Contrôle =====
    std::puts("== NOP ==");
    { CpuTest t; Cycles c = t.exec({0x00});
      CHECK(c.value() == 1);
      CHECK(t.PC() == 0x0101); }                    // 1 octet

    // ===== LD r,n8 (par valeur -> bug attendu) =====
    std::puts("== LD r,n8 ==");
    { CpuTest t; t.exec({0x3E, 0x42}); CHECK(t.A().value() == 0x42); }  // LD A,n8  (BUG)
    { CpuTest t; t.exec({0x06, 0x42}); CHECK(t.B().value() == 0x42); }  // LD B,n8  (BUG)

    // ===== LD rr,n16 (RegisterPair garde ses refs -> OK) =====
    std::puts("== LD rr,n16 ==");
    { CpuTest t; t.exec({0x01, 0x34, 0x12}); CHECK(t.BC().value() == 0x1234); } // LD BC,n16
    { CpuTest t; t.exec({0x21, 0x00, 0xC0}); CHECK(t.HL().value() == 0xC000); } // LD HL,n16
    { CpuTest t; t.exec({0x31, 0xFE, 0xFF}); CHECK(t.SP().value() == 0xFFFE); } // LD SP,n16

    // ===== LD (rr),A et LD A,(rr) =====
    std::puts("== LD (rr),A / LD A,(rr) ==");
    { CpuTest t; t.A().set(0x99); t.BC().set(0xC000); t.exec({0x02});
      CHECK(t.mem.raw(0xC000) == 0x99); }                                 // LD (BC),A
    { CpuTest t; t.mem.write(0xC000, 0x77); t.BC().set(0xC000); t.exec({0x0A});
      CHECK(t.A().value() == 0x77); }                                     // LD A,(BC)

    // ===== LD (HL+/-),A et LD A,(HL+/-) =====
    std::puts("== LD (HL+/-) ==");
    { CpuTest t; t.A().set(0x55); t.HL().set(0xC000); t.exec({0x22});
      CHECK(t.mem.raw(0xC000) == 0x55); CHECK(t.HL().value() == 0xC001); }// LD (HL+),A
    { CpuTest t; t.mem.write(0xC000, 0x33); t.HL().set(0xC000); t.exec({0x2A});
      CHECK(t.A().value() == 0x33); CHECK(t.HL().value() == 0xC001); }    // LD A,(HL+)

    // ===== LD (HL),r et LD r,(HL) =====
    std::puts("== LD (HL),r / LD r,(HL) ==");
    { CpuTest t; t.A().set(0xAB); t.HL().set(0xC000); t.exec({0x77});
      CHECK(t.mem.raw(0xC000) == 0xAB); }                                 // LD (HL),A
    { CpuTest t; t.mem.write(0xC000, 0xCD); t.HL().set(0xC000); t.exec({0x7E});
      CHECK(t.A().value() == 0xCD); }                                     // LD A,(HL) (BUG par valeur)

    // ===== Accès absolus / haute mémoire =====
    std::puts("== LD (n16),A / LD A,(n16) / LDH ==");
    { CpuTest t; t.A().set(0x42); t.exec({0xEA, 0x05, 0xC0});
      CHECK(t.mem.raw(0xC005) == 0x42); }                                 // LD (n16),A
    { CpuTest t; t.mem.write(0xC005, 0x99); t.exec({0xFA, 0x05, 0xC0});
      CHECK(t.A().value() == 0x99); }                                     // LD A,(n16) (BUG)
    { CpuTest t; t.A().set(0x12); t.exec({0xE0, 0x80});
      CHECK(t.mem.raw(0xFF80) == 0x12); }                                 // LDH (a8),A

    // ===== ADC =====
    std::puts("== ADC ==");
    { CpuTest t; t.A().set(0x10); t.B().set(0x05); t.F().set(0x00); t.exec({0x88});
      CHECK(t.A().value() == 0x15); }                                     // ADC A,B
    { CpuTest t; t.A().set(0x10); t.F().set(0x00); t.exec({0xCE, 0x05});
      CHECK(t.A().value() == 0x15);                                       // ADC A,n8 (résultat OK)
      CHECK(t.PC() == 0x0102); }                                         // 2 octets (BUG double PC)

    // ===== ADD =====
    std::puts("== ADD ==");
    { CpuTest t; t.A().set(0x10); t.B().set(0x20); t.F().set(0x00); t.exec({0x80});
      CHECK(t.A().value() == 0x30); }                                     // ADD A,B
    { CpuTest t; t.A().set(0x10); t.HL().set(0xC000); t.mem.write(0xC000, 0x05); t.exec({0x86});
      CHECK(t.A().value() == 0x15); }                                     // ADD A,(HL) (BUG : lit reg depuis PC)

    std::printf("\n=== Opcodes : %d réussis, %d échoués ===\n", pass, fail);
    return fail == 0 ? 0 : 1;
}
