//
// Created by janus on 3/3/20.
//

#pragma once

#include <cstdint>
#include <array>
#include "cpu6502.h"

class Bus {
public:
    Bus();
    ~Bus();
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr, bool bReadOnly = false);

    cpu6502 cpu;
    std::array<uint8_t , 64*1024> ram{};
};

