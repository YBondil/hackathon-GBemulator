#include "utils/definitions.hpp"
#include "utils/readfile.hpp"
#include "gameboy.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

constexpr int MAX_INSTRUCTIONS = 30;

auto main(int argc, char* argv[]) -> int {
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

    Gameboy gb(std::move(rom));

    for (int i = 0; i < MAX_INSTRUCTIONS; ++i) {
        std::cout << "[" << std::setw(2) << i << "] PC=0x"
                  << std::hex << std::setw(4) << std::setfill('0') << gb.pc()
                  << std::dec << std::setfill(' ') << std::endl;
        gb.step();
    }

    std::cout << "fin" << std::endl;
    return 0;
}
