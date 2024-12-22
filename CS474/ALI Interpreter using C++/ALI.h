#ifndef ALI_H
#define ALI_H

#include "Instruction.h"
#include <string>
#include <array>

class ALI {
private:
    static const int MEMORY_SIZE = 128; 
    static const int REGISTERS_SIZE = 2; //register a and b
    int memory[MEMORY_SIZE];   
    int registers[REGISTERS_SIZE];  //two registers
    int programCounter;
    bool zeroBit;
    bool halted;
    Instruction* instructionMemory[MEMORY_SIZE];

public:
    ALI();
    ~ALI();
    void loadInstruction(int address, const std::string& line);
    void step();
    void run();
    void haltExecution();
    int getMemoryValue(int address) const;
    void setMemoryValue(int address, int value);

    //registers accessors
    int getAccumulator() const;
    int getDataRegister() const;
    void setAccumulator(int value);
    void setDataRegister(int value);

    //program Counter
    int getPC() const;
    void setPC(int address);
    void incrementPC();

    //zero flag
    bool isZeroBitSet() const;
    void setZeroBit(bool value);

};

#endif
