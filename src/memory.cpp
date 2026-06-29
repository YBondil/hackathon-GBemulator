

#include "../lib/memory.hpp"

#include <iostream>




u8 Memory::read(u16 adress);



void Memory::write(u16 adress);



int main () {
    std::cout << "ça compile !" << std::endl;
    Memory::Memory memory_test();
    u8 octet_test = memory_test.read()
    return 0;
}