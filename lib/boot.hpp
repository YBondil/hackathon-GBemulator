#pragma once
#include "definitions.hpp"
#include <vector>

class Boot {
public:
    Boot() = default;
    explicit Boot(std::vector<u8> rom);

    bool loaded() const;
    bool active() const;
    auto read(u16 address) const -> u8;
    void disable();

private:
    std::vector<u8> data;
    bool is_active = false;
};
