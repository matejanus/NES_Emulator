//
// Created by janus on 3/21/20.
//
//2C02
#pragma once

#include <cstdint>
#include <memory>

#include "Cartridge.h"

class Ppu {
public:
    Ppu();
    ~Ppu();

    uint8_t cpuRead(uint16_t addr, bool rdonly = false);
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t ppuRead(uint16_t addr, bool rdonly = false);
    void ppuWrite(uint16_t addr, uint8_t data);

    void connectCartridge(const std::shared_ptr<Cartridge> &cartridge);
    void clock();
private:
    std::shared_ptr<Cartridge> cart;

    uint8_t name[2][1024]; //name tables in VRAM = 2 * 1KB
    uint8_t palette[32]; //RAM for pallets
    uint8_t pattern[32]; //TODO:for later

};