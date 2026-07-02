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

    Cycles cycles(1);
    return cycles;
}


Cycles CPU::opcode_add_A_HL(){
    u8 value = memory.read(HL.value());
    u8 reg = memory.read(PC);
    _opcode_add(value,reg);

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_add_A_n8(u8 value, u8 reg){
    _opcode_add(value,reg);

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_add_HL_SP(){
    HL.set(SP.value());

    F.set_flag_subtract(false);
    F.set_flag_half_carry(SP.value() > 0x800);
    F.set_flag_carry(SP.value() > 0x8000);

    Cycles cycles(2);
    return cycles;
}

Cycles CPU::opcode_add_SP_s8(s8 n){
    SP.set(n);

    F.set_flag_zero(false);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(SP.value() > 0xf);
    F.set_flag_carry(SP.value() > 0xff);

    Cycles cycles(4);
    return cycles;
}


/*AND*/

Cycles CPU::opcode_and_A_r8(Register& R){
    u8 R_value = R.value();
    u8 A_value = A.value();
    u8 result = R_value && A_value;
    A.set(result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(true);
    F.set_flag_carry(false);

    Cycles cycles(1);
    return cycles;
}


Cycles CPU::opcode_and_A_HL(){
    u8 value = memory.read(HL.value());
    u8 A_value = A.value();
    u8 result = value && A_value;
    A.set(result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(true);
    F.set_flag_carry(false);

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_and_A_n8(u8 n){
    u8 A_value = A.value();
    u8 result = n && A_value;
    A.set(result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(true);
    F.set_flag_carry(false);

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_bit_u3_r8(u8 bit, Register& R){
    u8 value = R.value();
    bool bit_set = bitwise::check_bit(value, bit);

    F.set_flag_zero(!bit_set);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(true);

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_bit_u3_HL(u8 bit){
    u8 value = memory.read(HL.value());
    bool bit_set = bitwise::check_bit(value, bit);

    F.set_flag_zero(!bit_set);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(true);

    Cycles cycles(3);
    return cycles;
}


Cycles CPU:: opcode_ccf(){
    F.set_flag_subtract(false);
    F.set_flag_half_carry(true);
    F.set_flag_carry(!F.flag_carry());

    Cycles cycles(1);
    return cycles;
}


Cycles CPU::opcode_cpl(){
    u8 A_value = A.value();
    u8 result = ~A_value;   //Vérifier si ~A_value existe bien ; peut-être doit être passé en argument à la fonction
    A.set(result);

    F.set_flag_subtract(true);
    F.set_flag_half_carry(true);

    Cycles cycles(1);
    return cycles;
}


Cycles CPU::opcode_daa(){
    u8 A_value = A.value();
    u8 correction = 0;
    bool set_carry = false;

    if(!F.flag_subtract()){
        if(F.flag_half_carry() || (A_value & 0x0F) > 9){
            correction |= 0x06;
        }
        if(F.flag_carry() || A_value > 0x99){
            correction |= 0x60;
            set_carry = true;
        }
        A.set(A_value + correction);
    }

    else{
        if(F.flag_half_carry()){
            correction |= 0x06;
        }
        if(F.flag_carry()){
            correction |= 0x60;
            set_carry = true;
        }
        A.set(A_value - correction);
    }

    F.set_flag_zero(A.value() == 0);
    F.set_flag_half_carry(false);
    F.set_flag_carry(set_carry);

    Cycles cycles(1);
    return cycles;
}


/*DEC*/

Cycles CPU::opcode_dec_r8(Register& R){
    u8 value = R.value();
    u8 result = value - 1;
    R.set(result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(true);
    F.set_flag_half_carry((value & 0xf) == 0);

    Cycles cycles(1);
    return cycles;
}


Cycles CPU::opcode_dec_SP(){
    u8 value = memory.read(SP.value());
    u8 result = value - 1;
    memory.write(SP.value(), result);

    Cycles cycles(2);
    return cycles;
}


/*INC*/

Cycles CPU::opcode_inc_r8(Register& R){
    u8 value = R.value();
    u8 result = value + 1;
    R.set(result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry((value & 0xf) + 1 > 0xf);

    Cycles cycles(1);
    return cycles;
}

Cycles CPU::opcode_inc_HL(){
    u8 value = memory.read(HL.value());
    u8 result = value + 1;
    memory.write(HL.value(), result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry((value & 0xf) + 1 > 0xf);

    Cycles cycles(3);
    return cycles;
}

Cycles CPU::opcode_inc_r16(RegisterPair& R){
    u16 value = R.value();
    u16 result = value + 1;
    R.set(result);

    Cycles cycles(2);
    return cycles;
}

Cycles CPU::opcode_inc_SP(){
    u8 value = memory.read(SP.value());
    u8 result = value + 1;
    memory.write(SP.value(), result);

    Cycles cycles(2);
    return cycles;
}


/*JP*/


Cycles CPU::opcode_jp_HL(){
    u16 adress = HL.value();
    PC = adress;

    Cycles cycles(1);
    return cycles;
}


Cycles CPU::opcode_jp_n16(u16 adress){
    PC = adress;

    Cycles cycles(4);
    return cycles;
}


Cycles CPU::opcode_jp_cc_n16(u16 adress, Condition_code cc){
    if(check_condition(cc)){
        return opcode_jp_n16(adress);
    }
    else{
        Cycles cycles(3);
        return cycles;
    }
}


/*Loadouts*/

Cycles CPU::opcode_ld_r8_r8(Register& R1, Register& R2){
    R2.set(R1.value());

    Cycles cycles(1);
    return cycles;
}


Cycles CPU::opcode_ld_r8_n8(u8 n, Register& R){
    R.set(n);

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_r16_n16(u16 n, RegisterPair& R){
    R.set(n);

    Cycles cycles(3);
    return cycles;
}


Cycles CPU::opcode_ld_HL_r8(Register& R){
    memory.write(HL.value(), R.value());

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_HL_n8(RegisterPair& R){
    memory.write(HL.value(), R.value());

    Cycles cycles(3);
    return cycles;
}


Cycles CPU::opcode_ld_r8_HL(Register& R){
    R.set(memory.read(HL.value()));

    Cycles cycles(2);
    return cycles;

}


Cycles CPU::opcode_ld_r16_A(RegisterPair& R){
    memory.write(R.value(), A.value());

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_n16_A(const u16 adress){
    memory.write(adress, A.value());

    Cycles cycles(4);
    return cycles;
}


Cycles CPU::opcode_ldh_n16_A(const u16 adress){             //Manque l'encadrement des valeurs de adress
    memory.write(adress, A.value());

    Cycles cycles(3);
    return cycles;
}


Cycles CPU::opcode_ldh_C_A(){
    u16 adress = 0xFF00 + static_cast<u16>(C.value());
    memory.write(adress, A.value());

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_A_r16(RegisterPair& R){
    A.set(memory.read(R.value()));

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_A_n16(const u16 adress){
    A.set(memory.read(adress));

    Cycles cycles(4);
    return cycles;
}


Cycles CPU::opcode_ldh_A_n16(const u16 adress){                 //Manque l'encadrement des valeurs de adress
    A.set(memory.read(adress));

    Cycles cycles(3);
    return cycles;
}

Cycles CPU::opcode_ldh_A_C(){
    u16 adress = 0xFF00 + static_cast<u16>(C.value());
    A.set(memory.read(adress));

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_HLI_A(){
    memory.write(HL.value(),A.value());
    HL.increment();

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_HLD_A(){
    memory.write(HL.value(),A.value());
    HL.decrement();

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_A_HLI(){
    A.set(memory.read(HL.value()));
    HL.increment();

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_A_HLD(){
    A.set(memory.read(HL.value()));
    HL.decrement();

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_SP_n16(const u16 n){
    SP.set(n);

    Cycles cycles(3);
    return cycles;
}


Cycles CPU::opcode_ld_n16_SP(const u16 adress){
    u8 down = SP.low();
    u8 up = SP.high();
    memory.write(adress, down);
    memory.write(adress + 1, up);

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

    Cycles cycles(3);
    return cycles;
}


Cycles CPU::opcode_ld_SP_HL(){
    SP.set(HL.value());

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_nop(){

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
    u8 new_value = bitwise::set_bit_to(value, u, false);
    R.set(new_value);
    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_res_u3_HL(u8 u){
    u8 value = memory.read(HL.value());
    u8 new_value = bitwise::set_bit_to(value, u, false);
    memory.write(HL.value(), new_value);

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
    u8 result = ((R_value << 1) & 0xFE) | F.flag_carry_value();
    R.set(result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_rl_HL(){
    u8 HL_value = memory.read(HL.value());
    u8 new_carry = bitwise::bit_value(HL_value, 7);
    u8 result = ((HL_value << 1) & 0xFE) | F.flag_carry_value();
    memory.write(HL.value(), result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);

    Cycles cycles(4);
    return cycles;
}


Cycles CPU::opcode_rl_A(){
    u8 A_value = A.value();
    u8 new_carry = bitwise::bit_value(A_value, 7);
    u8 result = ((A_value << 1) & 0xFE) | F.flag_carry_value();
    A.set(result);

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
    u8 result = ((R_value >> 1) & 0x7F) | (F.flag_carry_value() <<7);
    R.set(result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_rr_HL(){
    u8 HL_value = memory.read(HL.value());
    u8 new_carry = bitwise::bit_value(HL_value, 0);
    u8 result = ((HL_value >> 1)  & 0x7F) | (F.flag_carry_value() <<7);
    memory.write(HL.value(), result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);

    Cycles cycles(4);
    return cycles;
}


Cycles CPU::opcode_rr_A(){
    u8 A_value = A.value();
    u8 new_carry = bitwise::bit_value(A_value, 0);
    u8 result = ((A_value >> 1) & 0x7F) | (F.flag_carry_value() <<7);
    A.set(result);

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


Cycles CPU::opcode_call_n16(u16 adress){
    adress_call = adress;
    latent_call = true;

    Cycles cycles(6);
    return cycles;
}


Cycles CPU::opcode_call_cc_n16(u16 adress, Condition_code cc){
    if(check_condition(cc)){
        return opcode_call_n16(adress);
    }
    return Cycles(3);
}


Cycles CPU::opcode_rst_vec(u16 vec){
    opcode_call_n16(vec);

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
    u8 new_value = bitwise::set_bit_to(value, u, true);
    R.set(new_value);

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_set_u3_HL(u8 u){
    u8 value = memory.read(HL.value());
    u8 new_value = bitwise::set_bit_to(value, u, true);
    memory.write(HL.value(), new_value);

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

    Cycles cycles(2);
    return cycles;
}

/*XOR*/

Cycles CPU::opcode_xor_A_r8(Register& R){
    u8 R_value = R.value();
    u8 A_value = A.value();
    u8 result = R_value ^ A_value;
    A.set(result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(false);

    Cycles cycles(1);
    return cycles;
}


Cycles CPU::opcode_xor_A_HL(){
    u8 value = memory.read(HL.value());
    u8 A_value = A.value();
    u8 result = value ^ A_value;
    A.set(result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(false);

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_sla_HL(){
    u8 HL_value = memory.read(HL.value());
    u8 new_carry = bitwise::bit_value(HL_value, 7);
    u8 result = (HL_value << 1) & 0xFE;
    memory.write(HL.value(), result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);

    Cycles cycles(4);
    return cycles;
}


Cycles CPU::opcode_xor_A_n8(u8 n){
    u8 A_value = A.value();
    u8 result = n ^ A_value;
    A.set(result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(false);

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_sra_r8(Register& R){
    u8 R_value = R.value();
    u8 new_carry = bitwise::bit_value(R_value, 0);
    u8 b7 = bitwise::bit_value(R_value, 7);
    u8 result = (R_value >> 1) & (bitwise::set_bit_to(0xFF,7,bitwise::check_bit(b7,0)));        //R_value>>1 & ?1111111
    R.set(result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_sra_HL(){
    u8 HL_value = memory.read(HL.value());
    u8 new_carry = bitwise::bit_value(HL_value, 0);
    u8 b7 = bitwise::bit_value(HL_value, 7);
    u8 result = (HL_value >> 1) & (bitwise::set_bit_to(0xFF,7,bitwise::check_bit(b7,0)));
    memory.write(HL.value(), result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);

    Cycles cycles(4);
    return cycles;
}


Cycles CPU::opcode_srl_r8(Register& R){
    u8 R_value = R.value();
    u8 new_carry = bitwise::bit_value(R_value, 0);
    u8 result = (R_value >> 1) & 0x7F;        //R_value>>1 & 01111111
    R.set(result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_srl_HL(){
    u8 HL_value = memory.read(HL.value());
    u8 new_carry = bitwise::bit_value(HL_value, 0);
    u8 result = (HL_value >> 1) & 0x7F;
    memory.write(HL.value(), result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(new_carry);

    Cycles cycles(4);
    return cycles;
}
/* Cycles CPU::opcode_ret_cc(){
    if()
} */



// 8-bit arithmetic instructions (except those starting with A)

Cycles CPU::opcode_cp_a_r8(Register& R){
    const int compare = (A.value()-R.value());

    F.set_flag_zero(compare == 0);
    F.set_flag_subtract(1);
    F.set_flag_half_carry(A.value()%16 < R.value()%16);
    F.set_flag_carry(compare < 0);

    Cycles cycles(1);
    return cycles;

}


Cycles CPU::opcode_cp_a_hl(){
    const u8 value = memory.read(HL.value());
    const int compare = (A.value() - value);

    F.set_flag_zero(compare == 0);
    F.set_flag_subtract(1);
    F.set_flag_half_carry(A.value()%16 < value%16);
    F.set_flag_carry(compare < 0);

    Cycles cycles(2);
    return cycles;

}


Cycles CPU::opcode_cp_a_n8(u8 n8){
    const int compare  = A.value() - n8;

    F.set_flag_zero(compare == 0);
    F.set_flag_subtract(1);
    F.set_flag_half_carry(A.value()%16 < n8%16);
    F.set_flag_carry(compare < 0);

    Cycles cycles(2);
    return cycles;

}

Cycles CPU::opcode_dec_hl(){
    const u8 value = memory.read(HL.value());
    memory.write(HL.value(), value - 1);

    F.set_flag_zero(memory.read(HL.value()) == 0);
    F.set_flag_subtract(1);
    F.set_flag_half_carry(memory.read(HL.value())%16 == 15);

    Cycles cycles(3);
    return cycles;

}


Cycles CPU::opcode_inc_hl(){
    const u8 value = memory.read(HL.value());
    memory.write(HL.value(), value + 1);

    F.set_flag_zero(memory.read(HL.value()) == 0);
    F.set_flag_subtract(0);
    F.set_flag_half_carry(memory.read(HL.value())%16 == 0);

    Cycles cycles(3);
    return cycles;

}





Cycles CPU::opcode_sbc_a_r8(Register& R){
    const int result = A.value() - R.value() - F.flag_carry();
    A.set(result);

    F.set_flag_zero(result == 0);
    F.set_flag_subtract(1);
    F.set_flag_half_carry(A.value()%16 < R.value()%16 + F.flag_carry()%16);
    F.set_flag_carry(result < 0);

    Cycles cycles(1);
    return cycles;

}


Cycles CPU::opcode_sbc_a_hl(){
    const int result = A.value() - memory.read(HL.value()) - F.flag_carry();
    A.set(result);

    F.set_flag_zero(result == 0);
    F.set_flag_subtract(1);
    F.set_flag_half_carry(A.value()%16 < memory.read(HL.value())%16 + F.flag_carry()%16);
    F.set_flag_carry(result < 0);

    Cycles cycles(2);
    return cycles;

}


Cycles CPU::opcode_sbc_a_n8(u8 n8){
    const int result = A.value() - n8 - F.flag_carry();
    A.set(result);

    F.set_flag_zero(result == 0);
    F.set_flag_subtract(1);
    F.set_flag_half_carry(A.value()%16 < n8%16 + F.flag_carry()%16);
    F.set_flag_carry(result < 0);

    Cycles cycles(2);
    return cycles;

}

Cycles CPU::opcode_sub_a_r8(Register& R){
    const int result = A.value() - R.value();
    A.set(result);

    F.set_flag_zero(result == 0);
    F.set_flag_subtract(1);
    F.set_flag_half_carry(A.value()%16 < R.value()%16);
    F.set_flag_carry(result < 0);

    Cycles cycles(1);
    return cycles;

}

Cycles CPU::opcode_sub_a_hl(){
    const int result = A.value() - memory.read(HL.value());
    A.set(result);

    F.set_flag_zero(result == 0);
    F.set_flag_subtract(1);
    F.set_flag_half_carry(A.value()%16 < memory.read(HL.value())%16);
    F.set_flag_carry(result < 0);

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_sub_a_n8(u8 n8){
    const int result = A.value() - n8;
    A.set(result);

    F.set_flag_zero(result == 0);
    F.set_flag_subtract(1);
    F.set_flag_half_carry(A.value()%16 < n8%16);
    F.set_flag_carry(result < 0);

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_swap_r8(Register& R){
    u8 value = R.value();
    u8 low = (value & 0x0F)<<4;
    u8 high = (value & 0xF0)<<4;
    u8 result = low | high;
    R.set(result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(false);
    return Cycles(10) ; //-->Valeur fausse à verifier
}


Cycles CPU::opcode_swap_HL(){
    u8 value = memory.read(HL.value());
    u8 low = (value & 0x0F)<<4;
    u8 high = (value & 0xF0)<<4;
    u8 result = low | high;
    memory.write(HL.value(), result);

    F.set_flag_zero(result==0);
    F.set_flag_subtract(false);
    F.set_flag_half_carry(false);
    F.set_flag_carry(false);

    return Cycles(10) ; //-->Valeur fausse à verifier
}


Cycles CPU::opcode_jr_n16(u8 offset){
    PC += offset;

    Cycles cycles(3);
    return cycles;
}


Cycles CPU::opcode_jr_cc_n16(u8 offset, Condition_code cc){
    if (check_condition(cc)){
        PC += offset;
        return Cycles(3);
    }

    Cycles cycles(2);
    return cycles;
}
