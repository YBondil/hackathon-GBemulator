#pragma once
#include "definitions.hpp"
#include <vector>

class Memory {
    public :
        Memory();
        ~Memory() = default ;
        auto read(u16 adress) -> u8;
        auto read_io(u16 address);
        auto unmapped_io_read(u16 address)-> u8;
        void write(const u16 adress, const u8 data);
        void write_io(const u16 address, const u8 data);
        void unmapped_io_write(const u16 address, const u8 data);
        bool boot_rom_active();

    private :
        std::vector<u8> WRAM;
        std::vector<u8> OAM;
        std::vector<u8> HRAM;

};
