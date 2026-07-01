#pragma once
#include "definitions.hpp"
#include "CPU/CPU.hpp"
#include "video/PPU.hpp"
#include "memory.hpp"
#include "timer.hpp"
#include <vector>


class Gameboy {
    public:
        Gameboy(std::vector<u8> rom)
            : timer(),
              memory(*this, rom),
              ppu(*this),
              cpu(memory)
        {}

        void run() { cpu.run(); }
        Cycles step() { return cpu.step(); }   // une instruction
        u16 pc() const { return cpu.pc(); }

    private:
        Timer  timer;
        Memory memory;
        PPU    ppu;
        CPU    cpu;

        friend class Memory;
        friend class Timer;
};
