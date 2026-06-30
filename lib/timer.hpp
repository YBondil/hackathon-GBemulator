#pragma once
#include "definitions.hpp"
#include "registers.hpp"

class Timer {
    public :
        Timer(){};
        void tick(Cycles cycle) ;
        void write(u16 adress, u8 value) ;
        auto read(u16 adress) -> u8 ;

        auto take_interrupt()->bool ;

    private :
        auto tima_period() const -> int ;

        int divider_accumulator = 0 ;
        int cycle_accumulator = 0 ;
        bool interrupt_request = false ;

        Register divider_register ;
        Register timer_counter ;
        Register timer_modulo ;
        Register timer_control ;

};
