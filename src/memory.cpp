#include "memory.hpp"

Memory::Memory(){

    WRAM = std::vector<u8>(8192);
    OAM = std::vector<u8>(160);
    HRAM = std::vector<u8>(127);

};

u8 Memory::read(u16 address) {
    if (0x0 <= address) && (address <= 0x7FFF) {
        if (((0x0 <= address) && (address <= 0xFF)) && boot_rom_active()) {
            return bootDMG[address];
        }
        return gb.cartridge->read(address);
    }
    /* VRAM */
    if (((0x8000 <= address) && (address <= 0x9FFF))) {
        return gb.video.read(address - 0x8000);
    }

    /* External (cartridge) RAM */
    if (((0xA000 <= address) && (address <= 0xBFFF))) {
        return gb.cartridge->read(address);
    }

    /* Internal work RAM */
    if (((0xC000 <= address) && (address <= 0xDFFF))) {
        return WRAM.at(address - 0xC000);
    }

    if (((0xE000 <= address) && (address <= 0xFDFF))) {
        /* affiche("Attempting to read from mirrored work RAM"); */
        return read(address - 0x2000);
    }

    /* OAM */
    if (((0xFE00 <= address) && (address <= 0xFE9F))) {
        return OAM.at(address - 0xFE00);
    }

    if (((0xFEA0 <= address) && (address <= 0xFEFF))) {
        affiche("Attempting to read from unusable memory 0x%x", address);
        return 0xFF;
    }

    /* Mapped IO */
    if (((0xFF00 <= address) && (address <= 0xFF7F))) {
        return read_io(address);
    }

    /* Zero Page ram */
    if (((0xFF80 <= address) && (address <= 0xFFFE))) {
        return HRAM.at(address - 0xFF80);
    }

    /* Interrupt Enable register */
    if (address == 0xFFFF) {
        return gb.cpu.interrupt_enabled.value();
    }

    affiche("Attempted to read from unmapped memory address 0x%X", address);
}



auto Memory::read_io(u16 address) {
    switch (address) {
        case 0xFF00:
            return gb.input.get_input();

        case 0xFF04:
        case 0xFF05:
        case 0xFF06:
        case 0xFF07:
            return gb.timer.read(address);


        case 0xFF0F:
            return gb.cpu.interrupt_flag.value();

        /* TODO: Audio - Channel 1: Tone & Sweep */
        case 0xFF10:
        case 0xFF11:
        case 0xFF12:
        case 0xFF13:
        case 0xFF14:
            return 0xFF;

        case 0xFF15:
            return unmapped_io_read(address);

        /* TODO: Audio - Channel 2: Tone */
        case 0xFF16:
        case 0xFF17:
        case 0xFF18:
        case 0xFF19:
            return 0xFF;

        /* TODO: Audio - Channel 3: Wave Output */
        case 0xFF1A:
        case 0xFF1B:
        case 0xFF1C:
        case 0xFF1D:
        case 0xFF1E:
        case 0xFF1F:
            return 0xFF;

        /* TODO: Audio - Channel 4: Noise */
        case 0xFF20:
        case 0xFF21:
        case 0xFF22:
        case 0xFF23:
            return 0xFF;

        /* TODO: Audio - Channel control/ON-OFF/Volume */
        case 0xFF24:
            return 0xFF;

        /* TODO: Audio - Selection of sound output terminal */
        case 0xFF25:
            return 0xFF;

        /* TODO: Audio - Sound on/off */
        case 0xFF26:
            return 0xFF;

        case 0xFF27:
        case 0xFF28:
        case 0xFF29:
        case 0xFF2A:
        case 0xFF2B:
        case 0xFF2C:
        case 0xFF2D:
        case 0xFF2E:
        case 0xFF2F:
            return unmapped_io_read(address);

        /* TODO: Audio - Wave pattern RAM */
        case 0xFF30:
        case 0xFF31:
        case 0xFF32:
        case 0xFF33:
        case 0xFF34:
        case 0xFF35:
        case 0xFF36:
        case 0xFF37:
        case 0xFF38:
        case 0xFF39:
        case 0xFF3A:
        case 0xFF3B:
        case 0xFF3C:
        case 0xFF3D:
        case 0xFF3E:
        case 0xFF3F:
            return 0xFF;

        case 0xFF40:
            return gb.video.control_byte;

        case 0xFF41:
            return gb.video.lcd_status.value();

        case 0xFF42:
            return gb.video.scroll_y.value();

        case 0xFF43:
            return gb.video.scroll_x.value();

        case 0xFF44:
            return gb.video.line.value();

        case 0xFF45:
            return gb.video.ly_compare.value();

        case 0xFF46:
            affiche("Attempted to read from write-only DMA Transfer/Start Address (0xFF46)");
            return 0xFF;

        case 0xFF47:
            return gb.video.bg_palette.value();

        case 0xFF48:
            return gb.video.sprite_palette_0.value();

        case 0xFF49:
            return gb.video.sprite_palette_1.value();

        case 0xFF4A:
            return gb.video.window_y.value();

        case 0xFF4B:
            return gb.video.window_x.value();

        /* Disable boot rom switch */
        case 0xFF50:
            return disable_boot_rom_switch.value();


        default:
            return unmapped_io_read(address);
    }
}

auto Memory::unmapped_io_read(u16 address){
    affiche("Attempting to read from unused IO address 0x%x", address);
    return 0xFF;
}











