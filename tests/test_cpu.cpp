// Phase 3 — tests du CPU (stack, interruptions, opcodes) via FakeMemory.
//
// Accès aux membres privés via `struct CpuTest` (friend de CPU dans CPU.hpp).
// Les fonctions de test libres passent toutes par les forwarders de CpuTest.
#include "CPU/CPU.hpp"
#include "fakememory.hpp"
#include <cstdio>

struct CpuTest {
    FakeMemory mem;
    CPU cpu{mem};

    // accès à l'état privé
    u16&         PC()  { return cpu.PC; }
    Register16&  SP()  { return cpu.SP; }
    bool&        IME() { return cpu.IME; }
    Register&    A()   { return cpu.A; }
    Register&    B()   { return cpu.B; }
    FlagRegister& F()  { return cpu.F; }
    RegisterPair& HL() { return cpu.HL; }

    // forwarders vers les méthodes privées
    void   push(u16 v)            { cpu.push_stack(v); }
    u16    pop()                  { return cpu.pop_stack16(); }
    Cycles handle_interrupts()    { return cpu.handle_interrupts(); }
    Cycles adc(u8 v)             { return cpu._opcode_adc(v); }
    Cycles adc_HL()              { return cpu.opcode_adc_HL(); }
    Cycles adc_n8(u8 v)          { return cpu.opcode_adc_n8(v); }
    Cycles ld_HL(Register& r)    { return cpu.opcode_ld_HL(r); }
    Cycles ld_n8(u8 n, Register& r){ return cpu.opcode_ld_n8(n, r); }
    Cycles ld_A_n16(u16 a)       { return cpu.opcode_ld_A_n16(a); }
    Cycles ld_A_HLI()            { return cpu.opcode_ld_A_HLI(); }
};

