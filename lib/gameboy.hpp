#include "CPU.hpp"
#include "memory.hpp"
#include "timer.hpp"
// #include "video.hpp"
// #include "inputs.hpp"


class Gameboy {
    public :
        Gameboy() = default;

    private :
        void tick();
        CPU cpu;
        friend class CPU;

        Memory memory;
        friend class MMU;

        Timer timer;
        friend class Timer;
}
