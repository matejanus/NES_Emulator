//
// Created by janus on 3/3/20.
//
#include "Bus.h"

Bus::Bus() {
    cpu.connectBus(this);
}

void Bus::cpuWrite(uint16_t addr, uint8_t data) {

    if(cart->cpuWrite(addr, data))
    {
        //for later
    }
    else if(addr <= 0x1FFF)
        cpuRam[addr & 0x07FF] = data;
    else if(addr >= 0x2000 && addr <= 0x3FFF)
        ppu.cpuWrite(addr & 0x0007, data);
}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly) {

    uint8_t data = 0x00;
    if(cart->cpuRead(addr, data))
    {
        //for later
    }
    else if(addr <= 0x1FFF)
        data = cpuRam[addr & 0x07FF];
    else if(addr >= 0x2000 && addr <= 0x3FFF)
        data = ppu.cpuRead(addr & 0x0007, bReadOnly);
    return data;
}
void Bus::reset() {
    cpu.reset();
    nSystemClockCounter = 0;
}
void Bus::clock() {
    ppu.clock();

    // The CPU runs 3 times slower than the PPU so we only call its
    // clock() function every 3 times this function is called. We
    // have a global counter to keep track of this.
    if (nSystemClockCounter % 3 == 0)
    {
        cpu.clock();
    }

    nSystemClockCounter++;
}
void Bus::insertCartridge(const std::shared_ptr<Cartridge> &cartridge) {
    this->cart = cartridge;
    ppu.connectCartridge(cartridge);
}
