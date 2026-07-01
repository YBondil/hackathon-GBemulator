#include "gameboy.hpp"
#include "video/PPU.hpp"

namespace {
    constexpr u16 VRAM_BEGIN = 0x8000, VRAM_END = 0x9FFF;
    constexpr u16 LCD_CONTROL = 0xFF40, LCDC_STATUS = 0xFF41;
    constexpr u16 SCY = 0xFF42, SCX = 0xFF43, LY = 0xFF44, LYC = 0xFF45;
    constexpr u16 DMA = 0xFF46, BGP = 0xFF47, OBP0 = 0xFF48, OBP1 = 0xFF49;
    constexpr u16 WY = 0xFF4A, WX = 0xFF4B;

    constexpr int DOTS_PER_LINE = 456;   // T-cycles par ligne
    constexpr int VBLANK_LINE   = 144;   // début de la VBlank
    constexpr int LAST_LINE     = 153;   // dernière ligne avant de reboucler
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

//on ne fait qu'avancer LY. Le rendu se fait d'un seul coup quand on entre en VBlank
void PPU::tick(Cycles cycle) {
    dots += cycle.value();
    while (dots >= DOTS_PER_LINE) {
        dots -= DOTS_PER_LINE;
        ++line;
        if (line == VBLANK_LINE) {
            render_background();   // rend toute l'image d'un coup
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
// absence gestion des sprites
void PPU::render_background() {
    const u8 lcdc = LDC_control.value();

    // bit0 : BG activé ? sinon écran blanc
    if (!(lcdc & 0x01)) { frame.fill(0); return; }

    const u16  map_base       = (lcdc & 0x08) ? 0x9C00 : 0x9800;  // bit3 : quelle tilemap
    const bool unsigned_tiles = (lcdc & 0x10);                    // bit4 : quelle tile data
    const u8   scy = Scroll_Y.value();
    const u8   scx = Scroll_X.value();
    const u8   bgp = BG_palette.value();

    for (int y = 0; y < HEIGHT; ++y) {
        const u8 bgY = static_cast<u8>(scy + y);      // wrap 256 automatique (u8)
        for (int x = 0; x < WIDTH; ++x) {
            const u8 bgX = static_cast<u8>(scx + x);

            // index de tuile dans la tilemap 32x32
            const u16 map_addr = map_base + (bgY / 8) * 32 + (bgX / 8);
            const u8  index    = vram(map_addr);

            //adresse de la tuile
            const u16 tile_addr = unsigned_tiles
                ? static_cast<u16>(0x8000 + index * 16)
                : static_cast<u16>(0x9000 + static_cast<int8_t>(index) * 16);

            //les 2 octets de la ligne de la tuile (2 bits/pixel)
            const int row = bgY % 8;
            const u8  lo  = vram(tile_addr + row * 2);
            const u8  hi  = vram(tile_addr + row * 2 + 1);

            //le pixel -> id couleur 0..3 -> palette BGP -> nuance
            const int bit      = 7 - (bgX % 8);
            const u8  color_id = static_cast<u8>((((hi >> bit) & 1) << 1) | ((lo >> bit) & 1));
            const u8  shade    = (bgp >> (color_id * 2)) & 0x3;

            frame[y * WIDTH + x] = shade;
        }
    }
}
