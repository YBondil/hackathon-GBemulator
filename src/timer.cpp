#include "timer.hpp"
#include <iostream>


namespace {
    constexpr u16 ADDR_DIV  = 0xFF04;
    constexpr u16 ADDR_TIMA = 0xFF05;
    constexpr u16 ADDR_TMA  = 0xFF06;
    constexpr u16 ADDR_TAC  = 0xFF07;

    constexpr int DIV_PERIOD = 256;   // T-cycles par incrément de DIV
    constexpr int TAC_ENABLE_BIT = 2; // bit "timer activé" dans TAC
}

auto Timer::tima_period() const -> int {
    switch (timer_control.value() & 0x03){
        case 0b00: return 1024;
        case 0b01: return 16;
        case 0b10: return 64;
        case 0b11: return 256;
    }
    return 1024;
}

void Timer::tick(Cycles cycle){
    const int n = cycle.value();

    divider_accumulator += n ;
    while(divider_accumulator >= DIV_PERIOD){
        divider_accumulator -= DIV_PERIOD;
        divider_register.increment();
    }

    if (timer_control.checkBit(TAC_ENABLE_BIT)) {
            cycle_accumulator += n;
            const int period = tima_period();
            while (cycle_accumulator >= period) {
                cycle_accumulator -= period;
                if (timer_counter.value() == 0xFF) {
                    timer_counter.set(timer_modulo.value());
                    interrupt_request = true;
                } else {
                    timer_counter.increment();
                }
            }
        }

}


void Timer::write(u16 adress, u8 value) {
    switch (adress) {
        case ADDR_DIV:
            divider_register.set(0);
            divider_accumulator = 0;
            break;
        case ADDR_TIMA: timer_counter.set(value); break;
        case ADDR_TMA:  timer_modulo.set(value);  break;
        case ADDR_TAC:  timer_control.set(value & 0x07); break;
        default :
            std::cout << "MAUVAIS ADDRESSAGE À L'ADRESSE " << std::hex << adress << std::endl ;
            break ;
    }
}

auto Timer::read(u16 adress) -> u8 {
    switch (adress) {
        case ADDR_DIV:  return divider_register.value();
        case ADDR_TIMA: return timer_counter.value();
        case ADDR_TMA:  return timer_modulo.value();
        case ADDR_TAC:  return timer_control.value() | 0xF8;
        default :
            std::cout << "MAUVAIS ADDRESSAGE À L'ADRESSE " << std::hex << adress << std::endl ;
            return 0xFF ;
    }
}


auto Timer::take_interrupt() -> bool {
    const bool fired = interrupt_request;
    interrupt_request = false;
    return fired;
}
