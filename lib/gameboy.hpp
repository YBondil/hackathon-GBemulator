#pragma once
#include "definitions.hpp"
#include "CPU/CPU.hpp"
#include "video/PPU.hpp"
#include "memory.hpp"
#include "timer.hpp"
#include "boot.hpp"
#include <vector>
#include <iomanip>
#include <iostream>


class Gameboy {
    public:
        Gameboy(std::vector<u8> rom, std::vector<u8> boot_rom={})
            : timer(),
              boot(std::move(boot_rom)),
              memory(*this, rom),
              ppu(*this),
              cpu(memory)
        {
            if(boot.active()) {
                cpu.set_pc(0x0000) ;
            }
        }

        static constexpr int CYCLES_PER_FRAME = 17556;

        void run() { cpu.run(); }
        Cycles step() { return cpu.step(); }
        u16 pc() const { return cpu.pc(); }
        std::string disassemble() const { return cpu.disassemble(); }
        const u8* framebuffer() const { return ppu.framebuffer(); }   // image à afficher

        void run_frame() {
            accumulated_cycles = 0;
            while (accumulated_cycles < CYCLES_PER_FRAME)
                accumulated_cycles += step().value();
        }

        void play(int limit_instructions){
            for (int i = 0; i < limit_instructions; ++i) {
                std::cout << "[" << std::setw(2) << i << "] PC=0x"
                          << std::hex << std::setw(4) << std::setfill('0') << pc() << " "
                          << disassemble()
                          << std::dec << std::setfill(' ') << std::endl;
                step();
            }

            std::cout << "limite d'instructions atteinte" << std::endl;
        };
        void load_memory(std::string& filename){memory.copy_from_file(filename);}
        void render_once() {ppu.render_background();}

    private:
        Timer  timer;
        Boot boot;
        Memory memory;
        PPU    ppu;
        CPU    cpu;



        int accumulated_cycles ;

        friend class Memory;
        friend class Timer;
        friend class Boot ;
};
