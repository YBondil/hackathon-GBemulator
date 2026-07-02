#pragma once
#include "definitions.hpp"
#include "memory"
#include <memory>

class Screen {
public:
    static constexpr int WIDTH  = 160;
    static constexpr int HEIGHT = 144;

    explicit Screen(int scale = 4);
    ~Screen();

    Screen(const Screen&) = delete;
    Screen& operator=(const Screen&) = delete; //pour interdire

    bool should_close() const;
    void draw(const u8* shades);

private:
    struct DataRaylib;
    std::unique_ptr<DataRaylib> infoRaylib;
};
