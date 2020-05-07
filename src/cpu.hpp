#ifndef MERCURY_CPU_HPP
#define MERCURY_CPU_HPP

#include <array>

#include "instruction_formats.hpp"


namespace mercury {

using Register = std::uint32_t;

class CPU {
public:
    void execute(RawInstruction r);

private:

    // std::array<Register, 32> register_bank;
};

}


#endif
