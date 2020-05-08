#include <iostream>
#include <vector>

#include "cpu.hpp"

int main()
{
    auto const instructions = std::vector<mercury::RawInstruction>{
        0b001010'01000'01001'0000000000001010,  // slti $t1 $t0 10
        0b000100'00000'01001'0000000000000001,  // beq $t1 $zero
        0b001000'01000'01000'0000000000000001,  // addi $t0 1
        // 0b000010'00000000000000000000000000,    // j 0
        0b000000'00000'00000'00000'00000'001000,    // jr $zero
    };

    auto cpu = mercury::CPU{instructions.data()};

    while (cpu.pc()/4 < instructions.size()) {
        cpu.execute_instruction();
    }

    {
        auto count = 0;
        for (auto& r: cpu.registers()) {
            std::cout << "R" << count << ": " << r << '\n';
            ++count;
        }
    }
}
