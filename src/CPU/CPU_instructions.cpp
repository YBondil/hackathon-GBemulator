#include "CPU/CPU.hpp"


/*ADC*/

Cycles CPU::_opcode_adc(u8 value){
    u8 reg = A.value();
    u8 carry = F.flag_carry_value();

    u16 result_tot = reg + value + carry;
    u8 result = static_cast<u8>(result_tot);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry((reg & 0xf) + (value & 0xf) + carry > 0xf);
    F.set_flag_carry(result > 0xff);

    A.set(result);

    PC +=8;             //décalage de 1 byte
    
    Cycles cycles(1);
    return cycles;

}


Cycles CPU::opcode_adc_HL(){
    u8 value = memory.read(HL.value());
    _opcode_adc(value);

    PC +=8;

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_adc_n8(u8 value){
    _opcode_adc(value);

    PC += 16;           //Décalage de 2 bytes

    Cycles cycles(2);
    return cycles;    
}

