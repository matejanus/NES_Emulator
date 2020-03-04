//
// Created by janus on 3/3/20.
//

#include "Bus.h"

Bus::Bus()
{
    cpu.connectBus(this);
}

Bus::~Bus()
{

}
void Bus::write(uint16_t addr, uint8_t data){
        ram[addr] = data;
}

uint8_t Bus::read(uint16_t addr, bool bReadOnly) {
       return ram[addr];
}
