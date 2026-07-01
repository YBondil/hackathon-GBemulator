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
              memory(*this),
              ppu(*this),
              cpu(memory),
              ROM(rom)
        {}

        void run() { cpu.run(); }

    private:
        Timer  timer;
        Memory memory;
        PPU    ppu;
        CPU    cpu;
        std::vector<u8> ROM ;

        friend class Memory;
        friend class Timer;
};
