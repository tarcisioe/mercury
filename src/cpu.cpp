#include "cpu.hpp"

#include <iostream>

#include "decoder.hpp"


namespace mercury {

namespace {

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

}

void CPU::execute(RawInstruction raw)
{
    auto const instruction = decode(raw);

    if (not instruction) {
        std::cout << "Unknown instruction.\n";
        return;
    }

    std::visit(
        overloaded{
            [&](RInstruction) { std::cout << "R\n"; },
            [&](IInstruction) { std::cout << "I\n"; },
            [&](JInstruction) { std::cout << "J\n"; },
        },
        *instruction);
}

}
