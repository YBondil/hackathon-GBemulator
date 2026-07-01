#include <iostream>
#include "gameboy.hpp"
#include "video/PPU.hpp"
namespace {
    constexpr u16 VRAM_BEGIN = 0x8000, VRAM_END = 0x9FFF;
    constexpr u16 LCD_CONTROL = 0xFF40 ;
    constexpr u16 LCDC_STATUS = 0xFF41 ;
    constexpr u16 SCY = 0xFF42 ;
    constexpr u16 SCX = 0xFF43 ;
    constexpr u16 LY = 0xFF44 ;
    constexpr u16 LYC = 0xFF45 ;
    constexpr u16 DMA = 0xFF46 ;
    constexpr u16 BGP = 0xFF47 ;
    constexpr u16 OBP0 = 0xFF48 ;
    constexpr u16 OBP1 = 0xFF49 ;
    constexpr u16 WY = 0xFF4A ;
    constexpr u16 WX = 0xFF4B ;
}

PPU::PPU(Gameboy& gb) : Gb(gb) {
    LDC_control  = Register();
    LCDC_stat = Register() ;
    Scroll_Y = Register();
    Scroll_X = Register();
    LCDC_Y_Coord = Register();
    LY_compare = Register();
    DMA_transf_stard = Register();
    BG_palette = Register();
    Object_palette_0 = Register();
    Object_palette_1 = Register();
    Window_Y = Register();
    Window_X = Register();

    VRAM = std::vector<u8>(8000);
}
// NB : vram = 0x8000 - 9FFF
auto PPU::read(u16 adress) -> u8 {
    switch (adress){
        case LCD_CONTROL :
            return LDC_control.value() ;
        case LCDC_STATUS :
            return LCDC_stat.value() ;
        case SCY :
            return Scroll_Y.value();
        case SCX :
            return Scroll_X.value();
        case LY :
            return LCDC_Y_Coord.value();
        case LYC :
            return LY_compare.value();
        case DMA :
            return DMA_transf_stard.value();
        case BGP :
            return BG_palette.value();
        case OBP0 :
            return Object_palette_0.value();
        case OBP1 :
            return Object_palette_1.value();
        case WY :
            return Window_Y.value() ;
        case WX :
            return Window_X.value() ;
        default :
            if ((adress >= VRAM_BEGIN) & (adress<= VRAM_END)) {
                return VRAM[adress-0x8000] ;
            }else{
                return 0x00 ;
            }
    }
}
void PPU::write(u16 adress, u8 value){
    switch (adress){
        case LCD_CONTROL :
            LDC_control.set(value) ;
        case LCDC_STATUS :
            LCDC_stat.set(value) ;
        case SCY :
            Scroll_Y.set(value) ;
        case SCX :
            Scroll_X.set(value) ;
        case LYC :
            LY_compare.set(value) ;
        case DMA :
            DMA_transf_stard.set(value) ;
        case BGP :
            BG_palette.set(value) ;
        case OBP0 :
            Object_palette_0.set(value) ;
        case OBP1 :
            Object_palette_1.set(value) ;
        case WY :
            Window_Y.set(value) ;
        case WX :
            Window_X.set(value) ;
        default :
            if ((adress >= VRAM_BEGIN) & (adress<= VRAM_END)) {
                VRAM[adress-0x8000] = value;
                break ;
            }else{
                return ;
            }
    }
}
