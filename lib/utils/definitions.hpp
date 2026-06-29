#pragma once


#include <cstdint>


using u8 = uint8_t ;
using u16 = uint16_t ;
using s8 = int8_t ;
using s16 = int16_t ;


Struct Brique {
    int Height;
    int Width;
    int *Full; //On note les blocs pleins de la brique
}