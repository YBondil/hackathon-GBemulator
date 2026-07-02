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
    Cycles cycles(1);
    return cycles;

}


Cycles CPU::opcode_adc_HL(){
    u8 value = memory.read(HL.value());
    _opcode_adc(value);
    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_adc_n8(u8 value){
    _opcode_adc(value);
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
    _opcode_add(A.value(), value);     // A = A + mem[HL]
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
    Cycles cycles(1);
    return cycles;
}


Cycles CPU::opcode_ld_n8(u8 n, Register& R){
    R.set(n);

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_n16(u16 n, RegisterPair& R){
    R.set(n);

    Cycles cycles(3);
    return cycles;
}


Cycles CPU::opcode_ld_HL(Register& R){
    memory.write(HL.value(), R.value());

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_ld_HL_n8(RegisterPair& R){
    memory.write(HL.value(), R.value());

    Cycles cycles(3);
    return cycles;
}


Cycles CPU::opcode_ld_HL_r8(Register& R){
    R.set(memory.read(HL.value()));

    Cycles cycles(2);
    return cycles;

}


Cycles CPU::opcode_ld_A(RegisterPair& R){
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
    A.set(adress);

    Cycles cycles(4);
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


Cycles CPU::opcode_SP_HL(){
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
    u8 result = R_value || A_value;
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
    u8 result = value || A_value;
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
    u8 result = n || A_value;
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

    F.set_flag_zero(bitwise::check_bit(value, 7));
    F.set_flag_subtract(bitwise::check_bit(value,6));
    F.set_flag_half_carry(bitwise::check_bit(value,5));
    F.set_flag_carry(bitwise::check_bit(value,4));

    Cycles cycles(3);
    return cycles;
}


Cycles CPU::opcode_pop_r16(RegisterPair& R){
    u16 value = pop_stack16();
    u8 low = static_cast<u8>(value) ;
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


Cycles CPU::opcode_res_u3_r8(Register& R){
    u8 value = R.value();
    R.set(value &= 0XF8);

    Cycles cycles(2);
    return cycles;
}


Cycles CPU::opcode_res_u3_HL(){
    u8 value = HL.value();
    HL.set(value &= 0XF8);

    Cycles cycles(4);
    return cycles;
}


Cycles CPU::opcode_ret(){
    pop_stack8();
    PC += 1;
    pop_stack8();
    PC += 1;
}


/* Cycles CPU::opcode_ret_cc(){
    if()
} */

Cycles CPU::opcode_DI(){
    IME = false;

    Cycles cycles(1);
    return cycles;
}

auto CPU::opcode_STOP() -> Cycles {

    u8 next_byte = fetch8(); 
    isStopped = true;

    u8 lcdc = memory.read(0xFF40);
    memory.write(0xFF40, lcdc & 0x7F); //éteindre l'écran en coupant le bit 7 du registre LCDC (0xFF40)

    return Cycles(1);
}