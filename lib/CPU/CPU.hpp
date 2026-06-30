#include "definitions.hpp"
#include "registers.hpp"
#include "memory.hpp"


class CPU {
    public :
        CPU(Memory& memory)
            : memory(memory),
              isRunning(true),
              PC(0x0100),            // point d'entrée après le boot ROM
              SP(0xFFFE),            // valeur d'init
              AF(A, F), BC(B, C), DE(D, E), HL(H, L)   // voir #4 et #15
        {}
        auto run() -> void;

    private :
        Cycles run_opcode(u8 opcode) ;
        Memory& memory ;
        bool isRunning;
        u16 PC ;//Program Counter
        u16 SP ; //Stack Counter
        //registers
        Register A,B,C,D,E,H,L ;
        FlagRegister F;
        RegisterPair AF, BC, DE, HL;

        // clock ? TODO

        //OPCodes
        auto _opcode_adc(u8 value)->Cycles;
        auto opcode_adc_HL()->Cycles;
        auto opcode_adc_n8(u8 value)->Cycles;
} ;
