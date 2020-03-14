//
// Created by janus on 3/3/20.
//

#include <Bus.h>
#include "cpu6502.h"

namespace {
    constexpr uint16_t stackOffset = 0x100;
    constexpr uint16_t programBaseAddress = 0xFFFC;
}

cpu6502::cpu6502():fetched(0x00), addr_abs(0x0000), addr_rel(0x00),opcode(0x00),cycles(0x00), acc(0x00),x(0x00), y(0x00), sp(0x00), pc(0x0000), st(0x00), bus(nullptr)
{
    using a = cpu6502;
    // http://archive.6502.org/datasheets/rockwell_r650x_r651x.pdf
lookup = {{
    { "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
    { "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
    { "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
    { "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
    { "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
    { "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
    { "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
    { "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
    { "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
    { "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
    { "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
    { "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
    { "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
    { "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
    { "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
    { "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
}};

}

void cpu6502::connectBus(Bus *b) {
    bus = b;
}

void cpu6502::write(uint16_t addr, uint8_t data) {
    bus->write(addr,data);
}

uint8_t cpu6502::read(uint16_t addr, bool bReadOnly) {
    return bus->read(addr, bReadOnly);
}

void cpu6502::clock() {

    if (cycles == 0) {
        opcode = read(pc);

    #ifdef LOGMODE
        uint16_t log_pc = pc;
    #endif
        pc++;

        cycles = lookup[opcode].cycles;

        uint8_t additionalCicle1 = (this->*lookup[opcode].addrmode)();
        uint8_t additionalCicle2 = (this->*lookup[opcode].opperate)();
        cycles += (additionalCicle1 && additionalCicle2);
        setFlag(U, true);

        #ifdef LOGMODE
                // This logger dumps every cycle the entire processor state for analysis.
                // This can be used for debugging the emulation, but has little utility
                // during emulation. Its also very slow, so only use if you have to.
                if (logfile == nullptr)	logfile = fopen("6502.txt", "wt");
                if (logfile != nullptr)
                {
                    fprintf(logfile, "%10d:%02d PC:%04X %s A:%02X X:%02X Y:%02X %s%s%s%s%s%s%s%s STKP:%02X\n",
                        clock_count, 0, log_pc, "XXX", acc, x, y,
                        getFlag(N) ? "N" : ".",	getFlag(V) ? "V" : ".",	getFlag(U) ? "U" : ".",
                        getFlag(B) ? "B" : ".",	getFlag(D) ? "D" : ".",	getFlag(I) ? "I" : ".",
                        getFlag(Z) ? "Z" : ".",	getFlag(C) ? "C" : ".",	sp);
                }
        #endif
    }
    clock_count++;
    cycles--;
}


void cpu6502::setFlag(cpu6502::flags f, bool v)
{
    if(v)
        st |= f;
    else
        st &= ~f;
}

uint8_t cpu6502::getFlag(cpu6502::flags f) {
    return ((st & f) > 0) ? 1 : 0;
}

uint8_t cpu6502::IMP() {
    fetched = acc;
    return 0;
}

uint8_t cpu6502::IMM()
{
    addr_abs = pc++;
    return 0;
}

uint8_t cpu6502::ZP0()
{
    addr_abs = read(pc);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t cpu6502::ZPX()
{
    addr_abs = (read(pc) + x);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t cpu6502::ZPY()
{
    addr_abs = (read(pc) + y);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t cpu6502::ABS()
{
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;
    addr_abs = ((hi << 8) | lo);

    return 0;
}

uint8_t cpu6502::ABX()
{
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;
    addr_abs = ((hi << 8) | lo);
    addr_abs += x;

    if ((addr_abs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

uint8_t cpu6502::IND()
{
    uint16_t ptr_lo = read(pc);
    pc++;
    uint16_t ptr_hi = read(pc);
    pc++;

    uint16_t  ptr = ((ptr_hi << 8) | ptr_lo);
    if (ptr_lo == 0x00FF) //simulate hardware bug
    {
        addr_abs += (read(0xFF00) << 8)| read(ptr);
    }
    else{ //noral baheaviour
        addr_abs += (read(ptr + 1) << 8)| read(ptr);
    }
    return 0;
}

uint8_t cpu6502::IZX()
{
    uint16_t t = read(pc);
    pc++;

    uint16_t lo = read(static_cast<uint16_t>(t + static_cast<uint16_t>(x)) & 0x00FF);
    uint16_t hi = read(static_cast<uint16_t>(t + static_cast<uint16_t>(x) + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;

    return 0;
}

uint8_t cpu6502::IZY()
{
    uint16_t t = read(pc);
    pc++;

    uint16_t lo = read(t & 0x00FF);
    uint16_t hi = read((t + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;
    addr_abs += y;

    if ((addr_abs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

uint8_t cpu6502::REL()
{
    addr_rel = read(pc);
    pc++;

    if (addr_rel & 0x80)
        addr_rel |= 0xFF00;
    return 0;
}

uint8_t  cpu6502::fetch() {
    if (lookup[opcode].addrmode != &cpu6502::IMP)
        fetched = read(addr_abs);
    return fetched;
}

uint8_t cpu6502::AND() {

    fetch();
    acc = acc & fetched;
    setFlag(Z, acc == 0x00);
    setFlag(N, acc & 0x80); // negative number
    return 1;
}

uint8_t cpu6502::BCS(){

    if (getFlag(C))
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t cpu6502::BCC(){

    if (!getFlag(C))
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t cpu6502::BEQ(){

    if (getFlag(Z))
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t cpu6502::BMI(){

    if (getFlag(N))
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t cpu6502::BNE(){

    if (getFlag(Z) == 0)
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t cpu6502::BPL(){

    if (!getFlag(N))
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t cpu6502::BVC(){

    if (!getFlag(V))
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t cpu6502::BVS(){

    if (getFlag(V))
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t cpu6502::CLC(){
    setFlag(C, false);
    return 0;
}

uint8_t cpu6502::CLD(){
    setFlag(D, false);
    return 0;
}

uint8_t cpu6502::CLI(){
    setFlag(I, false);
    return 0;
}
uint8_t cpu6502::CLV(){
    setFlag(V, false);
    return 0;
}

uint8_t cpu6502::ADC(){
    fetch();
    uint16_t temp = static_cast<uint16_t>(acc) + static_cast<uint16_t>(fetched) + static_cast<uint16_t>(getFlag(C));
    setFlag(C, temp > 255);
    setFlag(Z, (temp & 0x00FF) == 0);
    setFlag(N, temp & 0x80);
    setFlag(V, (~(static_cast<uint16_t>(acc) ^ static_cast<uint16_t>(fetched)) & (static_cast<uint16_t>(acc) ^ static_cast<uint16_t>(temp))) & 0x0080);

//    setFlag(V, (~((uint16_t)acc ^ (uint16_t)fetched) & ((uint16_t)acc ^ (uint16_t)temp)) & 0x0080);
    acc = temp & 0x00FF;
    return 1;
}


uint8_t cpu6502::SBC(){
    fetch();
    uint16_t value = static_cast<uint16_t>(fetched) ^ 0x00FF;

    uint16_t temp = static_cast<uint16_t>(acc) + value + static_cast<uint16_t>(getFlag(C));
    setFlag(C, temp > 255);
    setFlag(Z, (temp & 0x00FF) == 0);
    setFlag(N, temp & 0x80);
    setFlag(V, (((temp ^ static_cast<uint16_t>(acc)) & (temp ^ value)) & 0x0080));
    acc = temp & 0x00FF;
    return 1;
}

uint8_t cpu6502::PHA() {
    write(stackOffset + sp, acc);
    sp--;
    return 0;
}

uint8_t cpu6502::PLA() {
    sp++;
    read(stackOffset + sp);
    setFlag(Z, acc == 0);
    setFlag(N, acc & 0x80);
    return 0;
}

void cpu6502::reset() {
    acc = 0;
    x = 0;
    y = 0;
    sp = 0xFD;
    st = 0x00 | U;

    uint16_t  lo = read(programBaseAddress);
    uint16_t  hi = read(programBaseAddress + 1);

    pc = (hi << 8) | lo;

    addr_rel = 0x0000;
    addr_abs = 0x0000;
    fetched = 0x0000;

    cycles = 8;
}

void cpu6502::irq() {

    if(getFlag(I))
    {
        write(programBaseAddress + sp, (pc >> 8) & 0x00FF);
        sp--;
        write(programBaseAddress + sp, pc & 0x00FF);
        sp--;

        setFlag(B, false);
        setFlag(U, true);
        setFlag(I, true);
        sp--;

        addr_abs = 0xFFFE;
        uint16_t lo = read(addr_abs + 0);
        uint16_t hi = read(addr_abs + 1);
        pc = (hi << 8) | lo;

        cycles = 7;
    }
}

void cpu6502::nmi() {

        write(programBaseAddress + sp, (pc >> 8) & 0x00FF);
        sp--;
        write(programBaseAddress + sp, pc & 0x00FF);
        sp--;

        setFlag(B, false);
        setFlag(U, true);
        setFlag(I, true);
        sp--;

        addr_abs = 0xFFFE;
        uint16_t lo = read(addr_abs + 0);
        uint16_t hi = read(addr_abs + 1);
        pc = (hi << 8) | lo;

        cycles = 7;
}

uint8_t cpu6502::RTI()
{
    sp++;
    st = read(programBaseAddress + sp);
    st &= ~B;
    st &= ~U;
    return 0;
}


uint8_t cpu6502::NOP() {
    return 0;
}


uint8_t cpu6502::XXX()
{
    return 0;
}

// Instruction: Increment Value at Memory Location
// Function:    M = M + 1
// Flags Out:   N, Z
uint8_t cpu6502::INC()
{
    fetch();
    uint16_t temp = fetched + 1;
    write(addr_abs, temp & 0x00FF);
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(N, temp & 0x0080);
    return 0;
}

// Instruction: Bitwise Logic XOR
// Function:    A = A xor M
// Flags Out:   N, Z
uint8_t cpu6502::EOR()
{
    fetch();
    acc = acc ^ fetched;
    setFlag(Z, acc == 0x00);
    setFlag(N, acc & 0x80);
    return 1;
}

// Address Mode: Absolute with Y Offset
// Fundamentally the same as absolute addressing, but the contents of the Y Register
// is added to the supplied two byte address. If the resulting address changes
// the page, an additional clock cycle is required
uint8_t cpu6502::ABY()
{
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    addr_abs += y;

    if ((addr_abs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

// Instruction: Set Decimal Flag
// Function:    D = 1
uint8_t cpu6502::SED()
{
    setFlag(D, true);
    return 0;
}

// Instruction: Compare X Register
// Function:    C <- X >= M      Z <- (X - M) == 0
// Flags Out:   N, C, Z
uint8_t cpu6502::CPX()
{
    fetch();
    uint16_t  temp = (uint16_t)x - (uint16_t)fetched;
    setFlag(C, x >= fetched);
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(N, temp & 0x0080);
    return 0;
}


// Instruction: Increment X Register
// Function:    X = X + 1
// Flags Out:   N, Z
uint8_t cpu6502::INX()
{
    x++;
    setFlag(Z, x == 0x00);
    setFlag(N, x & 0x80);
    return 0;
}

uint8_t cpu6502::DEC()
{
    fetch();
    uint16_t temp = fetched - 1;
    write(addr_abs, temp & 0x00FF);
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(N, temp & 0x0080);
    return 0;
}

// Instruction: Compare Accumulator
// Function:    C <- A >= M      Z <- (A - M) == 0
// Flags Out:   N, C, Z
uint8_t cpu6502::CMP()
{
    fetch();
    uint16_t temp = (uint16_t)acc - (uint16_t)fetched;
    setFlag(C, acc >= fetched);
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(N, temp & 0x0080);
    return 1;
}


// Instruction: Compare Y Register
// Function:    C <- Y >= M      Z <- (Y - M) == 0
// Flags Out:   N, C, Z
uint8_t cpu6502::CPY()
{
    fetch();
    uint16_t temp = (uint16_t)y - (uint16_t)fetched;
    setFlag(C, y >= fetched);
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(N, temp & 0x0080);
    return 0;
}

// Instruction: Decrement X Register
// Function:    X = X - 1
// Flags Out:   N, Z
uint8_t cpu6502::DEX()
{
    x--;
    setFlag(Z, x == 0x00);
    setFlag(N, x & 0x80);
    return 0;
}

// Instruction: Decrement Y Register
// Function:    Y = Y - 1
// Flags Out:   N, Z
uint8_t cpu6502::DEY()
{
    y--;
    setFlag(Z, y == 0x00);
    setFlag(N, y & 0x80);
    return 0;
}

// Instruction: Increment Y Register
// Function:    Y = Y + 1
// Flags Out:   N, Z
uint8_t cpu6502::INY()
{
    y++;
    setFlag(Z, y == 0x00);
    setFlag(N, y & 0x80);
    return 0;
}

// Instruction: Load The Accumulator
// Function:    A = M
// Flags Out:   N, Z
uint8_t cpu6502::LDA()
{
    fetch();
    acc = fetched;
    setFlag(Z, acc == 0x00);
    setFlag(N, acc & 0x80);
    return 1;
}


// Instruction: Load The X Register
// Function:    X = M
// Flags Out:   N, Z
uint8_t cpu6502::LDX()
{
    fetch();
    x = fetched;
    setFlag(Z, x == 0x00);
    setFlag(N, x & 0x80);
    return 1;
}


// Instruction: Load The Y Register
// Function:    Y = M
// Flags Out:   N, Z
uint8_t cpu6502::LDY()
{
    fetch();
    y = fetched;
    setFlag(Z, y == 0x00);
    setFlag(N, y & 0x80);
    return 1;
}


// Instruction: Transfer Accumulator to Y Register
// Function:    Y = A
// Flags Out:   N, Z
uint8_t cpu6502::TAY()
{
    y = acc;
    setFlag(Z, y == 0x00);
    setFlag(N, y & 0x80);
    return 0;
}


// Instruction: Transfer Stack Pointer to X Register
// Function:    X = stack pointer
// Flags Out:   N, Z
uint8_t cpu6502::TSX()
{
    x = sp;
    setFlag(Z, x == 0x00);
    setFlag(N, x & 0x80);
    return 0;
}


// Instruction: Transfer X Register to Accumulator
// Function:    A = X
// Flags Out:   N, Z
uint8_t cpu6502::TXA()
{
    acc = x;
    setFlag(Z, acc == 0x00);
    setFlag(N, acc & 0x80);
    return 0;
}

// Instruction: Transfer Accumulator to X Register
// Function:    X = A
// Flags Out:   N, Z
uint8_t cpu6502::TAX()
{
    x = acc;
    setFlag(Z, x == 0x00);
    setFlag(N, x & 0x80);
    return 0;
}

// Instruction: Transfer X Register to Stack Pointer
// Function:    stack pointer = X
uint8_t cpu6502::TXS()
{
    sp = x;
    return 0;
}

// Instruction: Store Accumulator at Address
// Function:    M = A
uint8_t cpu6502::STA()
{
    write(addr_abs, acc);
    return 0;
}


// Instruction: Store X Register at Address
// Function:    M = X
uint8_t cpu6502::STX()
{
    write(addr_abs, x);
    return 0;
}


// Instruction: Store Y Register at Address
// Function:    M = Y
uint8_t cpu6502::STY()
{
    write(addr_abs, y);
    return 0;
}

// Instruction: Transfer Y Register to Accumulator
// Function:    A = Y
// Flags Out:   N, Z
uint8_t cpu6502::TYA()
{
    acc = y;
    setFlag(Z, acc == 0x00);
    setFlag(N, acc & 0x80);
    return 0;
}


uint8_t cpu6502::ROL()
{
    fetch();
    uint16_t temp = (uint16_t)(fetched << 1) | getFlag(C);
    setFlag(C, temp & 0xFF00);
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(N, temp & 0x0080);
    if (lookup[opcode].addrmode == &cpu6502::IMP)
        acc = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
}

uint8_t cpu6502::ROR()
{
    fetch();
    uint16_t temp = (uint16_t)(getFlag(C) << 7) | (fetched >> 1);
    setFlag(C, fetched & 0x01);
    setFlag(Z, (temp & 0x00FF) == 0x00);
    setFlag(N, temp & 0x0080);
    if (lookup[opcode].addrmode == &cpu6502::IMP)
        acc = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
}

uint8_t cpu6502::LSR()
{
    fetch();
    setFlag(C, fetched & 0x0001);
    uint16_t temp = fetched >> 1;
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(N, temp & 0x0080);
    if (lookup[opcode].addrmode == &cpu6502::IMP)
        acc = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
}

// Instruction: Bitwise Logic OR
// Function:    A = A | M
// Flags Out:   N, Z
uint8_t cpu6502::ORA()
{
    fetch();
    acc = acc | fetched;
    setFlag(Z, acc == 0x00);
    setFlag(N, acc& 0x80);
    return 1;
}


// Instruction: Arithmetic Shift Left
// Function:    A = C <- (A << 1) <- 0
// Flags Out:   N, Z, C
uint8_t cpu6502::ASL()
{
    fetch();
    uint16_t temp = (uint16_t)fetched << 1;
    setFlag(C, (temp & 0xFF00) > 0);
    setFlag(Z, (temp & 0x00FF) == 0x00);
    setFlag(N, temp & 0x80);
    if (lookup[opcode].addrmode == &cpu6502::IMP)
        acc = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
}

// Instruction: Jump To Location
// Function:    pc = address
uint8_t cpu6502::JMP()
{
    pc = addr_abs;
    return 0;
}

// Instruction: Set Interrupt Flag / Enable Interrupts
// Function:    I = 1
uint8_t cpu6502::SEI()
{
    setFlag(I, true);
    return 0;
}

uint8_t cpu6502::BIT()
{
    fetch();
    uint16_t temp = acc & fetched;
    setFlag(Z, (temp & 0x00FF) == 0x00);
    setFlag(N, fetched & (1 << 7));
    setFlag(V, fetched & (1 << 6));
    return 0;
}

uint8_t cpu6502::RTS()
{
    sp++;
    pc = (uint16_t)read(0x0100 + sp);
    sp++;
    pc |= (uint16_t)read(0x0100 + sp) << 8;

    pc++;
    return 0;
}

// Instruction: Set Carry Flag
// Function:    C = 1
uint8_t cpu6502::SEC()
{
    setFlag(C, true);
    return 0;
}

// Instruction: Pop Status Register off Stack
// Function:    Status <- stack
uint8_t cpu6502::PLP()
{
    sp++;
    st = read(0x0100 + sp);
    setFlag(U, 1);
    return 0;
}

// Instruction: Jump To Sub-Routine
// Function:    Push current pc to stack, pc = address
uint8_t cpu6502::JSR()
{
    pc--;
    write(0x0100 + sp, (pc >> 8) & 0x00FF);
    sp--;
    write(0x0100 + sp, pc & 0x00FF);
    sp--;

    pc = addr_abs;
    return 0;
}

// Instruction: Push Status Register to Stack
// Function:    status -> stack
// Note:        Break flag is set to 1 before push
uint8_t cpu6502::PHP()
{
    write(0x0100 + sp, st | B | U);
    setFlag(B, false);
    setFlag(U, false);
    sp--;
    return 0;
}

// Instruction: Break
// Function:    Program Sourced Interrupt
uint8_t cpu6502::BRK()
{
    pc++;

    setFlag(I, 1);
    write(0x0100 + sp, (pc >> 8) & 0x00FF);
    sp--;
    write(0x0100 + sp, pc & 0x00FF);
    sp--;

    setFlag(B, 1);
    write(0x0100 + sp, st);
    st--;
    setFlag(B, 0);

    pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// HELPER FUNCTIONS

bool cpu6502::complete()
{
    return cycles == 0;
}

// This is the disassembly function. Its workings are not required for emulation.
// It is merely a convenience function to turn the binary instruction code into
// human readable form. Its included as part of the emulator because it can take
// advantage of many of the CPUs internal operations to do this.
std::map<uint16_t, std::string> cpu6502::disassemble(uint16_t nStart, uint16_t nStop)
{
    uint32_t addr = nStart;
    uint8_t value = 0x00, lo = 0x00, hi = 0x00;
    std::map<uint16_t, std::string> mapLines;
    uint16_t line_addr = 0;

    // A convenient utility to convert variables into
    // hex strings because "modern C++"'s method with
    // streams is atrocious
    auto hex = [](uint32_t n, uint8_t d)
    {
        std::string s(d, '0');
        for (int i = d - 1; i >= 0; i--, n >>= 4)
            s[i] = "0123456789ABCDEF"[n & 0xF];
        return s;
    };

    // Starting at the specified address we read an instruction
    // byte, which in turn yields information from the lookup table
    // as to how many additional bytes we need to read and what the
    // addressing mode is. I need this info to assemble human readable
    // syntax, which is different depending upon the addressing mode

    // As the instruction is decoded, a std::string is assembled
    // with the readable output
    while (addr <= (uint32_t)nStop)
    {
        line_addr = addr;

        // Prefix line with instruction address
        std::string sInst = "$" + hex(addr, 4) + ": ";

        // Read instruction, and get its readable name
        uint8_t opcode = bus->read(addr, true); addr++;
        sInst += lookup[opcode].name + " ";

        // Get oprands from desired locations, and form the
        // instruction based upon its addressing mode. These
        // routines mimmick the actual fetch routine of the
        // 6502 in order to get accurate data as part of the
        // instruction
        if (lookup[opcode].addrmode == &cpu6502::IMP)
        {
            sInst += " {IMP}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::IMM)
        {
            value = bus->read(addr, true); addr++;
            sInst += "#$" + hex(value, 2) + " {IMM}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::ZP0)
        {
            lo = bus->read(addr, true); addr++;
            hi = 0x00;
            sInst += "$" + hex(lo, 2) + " {ZP0}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::ZPX)
        {
            lo = bus->read(addr, true); addr++;
            hi = 0x00;
            sInst += "$" + hex(lo, 2) + ", X {ZPX}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::ZPY)
        {
            lo = bus->read(addr, true); addr++;
            hi = 0x00;
            sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::IZX)
        {
            lo = bus->read(addr, true); addr++;
            hi = 0x00;
            sInst += "($" + hex(lo, 2) + ", X) {IZX}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::IZY)
        {
            lo = bus->read(addr, true); addr++;
            hi = 0x00;
            sInst += "($" + hex(lo, 2) + "), Y {IZY}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::ABS)
        {
            lo = bus->read(addr, true); addr++;
            hi = bus->read(addr, true); addr++;
            sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::ABX)
        {
            lo = bus->read(addr, true); addr++;
            hi = bus->read(addr, true); addr++;
            sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::ABY)
        {
            lo = bus->read(addr, true); addr++;
            hi = bus->read(addr, true); addr++;
            sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::IND)
        {
            lo = bus->read(addr, true); addr++;
            hi = bus->read(addr, true); addr++;
            sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::REL)
        {
            value = bus->read(addr, true); addr++;
            sInst += "$" + hex(value, 2) + " [$" + hex(addr + value, 4) + "] {REL}";
        }

        // Add the formed string to a std::map, using the instruction's
        // address as the key. This makes it convenient to look for later
        // as the instructions are variable in length, so a straight up
        // incremental index is not sufficient.
        mapLines[line_addr] = sInst;
    }

    return mapLines;
}
