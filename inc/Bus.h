//
// Created by janus on 3/3/20.
//
#pragma once
#include <array>
#include <cstdint>

#include "Cartridge.h"
#include "Ppu.h"
#include "cpu6502.h"

class Bus {
public:
    Bus();
    ~Bus() = default;

    cpu6502 cpu;
    Ppu ppu;

    std::shared_ptr<Cartridge> cart;
    std::array<uint8_t, 2048> cpuRam{};
    std::array<uint8_t, 2> controller{};

public:  // Main Bus Read & Write
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);
    void insertCartridge(const std::shared_ptr<Cartridge> &cartridge);
    void reset();  //as a reset button
    void clock();  //system clock tick

private:
    uint32_t nSystemClockCounter;
    std::array<uint8_t, 2> controller_state{};
};
