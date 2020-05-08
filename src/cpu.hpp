#ifndef MERCURY_CPU_HPP
#define MERCURY_CPU_HPP

#include <array>
#include <memory>

#include "instruction_formats.hpp"
#include "enum_tools.hpp"


namespace mercury {

struct CPUInternals;

class CPU {
public:
    void execute(RawInstruction instruction);

    CPU();
    ~CPU();

private:
    void execute(RInstruction);
    void execute(IInstruction);
    void execute(JInstruction);

    std::unique_ptr<CPUInternals> impl;
};

}


#endif
