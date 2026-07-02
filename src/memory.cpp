#include "memory.hpp"
#include "gameboy.hpp"
#include "readfile.hpp"
#include <iostream>

namespace {
    constexpr u16 ROM_END    = 0x7FFF;                       // 0x0000-0x7FFF : cartouche
    constexpr u16 VRAM_BEGIN = 0x8000, VRAM_END = 0x9FFF;
    constexpr u16 ERAM_BEGIN = 0xA000, ERAM_END = 0xBFFF;    // RAM cartouche (non gérée)
    constexpr u16 WRAM_BEGIN = 0xC000, WRAM_END = 0xDFFF;
    constexpr u16 ECHO_BEGIN = 0xE000, ECHO_END = 0xFDFF;    // écho de la WRAM
    constexpr u16 OAM_BEGIN  = 0xFE00, OAM_END  = 0xFE9F;
    constexpr u16 UNUSABLE_B = 0xFEA0, UNUSABLE_E = 0xFEFF;
    constexpr u16 IO_BEGIN   = 0xFF00, IO_END   = 0xFF7F;
    constexpr u16 HRAM_BEGIN = 0xFF80, HRAM_END = 0xFFFE;
    constexpr u16 IE_ADDR    = 0xFFFF;

    // registres délégués
    constexpr u16 TIMER_BEGIN = 0xFF04, TIMER_END = 0xFF07;  // -> Timer
    constexpr u16 LCD_BEGIN   = 0xFF40, LCD_END   = 0xFF4B;  // -> PPU
    constexpr u16 IF_ADDR = 0xFF0F;
    constexpr u8  IRQ_TIMER = 1 << 2;
}

Memory::Memory( Gameboy& gb, std::vector<u8>& rom)
    : Gb(gb), ROM_Data(rom) {}

auto Memory::read(u16 address) -> u8 {
    if ((Gb.boot.active()) && (address<=0x00F)){
        return Gb.boot.read(address);
    }
    if (address <= ROM_END)
        return address < ROM_Data.size() ? ROM_Data[address] : 0xFF;
    if (address >= VRAM_BEGIN && address <= VRAM_END) return Gb.ppu.read(address);  // délégué PPU
    if (address >= ERAM_BEGIN && address <= ERAM_END) return 0xFF;                  // pas de RAM cartouche
    if (address >= WRAM_BEGIN && address <= WRAM_END) return WRAM[address - WRAM_BEGIN];
    if (address >= ECHO_BEGIN && address <= ECHO_END) return WRAM[address - ECHO_BEGIN];
    if (address >= OAM_BEGIN  && address <= OAM_END)  return OAM[address - OAM_BEGIN];
    if (address >= UNUSABLE_B && address <= UNUSABLE_E) return 0xFF;
    if (address >= IO_BEGIN   && address <= IO_END)   return read_io(address);
    if (address >= HRAM_BEGIN && address <= HRAM_END) return HRAM[address - HRAM_BEGIN];
    if (address == IE_ADDR)                           return IE;
    return 0xFF;
}

void Memory::write(u16 address, u8 data) {
    if (address <= ROM_END)                                return;                  // ROM lecture seule
    else if (address >= VRAM_BEGIN && address <= VRAM_END) Gb.ppu.write(address, data); // délégué PPU
    else if (address >= ERAM_BEGIN && address <= ERAM_END) { /* pas de RAM cartouche */ }
    else if (address >= WRAM_BEGIN && address <= WRAM_END) WRAM[address - WRAM_BEGIN] = data;
    else if (address >= ECHO_BEGIN && address <= ECHO_END) WRAM[address - ECHO_BEGIN] = data;
    else if (address >= OAM_BEGIN  && address <= OAM_END)  OAM[address - OAM_BEGIN] = data;
    else if (address >= UNUSABLE_B && address <= UNUSABLE_E) { /* interdit */ }
    else if (address >= IO_BEGIN   && address <= IO_END)   write_io(address, data);
    else if (address >= HRAM_BEGIN && address <= HRAM_END) HRAM[address - HRAM_BEGIN] = data;
    else if (address == IE_ADDR)                           IE = data;
}

// --- espace I/O 0xFF00-0xFF7F ---
auto Memory::read_io(u16 address) -> u8 {
    if (address >= TIMER_BEGIN && address <= TIMER_END) return Gb.timer.read(address);  // délégué Timer
    if (address >= LCD_BEGIN   && address <= LCD_END)   return Gb.ppu.read(address);    // délégué PPU
    return IO[address - IO_BEGIN];                                                      // IF, joypad, etc.
}

void Memory::write_io(u16 address, u8 data) {
    if (address == 0xFF50) { if (data != 0) Gb.boot.disable(); return; }
    if (address >= TIMER_BEGIN && address <= TIMER_END) Gb.timer.write(address, data);  // délégué Timer
    else if (address >= LCD_BEGIN && address <= LCD_END) Gb.ppu.write(address, data);   // délégué PPU
    else IO[address - IO_BEGIN] = data;
}

void Memory::tick(Cycles cycle) {
    const Cycles t(cycle.value() * 4);

    Gb.timer.tick(t);
    if (Gb.timer.take_interrupt())
        IO[IF_ADDR - IO_BEGIN] |= IRQ_TIMER;

    Gb.ppu.tick(t);
    if (Gb.ppu.take_vblank())
        IO[IF_ADDR - IO_BEGIN] |= 0x01;
}

bool Memory::boot_rom_active() const { return boot_active; }


void Memory::copy_from_file(std::string& file_name) {
    std::vector<u8> dump = read_file(file_name);
    if (dump.size() < 0x10000) {
        std::cerr << "Dump memoire invalide (" << dump.size()
                  << " octets, attendu 65536) : " << file_name << std::endl;
        return;
    }
    ROM_Data.assign(dump.begin(), dump.begin() + 0x8000);
    for (int addr = 0x8000; addr <= 0xFFFF; ++addr) {
        write(static_cast<u16>(addr), dump[addr]);
    }
}
