//
// Created by janus on 3/21/20.
//
#pragma once
#include <cstdint>
#include <string>
#include <vector>
class Cartridge
{
public:
    Cartridge(const std::string &sFileName);
    ~Cartridge() = default;

    bool cpuRead(uint16_t addr, uint8_t &data);
    bool cpuWrite(uint16_t addr, uint8_t data);

    bool ppuRead(uint16_t addr, uint8_t &data);
    bool ppuWrite(uint16_t addr, uint8_t data);

private:
    std::vector<uint8_t> v_PRGMemory;
    std::vector<uint8_t> v_CHRMemory;

    uint8_t nMapperID = 0;
    uint8_t nPRGBanks = 0;
    uint8_t nCHRBanks = 0;

};