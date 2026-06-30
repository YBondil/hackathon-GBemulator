#pragma once
#include "definitions.hpp"
#include "registers.hpp"
#include "memory.hpp"

namespace irq {
    constexpr u16 IF_ADDR = 0xFF0F;
    constexpr u16 IE_ADDR = 0xFFFF;
};

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

        u16 PC ;//Program Counter

        Register16 SP ; //Stack Counter
        void push_stack(u16 value) ;
        auto pop_stack()-> u16;

        //registers
        Register A,B,C,D,E,H,L ;
        FlagRegister F;
        RegisterPair AF, BC, DE, HL;

        // Timer gestion
        bool isRunning;
        bool IME = false ;
        auto handle_interrupts()-> Cycles ;

        //OPCodes
        auto _opcode_adc(u8 value)->Cycles;
        auto opcode_adc_HL()->Cycles;
        auto opcode_adc_n8(u8 value)->Cycles;
} ;
