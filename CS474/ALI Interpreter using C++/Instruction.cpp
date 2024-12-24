#include "Instruction.h"
#include "ALI.h"
#include <iostream>

LdaInstruction::LdaInstruction(int addr) : Instruction("LDA"), address(addr) {}

void LdaInstruction::execute(ALI& ali) {
    ali.setAccumulator(ali.getMemoryValue(address));
    ali.incrementPC();
}

DecInstruction::DecInstruction() : Instruction("DEC") {}

void DecInstruction::execute(ALI& ali) {
    int result = ali.getAccumulator() - 1;
    if (result > 32767) {
        ali.setAccumulator(32767); 
    } else if (result < -32768) {
        ali.setAccumulator(-32768);
    } else {
        ali.setAccumulator(result);
    }
    ali.setZeroBit(result == 0);
    ali.incrementPC();
}


LdiInstruction::LdiInstruction(int val) : Instruction("LDI"), value(val) {}

void LdiInstruction::execute(ALI& ali) {
    ali.setAccumulator(value);
    std::cout << "val";
    ali.incrementPC();
}

StrInstruction::StrInstruction(int addr) : Instruction("STR"), address(addr) {}

void StrInstruction::execute(ALI& ali) {
    ali.setMemoryValue(address, ali.getAccumulator());
    ali.incrementPC();
}

XchInstruction::XchInstruction() : Instruction("XCH") {}

void XchInstruction::execute(ALI& ali) {
    int temp = ali.getAccumulator();
    ali.setAccumulator(ali.getDataRegister());  //swap
    ali.setDataRegister(temp);
    ali.incrementPC();
}

JmpInstruction::JmpInstruction(int addr) : Instruction("JMP"), address(addr) {}

void JmpInstruction::execute(ALI& ali) {
    ali.setPC(address);
}

JzsInstruction::JzsInstruction(int addr) : Instruction("JZS"), address(addr) {}

void JzsInstruction::execute(ALI& ali) {
    if (ali.isZeroBitSet()) {
        ali.setPC(address);
    } else {
        ali.incrementPC();
    }
}

AddInstruction::AddInstruction() : Instruction("ADD") {}

void AddInstruction::execute(ALI& ali) {
    int result = ali.getAccumulator() + ali.getDataRegister();
    if (result > 32767 || result < -32768) {
        //check for overflow and does nothing
    } else {
        ali.setAccumulator(result);
        ali.setZeroBit(result == 0);
    }
    ali.incrementPC();
}

SubInstruction::SubInstruction() : Instruction("SUB") {}

void SubInstruction::execute(ALI& ali) {
    int result = ali.getAccumulator() - ali.getDataRegister();
    if (result > 32767 || result < -32768) {
        //check for overflow and does nothing
    } else {
        ali.setAccumulator(result);
        ali.setZeroBit(result == 0);
    }
    ali.incrementPC();
}

HltInstruction::HltInstruction() : Instruction("HLT") {}

void HltInstruction::execute(ALI& ali) {
    ali.haltExecution(); //ends program
}