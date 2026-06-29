#include "CPU/CPU.hpp"


/*OPCodes*/




/*ADC*/

void CPU::opcode_adc(u8 value){
    u8 reg = A.value();
    u8 carry = F.flag_carry_value();

    u16 result_tot = reg + value + carry;
    u8 result = static_cast<u8>(result_tot);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry((reg & 0xf) + (value & 0xf) + carry > 0xf);
    F.set_flag_carry(result > 0xff)

}