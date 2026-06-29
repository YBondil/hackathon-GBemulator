#include "CPU/registers.hpp"

void Register::reset(){
    val = 0x0 ;
}

auto Register::value() const -> u8{
    return val;
}

void Register::set(u8 new_value){
    val = new_value ;
}

void Register::increment(){
    val++ ;
}

void Register::decrement(){
    val--;
}

auto Register::operator==(u8 other)const -> bool{
    return other == val;
}

void FlagRegister::set(u8 new_value){
    val = new_value & 0xF0;
}


void FlagRegister::set_flag_zero(bool set){

};
void FlagRegister::set_flag_subtract(bool set){};
void FlagRegister::set_flag_half_carry(bool set){};
void FlagRegister::set_flag_carry(bool set){};

auto FlagRegister::flag_zero() const -> bool{};
auto FlagRegister::flag_subtract() const -> bool{};
auto FlagRegister::flag_half_carry() const -> bool{};
auto FlagRegister::flag_carry() const -> bool{};
auto FlagRegister::flag_zero_value() const -> u8{};
auto FlagRegister::flag_subtract_value() const -> u8{};
auto FlagRegister::flag_half_carry_value() const -> u8{};
auto FlagRegister::flag_carry_value() const -> u8{};
