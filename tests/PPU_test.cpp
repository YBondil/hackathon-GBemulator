// Test visuel du PPU : charge chaque dump mémoire de roms/exports_memoire,
// rend le background et l'affiche. Défile automatiquement (~2 s par dump),
// en boucle. Échap ou fermer la fenêtre pour quitter.
//
// Ne teste PAS le CPU : on injecte directement un état mémoire complet (VRAM +
// registres LCD) et on regarde ce que le PPU en fait.
#include "gameboy.hpp"
#include "video/screen.hpp"
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

int main() {
    const std::string dir = "roms/exports_memoire";

    // 1. collecte des dumps
    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".bin")
            files.push_back(entry.path().string());
    }
    std::sort(files.begin(), files.end());

    if (files.empty()) {
        std::cerr << "Aucun dump .bin dans " << dir
                  << " (lance depuis la racine du projet)" << std::endl;
        return 1;
    }
    std::cout << files.size() << " dumps trouves. Defilement auto ~2s, Echap pour quitter."
              << std::endl;

    // 2. une seule Gameboy + un seul Screen ; on recharge la mémoire à chaque dump
    std::vector<u8> dummy_rom(0x8000, 0);   // ROM factice, écrasée par le dump
    Gameboy gb(dummy_rom);
    Screen  screen(4);

    constexpr int FRAMES_PER_DUMP = 120;    // ~2 s à 60 fps
    std::size_t idx = 0;
    int frames = 0;

    auto show = [&](std::size_t i) {
        std::string path = files[i];        // lvalue requise par load_memory(std::string&)
        gb.load_memory(path);
        gb.render_once();
        std::cout << "[" << (i + 1) << "/" << files.size() << "] " << path << std::endl;
    };

    show(idx);
    while (!screen.should_close()) {
        screen.draw(gb.framebuffer());
        if (++frames >= FRAMES_PER_DUMP) {
            frames = 0;
            idx = (idx + 1) % files.size();
            show(idx);
        }
    }
    return 0;
}
