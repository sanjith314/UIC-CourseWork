#include "ALI.h"
#include <iostream>
#include <sstream>

ALI::ALI() : programCounter(0), zeroBit(false), halted(false) {
    std::fill(std::begin(memory), std::end(memory), 0);
    std::fill(std::begin(registers), std::end(registers), 0);
    std::fill(std::begin(instructionMemory), std::end(instructionMemory), nullptr);
}

ALI::~ALI() {
    for (Instruction* instr : instructionMemory) {
        delete instr;
    }
}


void ALI::loadInstruction(int address, const std::string& line) {
    if (address >= 0 && address < MEMORY_SIZE) {
        std::istringstream iss(line);
        std::string opcode;
        std::string operand;
        iss >> opcode >> operand;

        try { if (opcode == "LDA") {  //Added try catch for debugging an error
            instructionMemory[address] = new LdaInstruction(std::stoi(operand));
        } else if (opcode == "LDI") {
            instructionMemory[address] = new LdiInstruction(std::stoi(operand));
        } else if (opcode == "STR") {
            instructionMemory[address] = new StrInstruction(std::stoi(operand));
        } else if (opcode == "XCH") {
            instructionMemory[address] = new XchInstruction();
        } else if (opcode == "JMP") {
            instructionMemory[address] = new JmpInstruction(std::stoi(operand));
        } else if (opcode == "JZS") {
            instructionMemory[address] = new JzsInstruction(std::stoi(operand));
        } else if (opcode == "ADD") {
            instructionMemory[address] = new AddInstruction();
        } else if (opcode == "SUB") {
            instructionMemory[address] = new SubInstruction();
        } else if (opcode == "HLT") {
            instructionMemory[address] = new HltInstruction();
        } else if (opcode == "DEC") {
            instructionMemory[address] = new DecInstruction();
        } }
        catch (const std::invalid_argument& e) {
            std::cerr << "Invalid operand: " << operand << " in line: " << line << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Operand out of range: " << operand << " in line: " << line << std::endl;
        }
    }
}


void ALI::step() {
    if (halted) {
        std::cout << "execution halted.\n";
        return;
    }
    std::cout << ", Memory: " << memory << " PC: " << programCounter << ", Accumulator: " << registers[0]
              << ", Data Register: " << registers[1] << ", Zero Bit: " << zeroBit << std::endl;  // printing memory for debugging purposes

    if (instructionMemory[programCounter] != nullptr) {
        instructionMemory[programCounter]->execute(*this);
    } else {
        std::cerr << "Invalid instruction at address " << programCounter << ".\n";
        haltExecution();
    }
}


//run until halted
void ALI::run() {
    const int MAX_INSTRUCTIONS = 1000; //prevent infinite loops
    int executedInstructions = 0;

    while (!halted && executedInstructions < MAX_INSTRUCTIONS) {
        step();
        executedInstructions++;
    }

    if (executedInstructions == MAX_INSTRUCTIONS) {
        std::cerr << "execution limit reached halting.\n";
        haltExecution();
    }
}


void ALI::haltExecution() {
    halted = true;
}

int ALI::getMemoryValue(int address) const {
    return memory[address];
}

void ALI::setMemoryValue(int address, int value) {
    memory[address] = value;
}

int ALI::getAccumulator() const {
    return registers[0];
}

int ALI::getDataRegister() const {
    return registers[1];
}

void ALI::setAccumulator(int value) {
    registers[0] = value;
}

void ALI::setDataRegister(int value) {
    registers[1] = value;
}


int ALI::getPC() const {
    return programCounter;
}

void ALI::setPC(int address) {
    programCounter = address;
}

void ALI::incrementPC() {
    //check
    programCounter++;
}


bool ALI::isZeroBitSet() const {
    return zeroBit;
}

void ALI::setZeroBit(bool value) {
    zeroBit = value;
}
