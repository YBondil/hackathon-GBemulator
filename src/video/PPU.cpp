#include "gameboy.hpp"
#include "video/PPU.hpp"
#include "bitwise.hpp"

namespace {
    constexpr u16 VRAM_BEGIN = 0x8000, VRAM_END = 0x9FFF;
    constexpr u16 LCD_CONTROL = 0xFF40, LCDC_STATUS = 0xFF41;
    constexpr u16 SCY = 0xFF42, SCX = 0xFF43, LY = 0xFF44, LYC = 0xFF45;
    constexpr u16 DMA = 0xFF46, BGP = 0xFF47, OBP0 = 0xFF48, OBP1 = 0xFF49;
    constexpr u16 WY = 0xFF4A, WX = 0xFF4B;

    constexpr int DOTS_PER_LINE = 456;   // T-cycles par ligne
    constexpr int VBLANK_LINE   = 144;   // début VBlank
    constexpr int LAST_LINE     = 153;   // dernière ligne avant de reboucler
    constexpr int OBJ_8X8 = 64 ;
    constexpr int OBJ_8X16 = 128 ;

}

PPU::PPU(Gameboy& gb) : Gb(gb) {
    VRAM = std::vector<u8>(0x2000, 0);   // 0x8000-0x9FFF
}

auto PPU::vram(u16 adress) const -> u8 {
    if (adress >= VRAM_BEGIN && adress <= VRAM_END) return VRAM[adress - VRAM_BEGIN];
    return 0xFF;
}

auto PPU::read(u16 adress) -> u8 {
    switch (adress) {
        case LCD_CONTROL: return LDC_control.value();
        case LCDC_STATUS: return LCDC_stat.value();
        case SCY:  return Scroll_Y.value();
        case SCX:  return Scroll_X.value();
        case LY:   return LCDC_Y_Coord.value();
        case LYC:  return LY_compare.value();
        case DMA:  return DMA_transf_stard.value();
        case BGP:  return BG_palette.value();
        case OBP0: return Object_palette_0.value();
        case OBP1: return Object_palette_1.value();
        case WY:   return Window_Y.value();
        case WX:   return Window_X.value();
        default:
            if (adress >= VRAM_BEGIN && adress <= VRAM_END) return VRAM[adress - 0x8000];
            return 0x00;
    }
}

void PPU::write(u16 adress, u8 value) {
    switch (adress) {
        case LCD_CONTROL: LDC_control.set(value); break;
        case LCDC_STATUS: LCDC_stat.set(value); break;
        case SCY:  Scroll_Y.set(value); break;
        case SCX:  Scroll_X.set(value); break;
        case LYC:  LY_compare.set(value); break;
        case DMA:  DMA_transf_stard.set(value); break;
        case BGP:  BG_palette.set(value); break;
        case OBP0: Object_palette_0.set(value); break;
        case OBP1: Object_palette_1.set(value); break;
        case WY:   Window_Y.set(value); break;
        case WX:   Window_X.set(value); break;
        default:
            if (adress >= VRAM_BEGIN && adress <= VRAM_END) VRAM[adress - 0x8000] = value;
            break;
    }
}

void PPU::tick(Cycles cycle) {
    dots += cycle.value();
    while (dots >= DOTS_PER_LINE) {
        dots -= DOTS_PER_LINE;
        ++line;
        if (line == VBLANK_LINE) {
            render_background();
            vblank = true;
        } else if (line > LAST_LINE) {
            line = 0;
        }
        LCDC_Y_Coord.set(static_cast<u8>(line));
    }
}

bool PPU::take_vblank() {
    bool v = vblank;
    vblank = false;
    return v;
}

auto PPU::framebuffer() const -> const u8* { return frame.data(); }

void PPU::render_background(){
    const u8 lcdc = LDC_control.value();

    //lecture LCD_control
    bool window_bg_enable = bitwise::check_bit(lcdc, 0);
    bool obj_enable = bitwise::check_bit(lcdc, 1); //inutile
    int obj_size = (bitwise::check_bit(lcdc, 2)) ? OBJ_8X16 : OBJ_8X8 ; //inutile
    u16 bg_tile_map = bitwise::check_bit(lcdc, 3) ? 0x9C00 : 0x9800;
    bool unsigned_addressing = bitwise::check_bit(lcdc, 4);
    bool window_displayed = bitwise::check_bit(lcdc, 5); //inutile
    u16 window_tile_map = bitwise::check_bit(lcdc,6) ? 0x9800 : 0x9C00 ;
    bool ldc_active = bitwise::check_bit(lcdc,7) ;
    if(!ldc_active){frame.fill(0);}

    u8 scY = Scroll_Y.value();
    u8 scX = Scroll_X.value();
    u8 BGPalette = BG_palette.value();
    for (int y=0; y<HEIGHT; ++y){
        u8 bgy = static_cast<u8>(y + scY);
        for (int x=0; x<WIDTH; ++x){
            u8 bgx = static_cast<u8>(x + scX);

            u16 tile_address = bg_tile_map + (bgy/8) * 32 + (bgx/8) ;
            u8 tile_index = vram(tile_address) ;

            u16 tile_data = unsigned_addressing ?
                0x8000 + tile_index * 16 :
                0x9000 + (s8)tile_index * 16 ;

            u16 line = (u16)bgy % 8 ;
            u8 low = vram(tile_data + line*2) ;
            u8 high = vram(tile_data + line*2 + 1) ;
            int col = 7 - (bgx % 8) ;
            u8 color_id = bitwise::compose_bits(bitwise::bit_value(low, col), bitwise::bit_value(high, col));
            u8 color = BGPalette >> color_id * 2 & 0x3 ;
            frame[bgx + bgy*WIDTH] = color ;

        }
    }
}
