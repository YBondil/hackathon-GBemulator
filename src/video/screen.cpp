#include "video/screen.hpp"
#include "raylib.h"
#include <array>
#include <memory>

namespace {
    constexpr Color COLORS[4] = {
        Color{224, 248, 208, 255},   // 0
        Color{136, 192, 112, 255},   // 1
        Color{ 52, 104,  86, 255},   // 2
        Color{  8,  24,  32, 255},   // 3
    };
}

struct Screen::DataRaylib {
    DataRaylib() = default ;
    int       scale;
    Texture2D texture;
    std::array<Color, WIDTH * HEIGHT> rgba;
};

Screen::Screen(int scale) : infoRaylib(std::make_unique<DataRaylib>()) {

    infoRaylib->scale = scale;
    infoRaylib->rgba.fill(COLORS[0]);

    InitWindow(WIDTH * scale, HEIGHT * scale, "GBemulator");
    SetTargetFPS(60);

    Image img = {
        .data    = infoRaylib->rgba.data(),
        .width   = WIDTH,
        .height  = HEIGHT,
        .mipmaps = 1,
        .format  = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
    };
    infoRaylib->texture = LoadTextureFromImage(img);
    SetTextureFilter(infoRaylib->texture, TEXTURE_FILTER_POINT);
}

Screen::~Screen() {
    UnloadTexture(infoRaylib->texture);
    CloseWindow();
}

bool Screen::should_close() const {
    return WindowShouldClose();
}

void Screen::draw(const u8* colors) {
    for (int i = 0; i < WIDTH * HEIGHT; ++i)
        infoRaylib->rgba[i] = COLORS[colors[i] & 0x3];

    UpdateTexture(infoRaylib->texture, infoRaylib->rgba.data());

    BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(
            infoRaylib->texture,
            Rectangle{0, 0, (float)WIDTH, (float)HEIGHT},
            Rectangle{0, 0, (float)(WIDTH * infoRaylib->scale), (float)(HEIGHT * infoRaylib->scale)},
            Vector2{0, 0}, 0.0f, WHITE);
    EndDrawing();
}
