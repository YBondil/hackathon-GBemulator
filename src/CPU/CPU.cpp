#include "CPU/CPU.hpp"
#include "bitwise.hpp"
#include "opcode_names.hpp"
#include <cstdio>

auto CPU::fetch8()  -> u8  { return memory.read(PC++); }
auto CPU::fetch16() -> u16 {
    u8 lo = fetch8();
    u8 hi = fetch8();
    return bitwise::compose_bytes(hi, lo);
}
auto CPU::fetch_s8() -> s8 { return static_cast<s8>(fetch8()); }

// Exécute une seule instruction (ou sert une interruption en attente).
auto CPU::step() -> Cycles {
    Cycles cycle = handle_interrupts();

    if (cycle.value() == 0) {
        u8 opcode = fetch8();
        cycle = run_opcode(opcode);
    }

    memory.tick(cycle); //gestion ticks ppu etc
    return cycle;
}

auto CPU::pc() const -> u16 { return PC; }
void CPU::set_pc(u16 value){PC = value;}

// Désassemble l'instruction située à PC (sans modifier l'état).
static std::string fmt_hex(unsigned v, int width) {
    char buf[8];
    std::snprintf(buf, sizeof buf, "0x%0*X", width, v);
    return buf;
}

auto CPU::disassemble() const -> std::string {
    u8 op = memory.read(PC);

    if (op == 0xCB) {
        u8 cb = memory.read(static_cast<u16>(PC + 1));
        const char* n = CB_OPCODE_NAMES[cb];
        return n ? (std::string("CB ") + n) : ("CB " + fmt_hex(cb, 2));
    }

    const char* name = OPCODE_NAMES[op];
    if (!name) return "DB " + fmt_hex(op, 2);   // opcode non nommé

    std::string s = name;
    std::string::size_type p;
    if ((p = s.find("n16")) != std::string::npos || (p = s.find("a16")) != std::string::npos) {
        u16 lo = memory.read(static_cast<u16>(PC + 1));
        u16 hi = memory.read(static_cast<u16>(PC + 2));
        s.replace(p, 3, fmt_hex((hi << 8) | lo, 4));
    } else if ((p = s.find("n8")) != std::string::npos || (p = s.find("a8")) != std::string::npos) {
        u8 v = memory.read(static_cast<u16>(PC + 1));
        s.replace(p, 2, fmt_hex(v, 2));
    } else if ((p = s.find("e8")) != std::string::npos) {
        s8 v = static_cast<s8>(memory.read(static_cast<u16>(PC + 1)));
        char buf[8]; std::snprintf(buf, sizeof buf, "%+d", v);
        s.replace(p, 2, buf);
    }
    return s;
}

void CPU::run() {
    while (isRunning) step();
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


Cycles CPU::run_opcode(u8 opcode) {
    Op op = (opcode == 0xCB) ? cb_table[fetch8()] : opcode_table[opcode];
    if (!op) return Cycles(1);     // opcode non implémenté
    return op(*this);
}

#include "opcode_mapping.hpp"   //laisser l'include ici please
