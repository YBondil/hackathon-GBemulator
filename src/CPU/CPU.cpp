#include "CPU/CPU.hpp"


void CPU::run() {
    while (isRunning) {
        Cycles cycle = handle_interrupts();

        if (cycle.value() == 0) {
            u8 opcode = memory.read(PC);
            cycle = run_opcode(opcode);
        }

        memory.tick(cycle); //gestion ticks ppu etc
    }
}

auto CPU::handle_interrupts() -> Cycles {
    if (!IME) return Cycles(0);

    u8 IF = memory.read(irq::IF_ADDR);
    u8 IE = memory.read(irq::IE_ADDR);
    u8 pending = IF & IE & 0x1F;
    if (pending == 0) return Cycles(0);

    IME = false;

    for (int i = 0; i < 5; ++i) {           // bit 0 (VBlank) -> priorité ++
        if (pending & (1 << i)) {
            memory.write(irq::IF_ADDR, IF & ~(1 << i));
            push_stack(PC);
            PC = static_cast<u16>(0x0040 + i * 0x08);
            return Cycles(5);   // le dispatch coûte ~5 M-cycles (20 T-cycles)
        }
    }
    return Cycles(0);
}


void CPU::push_stack(u16 value) {
    SP.decrement();
    memory.write(SP.value(), static_cast<u8>(value >> 8));   // octet haut
    SP.decrement();
    memory.write(SP.value(), static_cast<u8>(value & 0xFF)); // octet bas
}

auto CPU::pop_stack16() -> u16 {
    u8 low  = memory.read(SP.value());
    SP.increment();
    u8 high = memory.read(SP.value());
    SP.increment();
    return static_cast<u16>((high << 8) | low);
}


auto CPU::pop_stack8() -> u8 {
    u8 value = memory.read(SP.value());
    SP.increment();
    return value;
}


auto run_opcode(u8 opcode){return 0x00;}//todo
