
#ifndef MEMORY_HPP
#define MEMORY_HPP


#include "./utils/definitions.hpp"
#include <vector>


class Memory {

    std::vector<u8> ROM_00;
    std::vector<u8> ROM_01;
    std::vector<u8> VRAM;
    std::vector<u8> Ext_RAM;
    std::vector<u8> WRAM_1;
    std::vector<u8> WRAM_2;
    std::vector<u8> Echo_RAM;
    std::vector<u8> OAM;
    // Ajouter message d'erreur si essaie entrer dans zone FEA0 - FEFF (interdit)
    std::vector<u8> IO;
    std::vector<u8> HRAM;
    std::vector<u8> Interrupt;
    
    Memory::Memory();

    u8 Memory::read(u16 adress);
    void Memory::write(u16 adress);


};











#endif