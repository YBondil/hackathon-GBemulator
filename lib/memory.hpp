#pragma once
#include "definitions.hpp"


class Memory {
    public:
    auto read(const u16 adress)-> u8; //implémenté par la team mémoire
    auto write(u16 adress, u8 value) -> void; //implémenté par la team mémoire
} ;
