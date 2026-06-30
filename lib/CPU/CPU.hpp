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
        u16 SP ; //Stack Pointer
        //registers
        Register A,B,C,D,E,H,L ;
        FlagRegister F;
        RegisterPair AF, BD, DE, HL;


        // clock ?


        //OPCodes


        auto _opcode_adc(u8 value)->Cycles;
        auto opcode_adc_HL()->Cycles;
        auto opcode_adc_n8(u8 value)->Cycles;

        auto _opcode_add(u8 reg, u8 value)->Cycles;
        auto opcode_add_HL()->Cycles;
        auto opcode_add_n8(u8 value, u8 reg)->Cycles;

        auto _opcode_ld(Register R1, Register R2)->Cycles;
        auto opcode_ld_n8(u8 n, Register R)->Cycles;
        auto opcode_ld_n16(u16 n, RegisterPair R)->Cycles;
        auto opcode_ld_HL(Register R)->Cycles;
        auto opcode_ld_HL_n8(RegisterPair R)->Cycles;
        auto opcode_ld_HL_r8(Register R)->Cycles;
        auto opcode_ld_A(RegisterPair R)->Cycles;
        auto opcode_ld_n16_A(const u16 adress)->Cycles;

        auto opcode_ldh_n16_A(const u16 adress)->Cycles;
        auto opcode_ldh_C_A()->Cycles;
        
        auto opcode_ld_A_r16(RegisterPair R)->Cycles;
        auto opcode_ld_A_n16(const u16 adress)->Cycles;

        auto opcode_ldh_A_n16(const u16 adress)->Cycles;
        auto opcode_ldh_A_C()->Cycles;
        
        auto opcode_ld_A_HLI()->Cycles;
        auto opcode_ld_A_HLD()->Cycles;
        auto opcode_ld_HLI_A()->Cycles;
        auto opcode_ld_HLD_A()->Cycles;

        auto opcode_ld_SP_n16(const u16 n)->Cycles;

};
