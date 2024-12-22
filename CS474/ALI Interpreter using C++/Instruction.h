#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>

class ALI;

class Instruction {
protected:
    std::string opcode;

public:
    explicit Instruction(const std::string& op) : opcode(op) {}
    virtual ~Instruction() = default;
    virtual void execute(ALI& ali) = 0;
    std::string getOpcode() const {
        return opcode;
    }
};

//all the instruction inherit from the Instruction class which is the parent class
class LdaInstruction : public Instruction {
private:
    int address;

public:
    explicit LdaInstruction(int addr);
    void execute(ALI& ali) override;
};

class LdiInstruction : public Instruction {
private:
    int value; 

public:
    explicit LdiInstruction(int val);
    void execute(ALI& ali) override;
};

class StrInstruction : public Instruction {
private:
    int address;

public:
    explicit StrInstruction(int addr);
    void execute(ALI& ali) override;
};

class XchInstruction : public Instruction {
public:
    XchInstruction();
    void execute(ALI& ali) override;
};

class JmpInstruction : public Instruction {
private:
    int address;

public:
    explicit JmpInstruction(int addr);
    void execute(ALI& ali) override;
};

class JzsInstruction : public Instruction {
private:
    int address;

public:
    explicit JzsInstruction(int addr);
    void execute(ALI& ali) override;
};

class AddInstruction : public Instruction {
public:
    AddInstruction();
    void execute(ALI& ali) override;
};

class SubInstruction : public Instruction {
public:
    SubInstruction();
    void execute(ALI& ali) override;
};

class HltInstruction : public Instruction {
public:
    HltInstruction();
    void execute(ALI& ali) override;
};

class DecInstruction : public Instruction {
public:
    DecInstruction();
    void execute(ALI& ali) override;
};

#endif
