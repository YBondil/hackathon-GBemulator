#pragma once
#include "definitions.hpp"


class Register{
    public :
        Register()=default ;

        void reset();
        auto value() const -> u8 ;
        virtual void set(u8 new_value);

        auto checkBit(u8 bit) const -> bool;
        void setBitTo(u8 bit, bool set);

        void increment();
        void decrement();

        auto operator==(u8 other) const -> bool ;
    protected :
        u8 val = 0x0 ;
};

class FlagRegister : Register{

    public :
        FlagRegister() = default;

        void set(u8 new_value) override;
        void set_flag_zero(bool set);
        void set_flag_subtract(bool set);
        void set_flag_half_carry(bool set);
        void set_flag_carry(bool set);

        auto flag_zero() const -> bool;
        auto flag_subtract() const -> bool;
        auto flag_half_carry() const -> bool;
        auto flag_carry() const -> bool;
        auto flag_zero_value() const -> u8;
        auto flag_subtract_value() const -> u8;
        auto flag_half_carry_value() const -> u8;
        auto flag_carry_value() const -> u8;
};

class RegisterPair{
public:
    RegisterPair(Register& high,Register& low): low_byte(low), high_byte(high) {};

    void set(u16 value);

    auto value() const -> u16;

    auto low() const -> u8;
    auto high() const -> u8;

    void increment();
    void decrement();

private:
    Register& low_byte;
    Register& high_byte;
};
