#pragma once
#include "definitions.hpp"

class Memory {
  public:
    virtual ~Memory() = default;
    virtual auto read(u16 address) -> u8 = 0;
    virtual auto write(u16 address, u8 value) -> void = 0;
    virtual void tick(Cycles cycle) = 0;
};
