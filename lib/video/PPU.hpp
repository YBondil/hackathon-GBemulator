#pragma once
#include <vector>
#include <array>
#include "memory.hpp"
#include "definitions.hpp"
#include "registers.hpp"

class Gameboy;

class PPU {
    public :
        static constexpr int WIDTH  = 160;
        static constexpr int HEIGHT = 144;

        PPU(Gameboy& Gb);
        ~PPU() = default ;

        auto read(u16 adress) -> u8 ;
        void write(u16 adress, u8 value) ;

        void tick(Cycles cycle);              // avance LY, rend la frame à la VBlank
        bool take_vblank();                   // true une fois par frame -> lever IF.bit0
        auto framebuffer() const -> const u8*;// WIDTH*HEIGHT nuances (0..3)
        void render_background();             // rend tout le BG dans 'frame'

    private :
        auto vram(u16 adress) const -> u8;    // accès VRAM (adresse absolue)

        Gameboy& Gb;
        std::vector<u8> VRAM ;

        std::array<u8, WIDTH * HEIGHT> frame{};   // image courante (nuances 0..3)
        int dots = 0;                             // cycles accumulés dans la ligne
        int line = 0;                             // LY courant (0..153)
        bool vblank = false;                      // demande d'interruption VBlank

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
