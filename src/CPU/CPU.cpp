#include "CPU/CPU.hpp"
#include "bitwise.hpp"

auto CPU::fetch8() -> u8 {return  memory.read(PC++);};
auto CPU::fetch16() -> u16 { u8 lo = fetch8(); u8 hi = fetch8() ; return bitwise::compose_bytes(hi, lo);}
auto CPU::fetch_s8()-> s8 {return static_cast<s8>(fetch8());}

void CPU::run() {
    while (isRunning) {
        Cycles cycle = handle_interrupts();

        if (cycle.value() == 0) {
            u8 opcode = fetch8();
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

auto CPU::pop_stack() -> u16 {
    u8 low  = memory.read(SP.value());
    SP.increment();
    u8 high = memory.read(SP.value());
    SP.increment();
    return static_cast<u16>((high << 8) | low);
}

Cycles CPU::run_opcode(u8 opcode) {
    Op op = (opcode == 0xCB) ? cb_table[fetch8()] : opcode_table[opcode]; // cas prefix CB
    if (!op) return Cycles(1);     // opcode non implémenté
    return op(*this);
}


#include "opcode_mapping.hpp"
