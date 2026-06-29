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