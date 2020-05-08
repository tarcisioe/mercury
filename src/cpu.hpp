#ifndef MERCURY_CPU_HPP
#define MERCURY_CPU_HPP

#include <array>
#include <memory>

#include "instruction_formats.hpp"
#include "enum_tools.hpp"


namespace mercury {

struct CPUInternals;

using Register = std::uint32_t;
using Registers = std::array<Register, 32>;

class CPU {
public:
    CPU(RawInstruction const* program);
    ~CPU();

    Registers const& registers() const;
    Register pc() const;
    void execute_instruction();

private:
    void execute(RInstruction);
    void execute(IInstruction);
    void execute(JInstruction);

    RawInstruction const* program_;
    std::unique_ptr<CPUInternals> impl;
};

}


#endif
