#pragma once
#include "definitions.hpp"
#include <vector>
#include <array>

class Gameboy;   // forward declaration : évite l'inclusion circulaire avec gameboy.hpp

// Memory = bus mémoire (MMU). Reçoit la Gameboy et délègue aux composants :
//   - VRAM (0x8000-9FFF) et registres LCD (0xFF40-4B) -> Gb.ppu
//   - registres timer (0xFF04-07)                     -> Gb.timer
// Possède ce que personne d'autre ne gère : ROM, WRAM, OAM, HRAM, I/O (dont IF), IE.
class Memory {
    public:
        Memory( Gameboy& gb, std::vector<u8>& rom);
        ~Memory() = default;

        auto read(u16 address) -> u8;
        void write(u16 address, u8 data);
        void tick(Cycles cycle);            // avance le timer + lève IF au débordement
        bool boot_rom_active() const;

    private:
        auto read_io(u16 address) -> u8;
        void write_io(u16 address, u8 data);

        Gameboy& Gb;
        std::vector<u8>        ROM_Data;      // cartouche (pas de MBC pour l'instant)
        std::array<u8, 0x2000> WRAM{};        // 0xC000-0xDFFF
        std::array<u8, 0x00A0> OAM{};         // 0xFE00-0xFE9F
        std::array<u8, 0x0080> IO{};          // 0xFF00-0xFF7F (dont IF = 0xFF0F)
        std::array<u8, 0x007F> HRAM{};        // 0xFF80-0xFFFE
        u8   IE = 0;                          // 0xFFFF
        bool boot_active = false;
};
