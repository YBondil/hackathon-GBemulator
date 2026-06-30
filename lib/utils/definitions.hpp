#pragma once


#include <cstdint>
#include <iostream>

using u8 = uint8_t ;
using u16 = uint16_t ;
using s8 = int8_t ;
using s16 = int16_t ;



template <typename T>
void affiche(T x){
    std::cout << x << std::endl;
}
