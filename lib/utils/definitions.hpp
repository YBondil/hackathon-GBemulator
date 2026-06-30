#pragma once


#include <cstdint>


using u8 = uint8_t ;
using u16 = uint16_t ;
using s8 = int8_t ;
using s16 = int16_t ;

struct Cycles{
    public:
        Cycles(int n): _value(n){};
        int value (){return _value;};

    private:
        int _value = 0;


};

struct Bytes{
    public:
        Bytes(int n): _value(n){};
        int value(){return _value;};

    private:
        int _value = 0;
};

namespace GB_CONST {
    const int CPU_FREQ = 4194304 ;
    const int CPU_FREQ_util = 1048576 ;
    const int T_CYCLE_PER_FRAME = 70224 ;
};
