//
// Created by janus on 4/20/20.
//

#pragma once
#include "Mapper.h"

class Mapper_000 : public Mapper {
public:
    Mapper_000(uint8_t prgBanks, uint8_t chrBanks);
    ~Mapper_000() override;

    bool cpuMapRead(uint16_t addr, uint32_t &mappedAddress) override;
    bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr, uint8_t data) override;

    bool ppuMapRead(uint16_t addr, uint32_t &mappedAddress) override;
    bool ppuMapWrite(uint16_t addr, uint32_t &mappedAddress) override;
    void reset() override;
};