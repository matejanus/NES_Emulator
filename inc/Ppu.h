//
// Created by janus on 3/21/20.
//
//2C02
#pragma once

#include <cstdint>
#include <memory>
#include "Cartridge.h"

#include "olcPixelGameEngine.h"

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

    std::array<std::array<uint8_t, 2>, 1024> name; //name tables in VRAM = 2 * 1KB
    std::array<uint8_t, 32> palette; //RAM for pallets
    std::array<std::array<uint8_t, 2>, 4096>pattern;

    //for display
    olc::Pixel palScreen[0x40];
    olc::Sprite sprScreen = olc::Sprite(256, 240);
    olc::Sprite sprNameTable[2] = {olc::Sprite(256, 240), olc::Sprite(256, 240)};
    olc::Sprite sprPatternTable[2] = {olc::Sprite(128, 128), olc::Sprite(128, 128)};
public:
    olc::Sprite& GetScreen();
    olc::Sprite& GetNameTable(uint8_t i);
    olc::Sprite& GetPatternTable(uint8_t i);
    bool frame_complete = false;
private:
    int16_t scanline = 0;
    int16_t cycle = 0;
};