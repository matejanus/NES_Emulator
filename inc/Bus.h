//
// Created by janus on 3/3/20.
//

#pragma once

#include <array>
#include <cstdint>
//#include <memory>

#include "Cartridge.h"
#include "Ppu.h"
#include "cpu6502.h"

class Bus {
public:
    Bus();
    ~Bus();

    cpu6502 cpu;
    Ppu ppu;

    std::shared_ptr<Cartridge> cart;

//    std::array<uint8_t , 2048> cpuRam{};
    uint8_t cpuRam[2048];

    uint8_t controller[2];
public: // Main Bus Read & Write

    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

    void insertCartridge(const std::shared_ptr<Cartridge> &cartridge);
    void reset(); //as a reset button
    void clock(); //system clock tick

private:
    uint32_t nSystemClockCounter;
    uint8_t controller_state[2]{};
};

