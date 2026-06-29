#include "definitions.hpp"
#include "registers.hpp"
#include "memory.hpp"
#include <vector>


class CPU {
    public :
        CPU(std::vector<u8>& ROM_data, Memory& memory) ;
        auto run() -> void;

    private :

        Memory& memory ;
        std::vector<u8> ROM_data ;

        bool isRunning;
        u16 PC ;//Program Counter
        u16 SP ; //Stack Counter
        //registers
        Register A,B,C,D,E,H,L ;
        FlagRegister F;
        RegisterPair AF, BD, DE, HL;


        // clock ?


        //OPCodes


        auto _opcode_adc(u8 value)->Cycles;
        auto opcode_adc_HL()->Cycles;
        auto opcode_adc_n8(u8 value)->Cycles;
} ;
