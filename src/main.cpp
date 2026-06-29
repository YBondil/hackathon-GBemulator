#include "utils/definitions.hpp"
#include "utils/readfile.hpp"
#include "raylib.h"
#include <iostream>
#include <string>
#include <vector>

constexpr int GB_WIDTH  = 160;
constexpr int GB_HEIGHT = 144;
constexpr int SCALE     = 4;

auto main(int argc, char* argv[]) -> int {
    const std::string rom_path = (argc > 1) ? argv[1] : "";
    if (rom_path == "") {
        std::cout << "veuillez préciser le fichier à ouvrir" << std::endl;
        return 1;
    }

    std::vector<u8> rom = read_file(rom_path);
    if (rom.empty()) {
        std::cerr << "ROM vide ou illisible : " << rom_path << std::endl;
        return 1;
    }
    std::cout << "ROM chargee : " << rom_path << " (" << rom.size()
              << " octets)" << std::endl;

    InitWindow(GB_WIDTH * SCALE, GB_HEIGHT * SCALE, "GBemulator");
    SetTargetFPS(60);

    //Ecran noir pour l'instant
    std::vector<Color> pixels(GB_WIDTH * GB_HEIGHT, BLACK);
    Image fb = {
        .data    = pixels.data(),
        .width   = GB_WIDTH,
        .height  = GB_HEIGHT,
        .mipmaps = 1,
        .format  = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
    };
    Texture2D screen = LoadTextureFromImage(fb);
    SetTextureFilter(screen, TEXTURE_FILTER_POINT);

    // Boucle principale de jeu
    while (!WindowShouldClose()) {
        UpdateTexture(screen, pixels.data());

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(
                screen,
                Rectangle{0, 0, (float)GB_WIDTH, (float)GB_HEIGHT},
                Rectangle{0, 0, (float)(GB_WIDTH * SCALE), (float)(GB_HEIGHT * SCALE)},
                Vector2{0, 0}, 0.0f, WHITE);
        EndDrawing();
    }

    UnloadTexture(screen);
    CloseWindow();
    return 0;
}
