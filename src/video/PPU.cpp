#include <iostream>
#include "video/PPU.hpp"


PPU::PPU(Memory& MMU, Timer& timer) : MMU(MMU), timer(timer) {
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
PPU::read(u16 adress){


}



/* int FillImage(FrameBuffer& buffer) -> void {
    int image[GB_WIDTH * GB_HEIGHT];
    for (int y = 0; y < GB_HEIGHT; y++) {
        for (int x = 0; x < GB_WIDTH; x++) {
            image[y * GB_WIDTH + x] = buffer.get_pixel(x, y);
        }
    }
    return image;
    } */
