
#ifndef MEMORY_HPP
#define MEMORY_HPP


#include "./utils/definitions.hpp"
#include <vector>


class Memory {
    public : 
    std::vector<u8> ROM_00 = std::vector<u8>(16384);
    std::vector<u8> ROM_01 = std::vector<u8>(16384);
    std::vector<u8> VRAM = std::vector<u8>(8192);
    std::vector<u8> Ext_RAM = std::vector<u8>(8192);
    std::vector<u8> WRAM_1 = std::vector<u8>(4096);
    std::vector<u8> WRAM_2 = std::vector<u8>(4096);
    std::vector<u8> Echo_RAM = std::vector<u8>(7680);
    std::vector<u8> OAM = std::vector<u8>(144);
    // Ajouter message d'erreur si essaie entrer dans zone FEA0 - FEFF (interdit)
    std::vector<u8> IO = std::vector<u8>(112);
    std::vector<u8> HRAM = std::vector<u8>(111);
    std::vector<u8> Interrupt = std::vector<u8>(1);
    
    

    u8 read(u16 adress);
    void write(u16 adress, u8 x);


};











#endif