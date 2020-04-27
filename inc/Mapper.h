//
// Created by janus on 4/20/20.
//

#pragma once
#include <cstdint>

// Abstract base class

class Mapper{
public:
    Mapper(uint8_t prgBanks, uint8_t chrBanks);
    virtual ~Mapper() = default;

    virtual bool cpuMapRead(uint16_t addr, uint32_t &mappedAddress) = 0;
    virtual bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr, uint8_t data = 0)	 = 0;
    virtual bool ppuMapRead(uint16_t addr, uint32_t &mappedAddress) = 0;
    virtual bool ppuMapWrite(uint16_t addr, uint32_t &mappedAddress) = 0;

    virtual void reset() = 0;

protected:
    uint8_t nPRGBanks = 0;
    uint8_t nCHRBanks = 0;

};