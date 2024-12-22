#include "ALI.h"
#include <iostream>
#include <fstream>
#include <string>

void loadProgram(ALI& ali, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    
    std::string line;
    int programCounter = 0;
    while (std::getline(file, line) && programCounter < 128) {
        std::cout << "Line " << programCounter << ": " << line << std::endl;
        ali.loadInstruction(programCounter++, line);
        std::cout << "Testing if fileloaded"; // printing for debugging
    }

    file.close();
}

int main() {
    ALI ali;
    std::string filename;
    std::cout << "Enter the filename: ";
    std::cin >> filename;

    loadProgram(ali, filename);

    char command;
    do {
        std::cout << "\nCommand (s = step, a = run all, q = quit): ";
        std::cin >> command;
        if (command == 's') {
            ali.step();
        } else if (command == 'a') {
            ali.run();
        }
    } while (command != 'q');

    std::cout << "Exiting" << std::endl;
    return 0;
}
