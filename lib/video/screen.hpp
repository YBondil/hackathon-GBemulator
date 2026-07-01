#pragma once
#include "definitions.hpp"
#include <memory>

class Screen {
public:
    static constexpr int WIDTH  = 160;
    static constexpr int HEIGHT = 144;

    explicit Screen(int scale = 4);   // ouvre la fenêtre
    ~Screen();                        // ferme la fenêtre

    Screen(const Screen&) = delete;               // possède fenêtre + texture GPU
    Screen& operator=(const Screen&) = delete;

    bool should_close() const;        // fenêtre fermée ou touche Échap
    void draw(const u8* shades);      // WIDTH*HEIGHT octets, chacun dans {0,1,2,3}

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
