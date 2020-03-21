//
// Created by janus on 3/21/20.
//

#pragma once

#include <cstdint>

class Ppu {
public:
    Ppu();

    ~Ppu();

    uint8_t cpuRead(uint16_t addr, bool rdonly = false);
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t ppuRead(uint16_t addr, bool rdonly = false);
    void ppuWrite(uint16_t addr, uint8_t data);
};