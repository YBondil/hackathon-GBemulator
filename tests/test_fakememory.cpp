#include "fakememory.hpp"
#include <cstdio>

static int pass = 0, fail = 0;
#define CHECK(c) do { if (c) ++pass; else { ++fail; \
    std::printf("  [FAIL] %s:%d  %s\n", __FILE__, __LINE__, #c); } } while (0)

int main() {
    // 1. Lecture/écriture RAM basique
    {
        FakeMemory m;
        m.write(0xC000, 0x42);
        CHECK(m.read(0xC000) == 0x42);
        CHECK(m.read(0xC001) == 0x00);   // zéro-init
    }

    // 2. Chargement d'un "programme"
    {
        FakeMemory m;
        m.load(0x0100, {0x00, 0xCE, 0x05});
        CHECK(m.read(0x0100) == 0x00);
        CHECK(m.read(0x0101) == 0xCE);
        CHECK(m.read(0x0102) == 0x05);
    }

    // 3. Routage timer : écrire/lire passe par le Timer (pas la RAM brute)
    {
        FakeMemory m;
        m.write(0xFF07, 0b101);                 // TAC -> timer
        CHECK(m.read(0xFF07) == (0b101 | 0xF8)); // lecture via timer (bits hauts à 1)
        CHECK(m.raw(0xFF07) == 0x00);            // la RAM brute n'a PAS été touchée
    }

    // 4. tick() fait avancer le timer et lève IF.bit2 au débordement
    {
        FakeMemory m;
        m.write(0xFF06, 0xAB);   // TMA
        m.write(0xFF05, 0xFF);   // TIMA proche du débordement
        m.write(0xFF07, 0b101);  // enable, période 16
        CHECK((m.read(FakeMemory::IF_ADDR) & 0x04) == 0); // pas encore d'IRQ
        m.tick(Cycles(16));                                // -> débordement
        CHECK(m.read(0xFF05) == 0xAB);                     // rechargé via TMA
        CHECK((m.read(FakeMemory::IF_ADDR) & 0x04) != 0);  // IF.bit2 (Timer) levé
    }

    // 5. request_interrupt manuel (utile pour tester handle_interrupts du CPU)
    {
        FakeMemory m;
        m.request_interrupt(FakeMemory::VBLANK);
        CHECK((m.read(FakeMemory::IF_ADDR) & 0x01) != 0);
        m.write(FakeMemory::IE_ADDR, 0x1F);
        CHECK(m.read(FakeMemory::IE_ADDR) == 0x1F);
    }

    std::printf("\n=== FakeMemory : %d réussis, %d échoués ===\n", pass, fail);
    return fail == 0 ? 0 : 1;
}
