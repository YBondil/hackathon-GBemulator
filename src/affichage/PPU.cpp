#include <iostream>
#include "PPU.hpp"

int FillImage(FrameBuffer& buffer) -> void {
    int image[GB_WIDTH * GB_HEIGHT];
    for (int y = 0; y < GB_HEIGHT; y++) {
        for (int x = 0; x < GB_WIDTH; x++) {
            Color color = buffer.get_pixel(x, y);
            int pixel_value = get_color(u8 pixel_value)
            image[y * GB_WIDTH + x] = pixel_value;
        }
    }
}