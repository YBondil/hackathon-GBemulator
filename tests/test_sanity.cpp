// Batterie de tests "sanity" — composants isolés (sans Memory ni CPU).
// Compile avec : registers.cpp + timer.cpp
#include "bitwise.hpp"
#include "registers.hpp"
#include "timer.hpp"
#include <cstdio>

static int g_pass = 0, g_fail = 0;
#define CHECK(cond) do { \
    if (cond) { ++g_pass; } \
    else { ++g_fail; std::printf("  [FAIL] %s:%d  %s\n", __FILE__, __LINE__, #cond); } \
} while (0)

static void test_bitwise() {
    std::puts("== bitwise ==");
    CHECK(bitwise::set_bit_to(0x00, 3, true)  == 0x08);
    CHECK(bitwise::set_bit_to(0xFF, 3, false) == 0xF7);
    CHECK(bitwise::check_bit(0x80, 7) == true);
    CHECK(bitwise::check_bit(0x80, 0) == false);
    CHECK(bitwise::compose_bytes(0x12, 0x34) == 0x1234);
    CHECK(bitwise::compose_nibbles(0xA, 0xB) == 0xAB);
    CHECK(bitwise::clear_bit(0xFF, 0) == 0xFE);
}

static void test_register() {
    std::puts("== Register ==");
    Register r;
    CHECK(r.value() == 0x00);                  // valeur par défaut
    r.set(0x42);   CHECK(r.value() == 0x42);   // set/value
    r.increment(); CHECK(r.value() == 0x43);
    r.set(0xFF); r.increment(); CHECK(r.value() == 0x00);  // wrap 0xFF -> 0x00
    r.set(0x00); r.decrement(); CHECK(r.value() == 0xFF);  // wrap 0x00 -> 0xFF
    r.set(0x00); r.setBitTo(5, true);  CHECK(r.checkBit(5) == true);
    r.setBitTo(5, false); CHECK(r.checkBit(5) == false);
    r.set(0x7E); CHECK((r == 0x7E) == true);
}

static void test_flag_register() {
    std::puts("== FlagRegister ==");
    FlagRegister f;
    f.set(0xFF); CHECK(f.value() == 0xF0);     // nibble bas toujours 0
    f.set(0x00);
    f.set_flag_zero(true);       CHECK(f.flag_zero() == true);
    CHECK(f.flag_subtract() == false);         // indépendance des flags
    f.set_flag_carry(true);      CHECK(f.flag_carry() == true);
    CHECK(f.value() == 0x90);                  // bit7 (Z) + bit4 (C)
    f.set_flag_zero(false);      CHECK(f.flag_zero() == false);
    CHECK(f.flag_carry() == true);             // C non affecté
    CHECK(f.flag_carry_value() == 1);
    CHECK(f.flag_zero_value() == 0);
    f.set_flag_half_carry(true); CHECK(f.checkBit(5) == true);
}

static void test_register16() {
    std::puts("== Register16 ==");
    Register16 sp(0xFFFE);
    CHECK(sp.value() == 0xFFFE);
    sp.increment(); CHECK(sp.value() == 0xFFFF);
    sp.increment(); CHECK(sp.value() == 0x0000);   // wrap 16 bits
    sp.decrement(); CHECK(sp.value() == 0xFFFF);
    sp.set(0x1234); CHECK(sp.value() == 0x1234);
}

static void test_timer_div() {
    std::puts("== Timer / DIV ==");
    Timer t;
    t.tick(Cycles(255)); CHECK(t.read(0xFF04) == 0x00);   // pas encore 256
    t.tick(Cycles(1));   CHECK(t.read(0xFF04) == 0x01);   // 256 -> +1
    t.tick(Cycles(512)); CHECK(t.read(0xFF04) == 0x03);   // +2
    t.write(0xFF04, 0x55); CHECK(t.read(0xFF04) == 0x00); // écrire DIV -> reset
}

static void test_timer_tima_disabled() {
    std::puts("== Timer / TIMA désactivé ==");
    Timer t;                       // TAC=0 par défaut -> timer off
    t.write(0xFF05, 0x00);
    t.tick(Cycles(100000));
    CHECK(t.read(0xFF05) == 0x00); // TIMA gelé
    CHECK(t.take_interrupt() == false);
}

static void test_timer_tima_freq() {
    std::puts("== Timer / TIMA fréquences ==");
    {   Timer t; t.write(0xFF07, 0b100);           // enable, freq 00 -> 1024
        t.tick(Cycles(1023)); CHECK(t.read(0xFF05) == 0x00);
        t.tick(Cycles(1));    CHECK(t.read(0xFF05) == 0x01); }
    {   Timer t; t.write(0xFF07, 0b101);           // enable, freq 01 -> 16
        t.tick(Cycles(16));   CHECK(t.read(0xFF05) == 0x01); }
    {   Timer t; t.write(0xFF07, 0b110);           // enable, freq 10 -> 64
        t.tick(Cycles(64));   CHECK(t.read(0xFF05) == 0x01); }
    {   Timer t; t.write(0xFF07, 0b111);           // enable, freq 11 -> 256
        t.tick(Cycles(256));  CHECK(t.read(0xFF05) == 0x01); }
}

static void test_timer_overflow() {
    std::puts("== Timer / débordement TIMA ==");
    Timer t;
    t.write(0xFF06, 0xAB);     // TMA = valeur de rechargement
    t.write(0xFF05, 0xFF);     // TIMA proche du débordement
    t.write(0xFF07, 0b101);    // enable, freq 16
    t.tick(Cycles(16));        // un incrément -> débordement
    CHECK(t.read(0xFF05) == 0xAB);          // rechargé avec TMA
    CHECK(t.take_interrupt() == true);      // interruption demandée
    CHECK(t.take_interrupt() == false);     // flag consommé une seule fois
}

static void test_timer_tac_read() {
    std::puts("== Timer / lecture TAC ==");
    Timer t;
    t.write(0xFF07, 0b101);
    CHECK(t.read(0xFF07) == (0b101 | 0xF8)); // bits hauts lus à 1
}

int main() {
    test_bitwise();
    test_register();
    test_flag_register();
    test_register16();
    test_timer_div();
    test_timer_tima_disabled();
    test_timer_tima_freq();
    test_timer_overflow();
    test_timer_tac_read();

    std::printf("\n=== RÉSULTAT : %d réussis, %d échoués ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