void Memory::write(const u16 address, const u8 data) {
    if (((0x0000 <= address) && (address <= 0x7FFF))) {
        gb.cartridge->write(address, data);
        return;
    }

    /* VRAM */
    if (((0x8000 <= address) && (address <= 0x9FFF))) {
        gb.video.write(address - 0x8000, data);
        return;
    }

    /* External (cartridge) RAM */
    if (((0xA000 <= address) && (address <= 0xBFFF))) {
        gb.cartridge->write(address, data);
        return;
    }

    /* Internal work RAM */
    if (((0xC000 <= address) && (address <= 0xDFFF))) {
        WRAM.at(address - 0xC000) = data;
        return;
    }

    /* Mirrored RAM */
    if (((0xE000 <= address) && (address <= 0xFDFF))) {
        affiche("Attempting to write to mirrored work RAM");
        write(address - 0x2000, data);
        return;
    }

    /* OAM */
    if (((0xFE00 <= address) && (address <= 0xFE9F))) {
        OAM.at(address - 0xFE00) = data;
        return;
    }

    if (((0xFEA0 <= address) && (address <= 0xFEFF))) {
        affiche("Attempting to write to unusable memory 0x%x - 0x%x", address, data);
        return;
    }

    /* Mapped IO */
    if (((0xFF00 <= address) && (address <= 0xFF7F))) {
        write_io(address, data);
        return;
    }

    /* Zero Page ram */
    if (((0xFF80 <= address) && (address <= 0xFFFE))) {
        HRAM.at(address - 0xFF80) = data;
        return;
    }

    /* Interrupt Enable register */
    if (address == 0xFFFF) {
        gb.cpu.interrupt_enabled.set(data);
        return;
    }

    affiche("Attempted to write to unmapped memory address 0x%X", address);
}




void Memory::write_io(const u16 address, const u8 data) {
    switch (address) {
        case 0xFF00:
            gb.input.write(data);
            return;


        case 0xFF04:
        case 0xFF05:
        case 0xFF06:
        case 0xFF07:
            gb.timer.write(address,data);
            return;



        case 0xFF0F:
            gb.cpu.interrupt_flag.set(data);
            return;

        /* TODO: Audio - Channel 1: Tone & Sweep */
        case 0xFF10:
        case 0xFF11:
        case 0xFF12:
        case 0xFF13:
        case 0xFF14:
            return;


        /* TODO: Audio - Channel 2: Tone */
        case 0xFF16:
        case 0xFF17:
        case 0xFF18:
        case 0xFF19:
            return;

        /* TODO: Audio - Channel 3: Wave Output */
        case 0xFF1A:
        case 0xFF1B:
        case 0xFF1C:
        case 0xFF1D:
        case 0xFF1E:
            return;


        /* TODO: Audio - Channel 4: Noise */
        case 0xFF20:
        case 0xFF21:
        case 0xFF22:
        case 0xFF23:
            return;

        /* TODO: Audio - Channel control/ON-OFF/Volume */
        case 0xFF24:
            return;

        /* TODO: Audio - Selection of sound output terminal */
        case 0xFF25:
            return;

        /* TODO: Audio - Sound on/off */
        case 0xFF26:
            affiche("Wrote to sound on/off address 0x%x - 0x%x", address, data);
            return;


        /* TODO: Audio - Wave pattern RAM */
        case 0xFF30:
        case 0xFF31:
        case 0xFF32:
        case 0xFF33:
        case 0xFF34:
        case 0xFF35:
        case 0xFF36:
        case 0xFF37:
        case 0xFF38:
        case 0xFF39:
        case 0xFF3A:
        case 0xFF3B:
        case 0xFF3C:
        case 0xFF3D:
        case 0xFF3E:
        case 0xFF3F:
            return;

        /* Switch on LCD */
        case 0xFF40:
            gb.video.control_byte = data;
            return;

        case 0xFF41:
            gb.video.lcd_status.set(data);
            return;

        /* Vertical Scroll Register */
        case 0xFF42:
            gb.video.scroll_y.set(data);
            return;

        /* Horizontal Scroll Register */
        case 0xFF43:
            gb.video.scroll_x.set(data);
            return;

        /* LY - Line Y coordinate */
        case 0xFF44:
            /* "Writing will reset the counter */
            gb.video.line.set(0x0);
            return;

        case 0xFF45:
            gb.video.ly_compare.set(data);
            return;

        case 0xFF46:
            // dma_transfer(data);
            affiche( "Il faut coder le DMA transfert (cf memory.cpp ligne 600)")
            return;

        case 0xFF47:
            gb.video.bg_palette.set(data);
            log_trace("Set video palette: 0x%x", data);
            return;

        case 0xFF48:
            gb.video.sprite_palette_0.set(data);
            log_trace("Set sprite palette 0: 0x%x", data);
            return;

        case 0xFF49:
            gb.video.sprite_palette_1.set(data);
            log_trace("Set sprite palette 1: 0x%x", data);
            return;

        case 0xFF4A:
            gb.video.window_y.set(data);
            return;

        case 0xFF4B:
            gb.video.window_x.set(data);
            return;



        /* Disable boot rom switch */
        case 0xFF50:
            disable_boot_rom_switch.set(data);
            global_logger.enable_tracing();
            affiche("Boot rom was disabled");
            return;


        default:
            unmapped_io_write(address, data);
    }
}

void Memory::unmapped_io_write(const u16 address, const u8 data) {
    affiche("Attempting to write 0x%x to unused IO address 0x%x ", data, address);
}

bool Memory::boot_rom_active() const {
    return read(0xFF50) != 0x1;
}
































int main () {
    affiche("ça compile !");
    Memory memory_test;
    affiche("ça marche 1");
    memory_test.write(static_cast<uint16_t>(0), static_cast<uint8_t>(42));
    affiche("ça marche 2");
    const u8 octet_test = memory_test.read(static_cast<uint16_t>(0));
    affiche(octet_test);



    return 0;
}
