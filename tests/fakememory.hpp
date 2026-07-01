#pragma once
// FakeMemory — mémoire factice pour les tests (ne pas linker dans gbemu).
//
// Fournit les 64 Kio adressables de la Game Boy, avec :
//   - un Timer intégré (registres 0xFF04-0xFF07 routés vers lui)
//   - les registres d'interruption IF (0xFF0F) et IE (0xFFFF)
//   - tick() qui avance le timer et lève IF.bit2 au débordement de TIMA
//   - des helpers de test (load, raw, request_interrupt)


#include "definitions.hpp"
#include "timer.hpp"
#include "memory.hpp"
#include <array>
#include <vector>
#include <cstddef>

class FakeMemory : public Memory  {
public:
    // Bits du registre IF / IE (ordre de priorité)
    enum Interrupt : u8 {
        VBLANK = 0, LCD_STAT = 1, TIMER = 2, SERIAL = 3, JOYPAD = 4
    };

    static constexpr u16 IF_ADDR = 0xFF0F;
    static constexpr u16 IE_ADDR = 0xFFFF;

    FakeMemory() = default;

    auto read(u16 address) -> u8 {
        switch (address) {
            case 0xFF04: case 0xFF05: case 0xFF06: case 0xFF07:
                return timer.read(address);
            default:
                return data[address];
        }
    }

    void write(u16 address, u8 value) {
        switch (address) {
            case 0xFF04: case 0xFF05: case 0xFF06: case 0xFF07:
                timer.write(address, value);
                break;
            default:
                data[address] = value;
                break;
        }
    }

    // Avance le timer (et plus tard le PPU) ; reporte le débordement sur IF.
    void tick(Cycles cycle) {
        timer.tick(cycle);
        if (timer.take_interrupt()) {
            request_interrupt(TIMER);
        }
    }

    // ---- helpers réservés aux tests ----

    // Charge une suite d'octets (ex. un petit programme) à partir de 'start'.
    void load(u16 start, const std::vector<u8>& bytes) {
        for (std::size_t i = 0; i < bytes.size(); ++i) {
            data[static_cast<u16>(start + i)] = bytes[i];
        }
    }

    // Lève manuellement une demande d'interruption (bit dans IF).
    void request_interrupt(Interrupt which) {
        data[IF_ADDR] = static_cast<u8>(data[IF_ADDR] | (1 << which));
    }

    // Accès direct, SANS routage timer (pour inspecter l'état brut en test).
    auto raw(u16 address) const -> u8 { return data[address]; }
    void set_raw(u16 address, u8 value) { data[address] = value; }

    // Accès au timer intégré si un test veut l'inspecter finement.
    auto timer_unit() -> Timer& { return timer; }

private:
    std::array<u8, 0x10000> data{};   // 64 Kio, zéro-initialisés
    Timer timer{};
};
