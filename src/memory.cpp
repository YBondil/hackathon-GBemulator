

#include "../lib/memory.hpp"

#include <iostream>




u8 Memory::read(u16 adress){
    return this->ROM_00[adress];
};



void Memory::write(u16 adress, u8 x){
    this->ROM_00[adress] = x;
};



int main () {
    affiche("ça compile !");
    Memory memory_test;
    affiche("ça marche 1");
    affiche(static_cast<uint16_t>(0));
    memory_test.write(static_cast<uint16_t>(0), static_cast<uint8_t>(42));
    affiche("ça marche 2");
    u8 octet_test = memory_test.read(0);
    affiche(octet_test);



    return 0;
}