#pragma once
#include "../utils/definitions.hpp"

enum class GBColor {
    Color0, /* White */
    Color1, /* Light gray */
    Color2, /* Dark gray */
    Color3, /* Black */
};

enum class Color {
    White,
    LightGray,
    DarkGray,
    Black,
};

struct Palette {
    Color color0 = Color::White;
    Color color1 = Color::LightGray;
    Color color2 = Color::DarkGray;
    Color color3 = Color::Black;
};