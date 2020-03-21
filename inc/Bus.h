//
// Created by janus on 3/3/20.
//

#pragma once

#include <cstdint>
#include <array>
#include "cpu6502.h"
#include "Ppu.h"

class Bus {
public:
    Bus();
    ~Bus();
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

    cpu6502 cpu;

    Ppu ppu;
    std::array<uint8_t , 2048> cpuRam{};
};

