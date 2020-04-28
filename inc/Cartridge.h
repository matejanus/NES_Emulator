//
// Created by janus on 3/21/20.
//
#pragma once
#include <cstdint>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "Mapper_000.h"
class Cartridge {
public:
    explicit Cartridge(const std::string &sFileName);
    ~Cartridge() = default;

    bool imageValid();
    bool cpuRead(uint16_t addr, uint8_t &data);
    bool cpuWrite(uint16_t addr, uint8_t data);
    // Communication with PPU Bus
    bool ppuRead(uint16_t addr, uint8_t &data);
    bool ppuWrite(uint16_t addr, uint8_t data);
    // Permits system rest of mapper to know state
    void reset();

    enum MIRROR {
        HORIZONTAL,
        VERTICAL,
        ONESCREEN_LO,
        ONESCREEN_HI,
    } mirror = HORIZONTAL;

private:
    bool bImageValid = false;

    uint8_t nMapperID = 0;
    uint8_t nPRGBanks = 0;
    uint8_t nCHRBanks = 0;

    std::vector<uint8_t> vPRGMemory;
    std::vector<uint8_t> vCHRMemory;
    std::shared_ptr<Mapper> pMapper;
};