#include "CPU.hpp"


void CPU::run(){
    while (isRunning){
        u8 opcode = memory.read(PC) ;
        Cycles cycle = run_opcode(opcode);
        //TODU: répercuter cycle.value() sur le timer / PPU
    }
}

auto run_opcode(u8 opcode) {
    //todo
}
