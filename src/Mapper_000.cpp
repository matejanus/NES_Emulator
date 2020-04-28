//
// Created by janus on 4/20/20.
//
#include "Mapper_000.h"

Mapper_000::Mapper_000(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks) {
}

bool Mapper_000::cpuMapRead(uint16_t addr, uint32_t& mappedAddress) {
    if(addr >= 0x8000) {
        mappedAddress = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }
    return false;
}
bool Mapper_000::cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data) {
    (void)data;
    if(addr >= 0x8000) {
        mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }

    return false;
}
bool Mapper_000::ppuMapRead(uint16_t addr, uint32_t& mappedAddress) {
    if(addr <= 0x1FFF) {
        mappedAddress = addr;
        return true;
    }
    return false;
}
bool Mapper_000::ppuMapWrite(uint16_t addr, uint32_t& mappedAddress) {
    if(addr <= 0x1FFF) {
        if(nCHRBanks == 0) {
            // Treat as RAM
            mappedAddress = addr;
            return true;
        }
    }

    return false;
}
void Mapper_000::reset() {
}
Mapper_000::~Mapper_000() = default;
