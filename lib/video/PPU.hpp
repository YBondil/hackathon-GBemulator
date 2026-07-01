#pragma once
#include <vector>
#include "memory.hpp"
#include "definitions.hpp"
#include "timer.hpp"
#include "registers.hpp"

class PPU {
    public :
        PPU(Memory& MMU, Timer& timer);
        ~PPU() = default ;
        auto read(u16 adress) -> u8 ;
        void write(u16 adress, u8 value) ;

        auto tick()-> void ;

    private :
        Memory& MMU;
        Timer& timer;

        std::vector<u8> VRAM ;

        //Registers
        Register LDC_control; // R/W
        Register LCDC_stat; // R/W
        Register Scroll_Y; // R/W
        Register Scroll_X; // R/W
        Register LCDC_Y_Coord; // R
        Register LY_compare; // R/W
        Register DMA_transf_stard;
        Register BG_palette; // R/W
        Register Object_palette_0; // R/W
        Register Object_palette_1; // R/W
        Register Window_Y ; // R/W
        Register Window_X ; // R/W


};