static int pass = 0, fail = 0;
#define CHECK(c) do { if (c) ++pass; else { ++fail; \
    std::printf("  [FAIL] %s:%d  %s\n", __FILE__, __LINE__, #c); } } while (0)

// ============================================================
//  1. STACK
// ============================================================
static void test_stack() {
    std::puts("== Stack : push/pop ==");
    CpuTest t;
    t.SP().set(0xFFFE);

    t.push(0x1234);
    CHECK(t.SP().value() == 0xFFFC);          // SP descend de 2
    CHECK(t.mem.raw(0xFFFD) == 0x12);         // octet haut
    CHECK(t.mem.raw(0xFFFC) == 0x34);         // octet bas

    CHECK(t.pop() == 0x1234);                  // roundtrip
    CHECK(t.SP().value() == 0xFFFE);          // SP revient

    t.push(0xAAAA);
    t.push(0xBBBB);
    CHECK(t.pop() == 0xBBBB);                   // LIFO
    CHECK(t.pop() == 0xAAAA);
    CHECK(t.SP().value() == 0xFFFE);
}

// ============================================================
//  2. INTERRUPTIONS
// ============================================================
static void test_interrupts() {
    std::puts("== Interruptions : handle_interrupts ==");

    {   CpuTest t; t.PC() = 0x4000; t.IME() = false;
        t.mem.write(FakeMemory::IE_ADDR, 0x1F);
        t.mem.request_interrupt(FakeMemory::TIMER);
        CHECK(t.handle_interrupts().value() == 0);   // IME=0 -> rien
        CHECK(t.PC() == 0x4000); }

    {   CpuTest t; t.PC() = 0x4000; t.IME() = true;
        t.mem.write(FakeMemory::IE_ADDR, 0x00);      // rien autorisé
        t.mem.request_interrupt(FakeMemory::TIMER);
        CHECK(t.handle_interrupts().value() == 0);
        CHECK(t.PC() == 0x4000); }

    {   CpuTest t; t.PC() = 0x4000; t.SP().set(0xFFFE); t.IME() = true;
        t.mem.write(FakeMemory::IE_ADDR, 0x04);      // Timer autorisé
        t.mem.request_interrupt(FakeMemory::TIMER);
        Cycles c = t.handle_interrupts();
        CHECK(c.value() == 5);                        // coût du dispatch
        CHECK(t.PC() == 0x0050);                      // vecteur Timer
        CHECK(t.IME() == false);                      // IME coupé
        CHECK((t.mem.read(FakeMemory::IF_ADDR) & 0x04) == 0); // IF.bit2 acquitté
        CHECK(t.SP().value() == 0xFFFC);              // PC empilé
        CHECK(t.pop() == 0x4000); }                   // ancien PC sur la pile

    {   CpuTest t; t.PC() = 0x4000; t.SP().set(0xFFFE); t.IME() = true;
        t.mem.write(FakeMemory::IE_ADDR, 0x1F);
        t.mem.request_interrupt(FakeMemory::TIMER);
        t.mem.request_interrupt(FakeMemory::VBLANK);
        t.handle_interrupts();
        CHECK(t.PC() == 0x0040);                      // VBlank prioritaire
        CHECK((t.mem.read(FakeMemory::IF_ADDR) & 0x01) == 0); // VBlank acquitté
        CHECK((t.mem.read(FakeMemory::IF_ADDR) & 0x04) != 0); }// Timer en attente
}

// ============================================================
//  3. ADC : résultat & flags
// ============================================================
static void test_adc_flags() {
    std::puts("== ADC : résultat et flags ==");
    {   CpuTest t; t.A().set(0x10); t.F().set(0x00);
        t.adc(0x05);
        CHECK(t.A().value() == 0x15);
        CHECK(t.F().flag_zero() == false);
        CHECK(t.F().flag_carry() == false);
        CHECK(t.F().flag_half_carry() == false); }

    {   CpuTest t; t.A().set(0xFF); t.F().set(0x00);
        t.adc(0x01);                                  // 0xFF+1 = 0x00
        CHECK(t.A().value() == 0x00);
        CHECK(t.F().flag_zero() == true);
        CHECK(t.F().flag_carry() == true);
        CHECK(t.F().flag_half_carry() == true); }

    {   CpuTest t; t.A().set(0x0F); t.F().set(0x00);
        t.adc(0x01);                                  // demi-retenue
        CHECK(t.A().value() == 0x10);
        CHECK(t.F().flag_half_carry() == true);
        CHECK(t.F().flag_carry() == false); }

    {   CpuTest t; t.A().set(0x10); t.F().set(0x00); t.F().set_flag_carry(true);
        t.adc(0x00);                                  // +1 via carry entrant
        CHECK(t.A().value() == 0x11); }
}

// ============================================================
//  4. CONTRAT PC : les opcodes seuls ne touchent pas PC
// ============================================================
// Dans le modèle table, l'avancement de PC est assuré par fetch8/16/s8
// (validé dans test_opcodes via le vrai run_opcode). Appelées en direct,
// les méthodes d'opération ne doivent PAS modifier PC.
static void test_pc_advance() {
    std::puts("== PC : non modifié par les opcodes appelés en direct ==");

    {   CpuTest t; t.PC() = 0x0100;
        t.HL().set(0xC000); t.mem.write(0xC000, 0x05); t.A().set(0x10);
        t.adc_HL();
        CHECK(t.PC() == 0x0100); }   // la méthode seule ne touche pas PC

    {   CpuTest t; t.PC() = 0x0100; t.A().set(0x10);
        t.adc_n8(0x05);
        CHECK(t.PC() == 0x0100); }   // idem
}

// ============================================================
//  5. LD (expose passage par valeur & lecture mémoire manquante)
// ============================================================
static void test_ld() {
    std::puts("== LD : registres & mémoire ==");

    {   CpuTest t; t.HL().set(0xC000); t.B().set(0x42);
        t.ld_HL(t.B());
        CHECK(t.mem.raw(0xC000) == 0x42); }          // LD (HL),B -> mémoire

    {   CpuTest t; t.B().set(0x00);
        t.ld_n8(0x42, t.B());
        CHECK(t.B().value() == 0x42); }              // bug attendu : Register par valeur

    {   CpuTest t; t.mem.write(0xC005, 0x99);
        t.ld_A_n16(0xC005);
        CHECK(t.A().value() == 0x99); }              // bug attendu : A.set(adresse) au lieu du contenu

    {   CpuTest t; t.HL().set(0xC000); t.mem.write(0xC000, 0x77); t.A().set(0x00);
        t.ld_A_HLI();
        CHECK(t.A().value() == 0x77);
        CHECK(t.HL().value() == 0xC001); }           // LD A,(HL+)
}

int main() {
    test_stack();
    test_interrupts();
    test_adc_flags();
    test_pc_advance();
    test_ld();
    std::printf("\n=== CPU (Phase 3) : %d réussis, %d échoués ===\n", pass, fail);
    return fail == 0 ? 0 : 1;
}
