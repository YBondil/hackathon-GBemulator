#include "boot.hpp"

Boot::Boot(std::vector<u8> rom)
    : data(std::move(rom)),
      is_active(!data.empty())
{}

bool Boot::loaded() const { return !data.empty(); }

bool Boot::active() const { return is_active; }

auto Boot::read(u16 address) const -> u8 {
    return address < data.size() ? data[address] : 0xFF;
}

void Boot::disable() { is_active = false; }
