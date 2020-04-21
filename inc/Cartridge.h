//
// Created by janus on 3/21/20.
//
#pragma once
#include <cstdint>
#include <string>
#include <fstream>
#include <vector>
#include <memory>

#include "Mapper_000.h"
class Cartridge
{
public:
    explicit Cartridge(const std::string &sFileName);
    ~Cartridge() = default;

    bool imageValid();

    enum MIRROR
    {
        HORIZONTAL,
        VERTICAL,
        ONESCREEN_LO,
        ONESCREEN_HI,
    } mirror = HORIZONTAL;

    bool cpuRead(uint16_t addr, uint8_t &data);
    bool cpuWrite(uint16_t addr, uint8_t data);

    bool ppuRead(uint16_t addr, uint8_t &data);
    bool ppuWrite(uint16_t addr, uint8_t data);

private:
    bool bImageValid = false;
    std::vector<uint8_t> vPRGMemory;
    std::vector<uint8_t> vCHRMemory;

    uint8_t nMapperID;
    uint8_t nPRGBanks;
    uint8_t nCHRBanks;

    std::shared_ptr<Mapper> pMapper;
};