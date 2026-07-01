#include "utils/definitions.hpp"
#include "utils/readfile.hpp"
#include "gameboy.hpp"
#include "video/screen.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <array>

auto main(int argc, char* argv[]) -> int {
    // chargement rom
    const std::string rom_path = (argc > 1) ? argv[1] : "";
    if (rom_path == "") {
        std::cerr << "Usage : gbemu <rom>" << std::endl;
        return 1;
    }
    std::vector<u8> rom = read_file(rom_path);
    if (rom.empty()) {
        std::cerr << "ROM vide ou illisible : " << rom_path << std::endl;
        return 1;
    }
    std::cout << "ROM chargee : " << rom_path << " (" << rom.size()
              << " octets)" << std::endl;

    //Boot
    std::vector<u8> boot = read_file("roms/dmg_boot.bin");   // 256 octets
    Gameboy gb(std::move(rom), std::move(boot));
    std::string dump = "roms/exports_memoire/out_9607920854581853450.bin";
    gb.load_memory(dump);

    Screen screen(4);
    gb.render_once();
    while (!screen.should_close())
        screen.draw(gb.framebuffer());
    return 0;
}
