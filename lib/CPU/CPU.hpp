#pragma once
#include "definitions.hpp"
#include "registers.hpp"
#include "memory.hpp"
#include <array>
namespace irq {
    constexpr u16 IF_ADDR = 0xFF0F;
    constexpr u16 IE_ADDR = 0xFFFF;
};

class CPU {
        friend struct CpuTest;   // accès aux membres privés pour les tests unitaires
    public :
        CPU(Memory& memory)
            : memory(memory),
              isRunning(true),
              PC(0x0100),            // point d'entrée après le boot ROM
              SP(0xFFFE),            // valeur d'init
              AF(A, F), BC(B, C), DE(D, E), HL(H, L)   // voir #4 et #15
        {}
        auto run() -> void;
        using Op = Cycles (*)(CPU&);

    private :
        Cycles run_opcode(u8 opcode) ;
        Memory& memory ;

        auto fetch8()  -> u8;
        auto fetch16() -> u16;
        auto fetch_s8()-> s8;
        static const std::array<Op, 256> opcode_table;
        static const std::array<Op, 256> cb_table;
        u16 PC ;//Program Counter
        Register16 SP ; //Stack Pointer
        void push_stack(u16 value) ;
        auto pop_stack16() -> u16;
        auto pop_stack8() ->u8;

        //registers
        Register A,B,C,D,E,H,L ;
        FlagRegister F;
        RegisterPair AF, BC, DE, HL;

        // Timer gestion
        bool isRunning;
        bool IME = false ;
        bool is_stopped = false ;
        auto handle_interrupts()-> Cycles ;

        //OPCodes
        auto _opcode_adc(u8 value)->Cycles;
        auto opcode_adc_HL()->Cycles;
        auto opcode_adc_n8(u8 value)->Cycles;

        auto _opcode_add(u8 reg, u8 value)->Cycles;
        auto opcode_add_A_HL()->Cycles;
        auto opcode_add_A_n8(u8 value, u8 reg)->Cycles;

        auto _opcode_ld(Register& R1, Register& R2)->Cycles;
        auto opcode_ld_n8(u8 n, Register& R)->Cycles;
        auto opcode_ld_n16(u16 n, RegisterPair& R)->Cycles;
        auto opcode_ld_HL(Register& R)->Cycles;
        auto opcode_ld_HL_n8(RegisterPair& R)->Cycles;
        auto opcode_ld_HL_r8(Register& R)->Cycles;
        auto opcode_ld_A(RegisterPair& R)->Cycles;
        auto opcode_ld_n16_A(const u16 adress)->Cycles;

        auto opcode_ldh_n16_A(const u16 adress)->Cycles;
        auto opcode_ldh_C_A()->Cycles;

        auto opcode_ld_A_r16(RegisterPair& R)->Cycles;
        auto opcode_ld_A_n16(const u16 adress)->Cycles;

        auto opcode_ldh_A_n16(const u16 adress)->Cycles;
        auto opcode_ldh_A_C()->Cycles;

        auto opcode_ld_A_HLI()->Cycles;
        auto opcode_ld_A_HLD()->Cycles;
        auto opcode_ld_HLI_A()->Cycles;
        auto opcode_ld_HLD_A()->Cycles;

        auto opcode_ld_SP_n16(const u16 n)->Cycles;
        auto opcode_ld_n16_SP(const u16 adress)->Cycles;
        auto opcode_ld_HL_SP_s8(s8 n)->Cycles;

        auto opcode_SP_HL()->Cycles;

        auto opcode_nop()->Cycles;

        auto opcode_or_A_r8(Register& R)->Cycles;
        auto opcode_or_A_HL()->Cycles;
        auto opcode_or_A_n8(u8 n)->Cycles;

        auto opcode_pop_AF()->Cycles;
        auto opcode_pop_r16(RegisterPair& R)->Cycles;

        auto opcode_push_AF()->Cycles;
        auto opcode_push_r16(RegisterPair& R)->Cycles;

        auto opcode_res_u3_r8(Register& R)->Cycles;
        auto opcode_res_u3_HL()->Cycles;
        auto opcode_ret()->Cycles;
        auto opcode_ret_cc()->Cycles;

        auto opcode_DI()->Cycles;
        
        auto opcode_STOP()->Cycles;
};
