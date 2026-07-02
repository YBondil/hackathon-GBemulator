#pragma once
#include "definitions.hpp"
#include <vector>
#include <string>
#include <array>

class Gameboy;

class Memory {
    public:
        Memory( Gameboy& gb, std::vector<u8>& rom);
        ~Memory() = default;

        auto read(u16 address) -> u8;
        void write(u16 address, u8 data);
        void tick(Cycles cycle);
        bool boot_rom_active() const;

        void copy_from_file(std::string& file_name);

    private:
        auto read_io(u16 address) -> u8;
        void write_io(u16 address, u8 data);

        Gameboy& Gb;
        std::vector<u8>        ROM_Data;      // cartouche
        std::array<u8, 0x2000> WRAM{};        // 0xC000-0xDFFF
        std::array<u8, 0x00A0> OAM{};         // 0xFE00-0xFE9F
        std::array<u8, 0x0080> IO{};          // 0xFF00-0xFF7F
        std::array<u8, 0x007F> HRAM{};        // 0xFF80-0xFFFE
        u8   IE = 0;                          // 0xFFFF
        bool boot_active = false;
};
