#include "CPU/registers.hpp"
#include "bitwise.hpp"

void Register::reset(){
    val = 0x0 ;
}

auto Register::value() const -> u8{
    return val;
}
auto Register::checkBit(u8 bit) const -> bool{
    return bitwise::check_bit(val, bit);
}
void Register::setBitTo(u8 bit, bool set){
    val = bitwise::set_bit_to(val, bit, set);
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

void FlagRegister::set_flag_zero(bool set)      { val = bitwise::set_bit_to(val, 7, set); }
void FlagRegister::set_flag_subtract(bool set)  { val = bitwise::set_bit_to(val, 6, set); }
void FlagRegister::set_flag_half_carry(bool set){ val = bitwise::set_bit_to(val, 5, set); }
void FlagRegister::set_flag_carry(bool set)     { val = bitwise::set_bit_to(val, 4, set); }


auto FlagRegister::flag_zero() const -> bool{return bitwise::check_bit(val,7);};
auto FlagRegister::flag_subtract() const -> bool{return bitwise::check_bit(val,6);};
auto FlagRegister::flag_half_carry() const -> bool{return bitwise::check_bit(val,5);};
auto FlagRegister::flag_carry() const -> bool{return bitwise::check_bit(val,4);};
auto FlagRegister::flag_zero_value() const -> u8{return static_cast<u8>(flag_zero()?1:0);};
auto FlagRegister::flag_subtract_value() const -> u8{return static_cast<u8>(flag_subtract()?1:0);};
auto FlagRegister::flag_half_carry_value() const -> u8{return static_cast<u8>(flag_half_carry()?1:0);};
auto FlagRegister::flag_carry_value() const -> u8{return static_cast<u8>(flag_carry()?1:0);};



void RegisterPair::set(u16 value){
    low_byte.set(static_cast<u8>(value));
    high_byte.set(static_cast<u8>(value >> 8)) ;
}

auto RegisterPair::value()const -> u16{
    return bitwise::compose_bytes(high_byte.value(),low_byte.value()) ;
}

void RegisterPair::increment(){
    set(value()+1);
}

void RegisterPair::decrement(){
    set(value()-1);
}
