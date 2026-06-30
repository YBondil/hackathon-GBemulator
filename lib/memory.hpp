#pragma once
#include "definitions.hpp"


class Memory {
    public:
    auto read(const u16 adress)-> u8; //TODO
    auto write(u16 adress, u8 value) -> void; //TODO
    void tick(Cycles cycle); //TODO
} ;
