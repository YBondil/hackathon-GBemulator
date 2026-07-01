#include "CPU/CPU.hpp"
#include "bitwise.hpp"

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


Cycles CPU::opcode_add_A_HL(){
    u8 value = memory.read(HL.value());
    u8 reg = memory.read(PC);
    _opcode_add(value,reg);

    PC += 1;

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_add_A_n8(u8 value, u8 reg){
    _opcode_add(value,reg);

    Cycles cycles(2);
    return cycles;
}






/*Loadouts*/

Cycles CPU::_opcode_ld(Register& R1, Register& R2){
    R2.set(R1.value());

    PC += 1;

    Cycles cycles(1);
    return cycles;
}


Cycles CPU::opcode_ld_n8(u8 n, Register& R){
    R.set(n);

    PC += 2;

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_n16(u16 n, RegisterPair& R){
    R.set(n);

    PC += 3;

    Cycles cycles(3);
    return cycles;
}


Cycles CPU::opcode_ld_HL(Register& R){
    memory.write(HL.value(), R.value());

    PC += 1;

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_HL_n8(RegisterPair& R){
    memory.write(HL.value(), R.value());

    PC += 2;

    Cycles cycles(3);
    return cycles;
}


Cycles CPU::opcode_ld_HL_r8(Register& R){
    R.set(memory.read(HL.value()));

    PC += 1;

    Cycles cycles(2);
    return cycles;

}


Cycles CPU::opcode_ld_A(RegisterPair& R){
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


Cycles CPU::opcode_ld_A_r16(RegisterPair& R){
    A.set(memory.read(R.value()));

    PC += 1;

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_A_n16(const u16 adress){
    A.set(memory.read(adress));

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
    u8 down = SP.low();
    u8 up = SP.high();
    memory.write(adress, down);
    memory.write(adress + 1, up);

    PC += 3;

    Cycles cycles(5);
    return cycles;
}


Cycles CPU::opcode_ld_HL_SP_s8(s8 n){
    u16 adress = SP.value() + n;
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


Cycles CPU::opcode_SP_HL(){
    SP.set(HL.value());

    PC += 1;

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_nop(){
    PC += 1;

    Cycles cycles(1);
    return cycles;
}


Cycles CPU::opcode_or_A_r8(Register& R){
    u8 R_value = R.value();
    u8 A_value = A.value();
    u8 result = R_value |
     A_value;
    A.set(result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(false);

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_or_A_HL(){
    u8 value = memory.read(HL.value());
    u8 A_value = A.value();
    u8 result = value | A_value;
    A.set(result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(false);

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_or_A_n8(u8 n){
    u8 A_value = A.value();
    u8 result = n | A_value;
    A.set(result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(false);

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_pop_AF(){
    u16 value = pop_stack16();
    u8 low = static_cast<u8>(value) ;
    AF.set(value);

    F.set_flag_zero(bitwise::check_bit(low, 7));
    F.set_flag_subtract(bitwise::check_bit(low,6));
    F.set_flag_half_carry(bitwise::check_bit(low,5));
    F.set_flag_carry(bitwise::check_bit(low,4));

    Cycles cycles(3);
    return cycles;
}


Cycles CPU::opcode_pop_r16(RegisterPair& R){
    u16 value = pop_stack16();
    R.set(value);

    Cycles cycles(3);
    return cycles;
}


Cycles CPU::opcode_push_AF(){
    push_stack(AF.value());

    Cycles cycles(4);
    return cycles;
}


Cycles CPU::opcode_push_r16(RegisterPair& R){
    push_stack(R.value());

    Cycles cycles(4);
    return cycles;
}


Cycles CPU::opcode_res_u3_r8(u8 u, Register& R){
    u8 value = R.value();
    bitwise::set_bit_to(value, u, false);
    R.set(value);
    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_res_u3_HL(u8 u){
    u8 value = HL.value();
    bitwise::set_bit_to(value, u, false);
    HL.set(value);

    Cycles cycles(4);
    return cycles;
}


Cycles CPU::opcode_ret(){
    PC = pop_stack16();

    Cycles cycles(4);
    return cycles;
}


Cycles CPU::opcode_ret_cc(Condition_code cc){
    if(check_condition(cc)){
        opcode_ret();
        return Cycles(5);
    };
    return Cycles(2);
}


Cycles CPU::opcode_ei(){
    latent_enable = true;

    Cycles cycles(1);
    return cycles;
}


Cycles CPU::opcode_reti(){
    opcode_ret();
    opcode_ei();

    Cycles cycles(4);
    return cycles;
}


Cycles CPU::opcode_rl_r8(Register& R){
    u8 R_value = R.value();
    u8 new_carry = bitwise::bit_value(R_value, 7);
    if (F.flag_carry()){
        u8 result = (R_value << 1) | 0x01;
        R.set(result);
        F.set_flag_zero(result==0);
    }
    else{
        u8 result = (R_value << 1) & 0xFE;
        R.set(result);
        F.set_flag_zero(result==0);
    }
    
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);
    
    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_rl_HL(){
    u8 HL_value = memory.read(HL.value());
    u8 new_carry = bitwise::bit_value(HL_value, 7);
    if (F.flag_carry()){
        u8 result = (HL_value << 1) | 0x01;
        memory.write(HL.value(), result);
        F.set_flag_zero(result==0);
    }
    else{
        u8 result = (HL_value << 1) & 0xFE;
        memory.write(HL.value(), result);
        F.set_flag_zero(result==0);
    }

    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);
    
    Cycles cycles(4);
    return cycles;
}


Cycles CPU::opcode_rl_A(){
    u8 A_value = A.value();
    u8 new_carry = bitwise::bit_value(A_value, 7);
    if (F.flag_carry()){
        u8 result = (A_value << 1) | 0x01;
        A.set(result);
    }
    else{
        u8 result = (A_value << 1) & 0xFE;
        A.set(result);
    }

    F.set_flag_zero(false);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);
    
    Cycles cycles(1);
    return cycles;
}



Cycles CPU::opcode_rlc_r8(Register& R){
    u8 R_value = R.value();
    u8 new_carry = bitwise::bit_value(R_value, 7);
    u8 result = bitwise::set_bit_to(R_value << 1, 0, bitwise::check_bit(new_carry,0));
    R.set(result);
    
    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);
    
    Cycles cycles(2);
    return cycles;
}    


Cycles CPU::opcode_rlc_HL(){
    u8 HL_value = memory.read(HL.value());
    u8 new_carry = bitwise::bit_value(HL_value, 7);
    u8 result = bitwise::set_bit_to(HL_value << 1, 0, bitwise::check_bit(new_carry,0));
    memory.write(HL.value(), result);
    
    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);

    Cycles cycles(4);
    return cycles;
}    


Cycles CPU::opcode_rlc_A(){
    u8 A_value = A.value();
    u8 new_carry = bitwise::bit_value(A_value, 7);
    u8 result = bitwise::set_bit_to(A_value << 1, 0, bitwise::check_bit(new_carry,0));
    A.set(result);
    
    F.set_flag_zero(false);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);
    
    Cycles cycles(1);
    return cycles;
}


Cycles CPU::opcode_rr_r8(Register& R){
    u8 R_value = R.value();
    u8 new_carry = bitwise::bit_value(R_value, 0);
    if (F.flag_carry()){
        u8 result = (R_value >> 1) & 0xFF;
        R.set(result);
        F.set_flag_zero(result==0);
    }
    else {
        u8 result = (R_value >> 1) & 0x7F;
        R.set(result);
        F.set_flag_zero(result==0);
    }
    
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);
    
    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_rr_HL(){
    u8 HL_value = memory.read(HL.value());
    u8 new_carry = bitwise::bit_value(HL_value, 0);
    if (F.flag_carry()){
        u8 result = (HL_value >> 1) & 0xFF;
        memory.write(HL.value(), result);
        F.set_flag_zero(result==0);
    }
    else {
        u8 result = (HL_value >> 1) & 0x7F;
        memory.write(HL.value(), result);
        F.set_flag_zero(result==0);
    }

    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);
    
    Cycles cycles(4);
    return cycles;
}


Cycles CPU::opcode_rr_A(){
    u8 A_value = A.value();
    u8 new_carry = bitwise::bit_value(A_value, 0);
    if (F.flag_carry()){
        u8 result = (A_value >> 1) & 0xFF;
        A.set(result);
    }
    else {
        u8 result = (A_value >> 1) & 0x7F;
        A.set(result);
    }

    F.set_flag_zero(false);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);
    
    Cycles cycles(1);
    return cycles;
}


Cycles CPU::opcode_rrc_r8(Register& R){
    u8 R_value = R.value();
    u8 new_carry = bitwise::bit_value(R_value, 0);
    u8 result = bitwise::set_bit_to(R_value >> 1, 7, bitwise::check_bit(new_carry,0));
    R.set(result);
    
    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);
    
    Cycles cycles(2);
    return cycles;
}    


Cycles CPU::opcode_rrc_HL(){
    u8 HL_value = memory.read(HL.value());
    u8 new_carry = bitwise::bit_value(HL_value, 0);
    u8 result = bitwise::set_bit_to(HL_value >>1, 7, bitwise::check_bit(new_carry,0));
    memory.write(HL.value(), result);
    
    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);
    
    Cycles cycles(4);
    return cycles;
}    


Cycles CPU::opcode_rrc_A(){
    u8 A_value = A.value();
    u8 new_carry = bitwise::bit_value(A_value, 0);
    u8 result = bitwise::set_bit_to(A_value >> 1, 7, bitwise::check_bit(new_carry,0));
    A.set(result);
    
    F.set_flag_zero(false);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);
    
    Cycles cycles(1);
    return cycles;
}


Cycles CPU::opcode_call(u16 adress){
    adress_call = adress;
    latent_call = true;

    Cycles cycles(6);
    return cycles;
}


Cycles CPU::opcode_call_cc(u16 adress, Condition_code cc){
   if(check_condition(cc)){
    return opcode_call(adress);
   }
   return Cycles(3);
}


Cycles CPU::opcode_rst_vec(u16 vec){
    opcode_call(vec);
    Cycles cycles(4);
    return cycles;
}


Cycles CPU::opcode_scf(){
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(true);

    Cycles cycles(1);
    return cycles;
}


Cycles CPU::opcode_set_u3_r8(u8 u, Register& R){
    u8 value = R.value();
    bitwise::set_bit_to(value, u, true);
    R.set(value);
    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_set_u3_HL(u8 u){
    u8 value = HL.value();
    bitwise::set_bit_to(value, u, true);
    HL.set(value);

    Cycles cycles(4);
    return cycles;
}


Cycles CPU::opcode_sla_r8(Register& R){
    u8 R_value = R.value();
    u8 new_carry = bitwise::bit_value(R_value, 7);
    u8 result = (R_value << 1) & 0xFE;
    R.set(result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);
}


