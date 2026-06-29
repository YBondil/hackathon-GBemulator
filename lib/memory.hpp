#pragma once


class Memory {
    public:
    auto read(const u16 adress)->u8;
    auto write(const u16 adress) -> void;
};
    