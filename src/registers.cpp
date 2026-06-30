#include "registers.hpp"
#include "bitwise.hpp"

///Register
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

///FlagRegister
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


/// Register16
void Register16::set(u16 value){
    _value = value ;
}
auto Register16::value() const -> u16{
    return _value;
}
void Register16::increment(){
    set(value()+1);
}
void Register16::decrement(){
    set(value()-1);
}
bool Register16::operator==(u16 other){
    return other==_value ;
}
auto Register16::low() const -> u8{
    return static_cast<u8>(_value) ;
}
auto Register16::high() const -> u8{
    return static_cast<u8>(_value>>8) ;
}

///RegisterPair
RegisterPair::RegisterPair(Register& high, Register& low): Register16(0), high_byte(high), low_byte(low){
    u16 value = bitwise::compose_bytes(high.value(), low.value());
}
void RegisterPair::set(u16 value){
    low_byte.set(static_cast<u8>(value));
    high_byte.set(static_cast<u8>(value >> 8)) ;
    _value = bitwise::compose_bytes(high_byte.value(),low_byte.value()) ;
}

auto RegisterPair::low() const -> u8{
    return low_byte.value() ;
}
auto RegisterPair::high() const -> u8{
    return high_byte.value() ;
}
