//
// Created by janus on 3/21/20.
//
#include "Cartridge.h"

#include <fstream>

Cartridge::Cartridge(const std::string& sFileName) {
    // iNES Format Header
    struct sHeader {
        char name[4];
        uint8_t prg_rom_chunks;
        uint8_t chr_rom_chunks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prg_ram_size;
        uint8_t tv_system1;
        uint8_t tv_system2;
        char unused[5];
    } header{};

    std::ifstream ifs;
    ifs.open(sFileName, std::ifstream::binary);
    if(ifs.is_open()) {
        ifs.read(reinterpret_cast<char*>(&header), sizeof(sHeader));

        if(header.mapper1 & 0x04)
            ifs.seekg(512, std::ios_base::cur);

        //determine mapper ID
        nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);

        //Discover file format
        uint8_t nFileType = 1;

        if(nFileType == 0) {
            //for later
        }
        if(nFileType == 1) {
            nPRGBanks = header.prg_rom_chunks;
            v_PRGMemory.resize(nPRGBanks * 16384);  //16KB
            ifs.read(reinterpret_cast<char*>(v_PRGMemory.data()), v_PRGMemory.size());

            nCHRBanks = header.chr_rom_chunks;
            v_CHRMemory.resize(nCHRBanks * 8192);  //8KB
            ifs.read(reinterpret_cast<char*>(v_CHRMemory.data()), v_CHRMemory.size());
        }

        if(nFileType == 2) {
            //for later
        }

        switch(nMapperID) {
        case 0:
            pMapper = std::make_unique<Mapper_000>(nPRGBanks, nCHRBanks);
            break;
        }
        ifs.close();
    }
}

bool Cartridge::cpuRead(uint16_t addr, uint8_t& data) {
    return false;
}
bool Cartridge::cpuWrite(uint16_t addr, uint8_t data) {
    return false;
}
bool Cartridge::ppuRead(uint16_t addr, uint8_t& data) {
    return false;
}
bool Cartridge::ppuWrite(uint16_t addr, uint8_t data) {
    return false;
}
