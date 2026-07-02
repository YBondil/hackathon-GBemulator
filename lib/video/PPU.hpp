#pragma once
#include <vector>
#include <array>
#include "memory.hpp"
#include "definitions.hpp"
#include "registers.hpp"

class Gameboy;
// Implémentation directe de https://gbdev.io/pandocs/Rendering.html#ppu-modes
// -> absence de window et de gestions des tiles ---> inutile pour Tetris
class PPU {
    public :
        static constexpr int WIDTH  = 160;
        static constexpr int HEIGHT = 144;

        PPU(Gameboy& Gb);
        ~PPU() = default ;

        auto read(u16 adress) -> u8 ;
        void write(u16 adress, u8 value) ;

        void tick(Cycles cycle);
        bool take_vblank();
        auto framebuffer() const -> const u8*;
        void render_background();

    private :
        auto vram(u16 adress) const -> u8;

        Gameboy& Gb;
        std::vector<u8> VRAM ;

        std::array<u8, WIDTH * HEIGHT> frame{};
        int dots = 0;
        int line = 0;
        bool vblank = false;

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
