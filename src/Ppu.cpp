//
// Created by janus on 3/21/20.
//

#include "Ppu.h"
Ppu::Ppu() = default;
Ppu::~Ppu() = default;

uint8_t Ppu::cpuRead(uint16_t addr, bool rdonly) {
    (void)rdonly;
    uint8_t data = 0x00;

    switch(addr) {
    case 0x0000:  //Control
        break;
    case 0x0001:  //Mask
        break;
    case 0x0002:  //Status
        break;
    case 0x0003:  //OAM address
        break;
    case 0x0004:  //OAM Data
        break;
    case 0x0005:  //Scroll
        break;
    case 0x0006:  //PPU Address
        break;
    case 0x0007:  //PPU data
        break;
    default:
        break;
    }

    return data;
}
void Ppu::cpuWrite(uint16_t addr, uint8_t data) {
    (void)data;
    switch(addr) {
    case 0x0000:  //Control
        break;
    case 0x0001:  //Mask
        break;
    case 0x0002:  //Status
        break;
    case 0x0003:  //OAM address
        break;
    case 0x0004:  //OAM Data
        break;
    case 0x0005:  //Scroll
        break;
    case 0x0006:  //PPU Address
        break;
    case 0x0007:  //PPU data
        break;
    default:
        break;
    }
}
uint8_t Ppu::ppuRead(uint16_t addr, bool rdonly) {
    (void)rdonly;
    uint8_t data = 0x00;
    addr &= 0x3FFF;

    if(cart->ppuRead(addr, data))
    {

    }

    return data;  //mock
}
void Ppu::ppuWrite(uint16_t addr, uint8_t data) {
    (void)data;
    addr &= 0x3FFF;

    if(cart->ppuWrite(addr, data))
    {

    }

}
void Ppu::connectCartridge(const std::shared_ptr<Cartridge>& cartridge) {
    this->cart = cartridge;
}
void Ppu::clock() {
}
