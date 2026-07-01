#include "video/screen.hpp"
#include "raylib.h"     // n'apparaît QUE dans ce .cpp -> pas de conflit avec ton enum Color
#include <array>

namespace {
    // Les 4 nuances DMG (index 0..3), en RGBA. 0 = clair, 3 = foncé.
    constexpr Color SHADES[4] = {
        Color{224, 248, 208, 255},   // 0
        Color{136, 192, 112, 255},   // 1
        Color{ 52, 104,  86, 255},   // 2
        Color{  8,  24,  32, 255},   // 3
    };
}

// Détails raylib cachés (PImpl) : rien de tout ça ne fuit dans screen.hpp.
struct Screen::Impl {
    int       scale;
    Texture2D texture;
    std::array<Color, WIDTH * HEIGHT> rgba;   // tampon envoyé à la texture
};

Screen::Screen(int scale) : impl(std::make_unique<Impl>()) {
    impl->scale = scale;
    impl->rgba.fill(SHADES[0]);

    InitWindow(WIDTH * scale, HEIGHT * scale, "GBemulator");
    SetTargetFPS(60);

    Image img = {
        .data    = impl->rgba.data(),
        .width   = WIDTH,
        .height  = HEIGHT,
        .mipmaps = 1,
        .format  = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
    };
    impl->texture = LoadTextureFromImage(img);
    SetTextureFilter(impl->texture, TEXTURE_FILTER_POINT);   // pixels nets, pas de flou
}

Screen::~Screen() {
    UnloadTexture(impl->texture);
    CloseWindow();
}

bool Screen::should_close() const {
    return WindowShouldClose();
}

void Screen::draw(const u8* shades) {
    // 1. nuances 0..3 -> RGBA
    for (int i = 0; i < WIDTH * HEIGHT; ++i)
        impl->rgba[i] = SHADES[shades[i] & 0x3];

    // 2. pousse le tampon vers la texture GPU
    UpdateTexture(impl->texture, impl->rgba.data());

    // 3. dessine la texture mise à l'échelle
    BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(
            impl->texture,
            Rectangle{0, 0, (float)WIDTH, (float)HEIGHT},
            Rectangle{0, 0, (float)(WIDTH * impl->scale), (float)(HEIGHT * impl->scale)},
            Vector2{0, 0}, 0.0f, WHITE);
    EndDrawing();
}
