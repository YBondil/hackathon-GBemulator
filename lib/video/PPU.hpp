#pragma once
#include <vector>
#include "memory.hpp"
#include "definitions.hpp"
#include "registers.hpp"
class Gameboy ;
class PPU {
    public :
        PPU(Gameboy& Gb);
        ~PPU() = default ;
        auto read(u16 adress) -> u8 ;
        void write(u16 adress, u8 value) ;
        void draw() ;
        auto tick()-> void ;

    private :
        Gameboy& Gb;
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
