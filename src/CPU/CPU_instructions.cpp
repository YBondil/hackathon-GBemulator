#include "CPU.hpp"

/*ADC*/
Cycles CPU::_opcode_adc(u8 value){
    u8 reg = A.value();
    u8 carry = F.flag_carry_value();

    u16 result_tot = reg + value + carry;
    u8 result = static_cast<u8>(result_tot);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry((reg & 0xf) + (value & 0xf) + carry > 0xf);
    F.set_flag_carry(result_tot > 0xff);
    A.set(result);

    PC += 1;             //décalage de 1 byte

    Cycles cycles(1);
    return cycles;

}
Cycles CPU::opcode_adc_HL(){
    u8 value = memory.read(HL.value());
    _opcode_adc(value);

    PC += 1;

    Cycles cycles(2);
    return cycles;
}
Cycles CPU::opcode_adc_n8(u8 value){
    _opcode_adc(value);

    PC += 2;           //Décalage de 2 bytes

    Cycles cycles(2);
    return cycles;    
}


/*ADD*/

Cycles CPU::_opcode_add(u8 reg, u8 value){
    u8 result = reg + value;
    A.set(static_cast<u8>(result));

    F.set_flag_zero(A.value()==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry((reg & 0xf) + (value & 0xf)> 0xf);
    F.set_flag_carry((result & 0x100) != 0);

    PC += 1;

    Cycles cycles(1);
    return cycles;
}


Cycles CPU::opcode_add_HL(){
    u8 value = memory.read(HL.value());
    u8 reg = memory.read(PC);
    _opcode_add(value,reg);

    PC += 1;

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_add_n8(u8 value, u8 reg){
    _opcode_add(value,reg);

}






/*Loadouts*/

Cycles CPU::_opcode_ld(Register R1, Register R2){
    R2.set(R1.value());

    PC += 1;

    Cycles cycles(1);
    return cycles;
}


Cycles CPU::opcode_ld_n8(u8 n, Register R){
    R.set(n);

    PC += 2;

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_n16(u16 n, RegisterPair R){
    R.set(n);

    PC += 3;

    Cycles cycles(3);
    return cycles;
}


Cycles CPU::opcode_ld_HL(Register R){
    memory.write(HL.value(), R.value());

    PC += 1;

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_HL_n8(RegisterPair R){
    memory.write(HL.value(), R.value()); 
      
    PC += 2;

    Cycles cycles(3);  
    return cycles;
}


Cycles CPU::opcode_ld_HL_r8(Register R){
    R.set(memory.read(HL.value()));

    PC += 1;

    Cycles cycles(2);
    return cycles;

}


Cycles CPU::opcode_ld_A(RegisterPair R){
    memory.write(R.value(), A.value());

    PC += 1;

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_n16_A(const u16 adress){
    memory.write(adress, A.value());

    PC += 3;

    Cycles cycles(4);
    return cycles;
}


Cycles CPU::opcode_ldh_n16_A(const u16 adress){             //Manque l'encadrement des valeurs de adress
    memory.write(adress, A.value());

    PC += 2;

    Cycles cycles(3); 
    return cycles;
}


Cycles CPU::opcode_ldh_C_A(){
    u16 adress = 0xFF00 + static_cast<u16>(C.value());
    memory.write(adress, A.value());

    PC += 1;
    
    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_A_r16(RegisterPair R){
    A.set(memory.read(R.value()));

    PC += 1;

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_A_n16(const u16 adress){
    A.set(adress);

    PC += 3;

    Cycles cycles(4);
    return cycles;
}


Cycles CPU::opcode_ldh_A_n16(const u16 adress){                 //Manque l'encadrement des valeurs de adress
    A.set(adress);

    PC += 3;

    Cycles cycles(4);
    return cycles;
}

Cycles CPU::opcode_ldh_A_C(){
    u16 adress = 0xFF00 + static_cast<u16>(C.value());
    A.set(memory.read(adress));

    PC += 1;
    
    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_HLI_A(){
    memory.write(HL.value(),A.value());
    HL.increment();
    
    PC += 1;
    
    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_HLD_A(){
    memory.write(HL.value(),A.value());
    HL.decrement();
    
    PC += 1;
    
    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_A_HLI(){
    A.set(memory.read(HL.value()));
    HL.increment();
    
    PC += 1;
    
    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_A_HLD(){
    A.set(memory.read(HL.value()));
    HL.decrement();
    
    PC += 1;
    
    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_SP_n16(const u16 n){
    SP.set(n);

    PC += 3;

    Cycles cycles(3);
    return cycles;
}


Cycles CPU::opcode_ld_n16_SP(const u16 adress){
    u8 down = SP & 0xFF;
    u8 up = SP >> 8;
    memory.write(adress, down);
    memory.write(adress + 1, up);

    PC += 3;

    Cycles cycles(5);
    return cycles;
}


Cycles CPU::opcode_ld_HL_SP_s8(s8 n){
    u16 adress = SP.valeur() + n;
    u8 value = memory.read(adress);
    memory.write(HL.value(), value);

    F.set_flag_zero(false);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(value & 0xF > 0xF);
    F.set_flag_carry(value > 0xFF);
    
    
    PC += 2;

    Cycles cycles(3);
    return cycles;
}
