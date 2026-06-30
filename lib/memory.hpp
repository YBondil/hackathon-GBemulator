
#ifndef MEMORY_HPP
#define MEMORY_HPP


#include "./utils/definitions.hpp"
#include <vector>


class Memory {
    private : 
   
    std::vector<u8> WRAM;
    std::vector<u8> OAM;
    std::vector<u8> HRAM;
    

    public:

    Memory();

    u8 read(u16 adress);
    auto read_io(u16 address); 
    auto unmapped_io_read(u16 address);

    void write(const u16 adress, const u8 data);
    void write_io(const u16 address, const u8 data);
    void unmapped_io_write(const u16 address, const u8 data);

    bool boot_rom_active();
};











#endif