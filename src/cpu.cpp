#include "cpu.hpp"

#include <iostream>

#include "decoder.hpp"
#include "enum_indexed_array.hpp"

namespace mercury {

using Register = std::uint32_t;

using RInstructionHandler = void (CPUInternals::*)(RInstruction);
using IInstructionHandler = void (CPUInternals::*)(IInstruction);
using JInstructionHandler = void (CPUInternals::*)(JInstruction);

using RInstructionHandlers =
    EnumIndexedArray<Funct, RInstructionHandler, Funct::SLTU>;
using IInstructionHandlers =
    EnumIndexedArray<Opcode, IInstructionHandler, Opcode::SC>;
using JInstructionHandlers =
    EnumIndexedArray<Opcode, JInstructionHandler, Opcode::JAL>;

struct CPUInternals {
    void unknown_r_instruction(RInstruction)
    {
        std::cout << "unknown_r_instruction\n";
    }

    void unknown_i_instruction(IInstruction)
    {
        std::cout << "unknown_i_instruction\n";
    }

    void unknown_j_instruction(JInstruction)
    {
        std::cout << "unknown_j_instruction\n";
    }

    void add(RInstruction instruction)
    {
        std::cout << "add" << static_cast<unsigned>(instruction.rt) << ' '
                  << static_cast<unsigned>(instruction.rs) << ' '
                  << static_cast<unsigned>(instruction.rd) << '\n';
    }

    void sll(RInstruction instruction)
    {
        std::cout << "sll " << static_cast<unsigned>(instruction.rt) << ' '
                  << static_cast<unsigned>(instruction.rd) << ' '
                  << static_cast<unsigned>(instruction.shamt) << '\n';
    }

    void beq(IInstruction instruction)
    {
        std::cout << "beq " << static_cast<unsigned>(instruction.rt) << ' '
                  << static_cast<unsigned>(instruction.rs) << ' '
                  << instruction.immediate << "\n";
    }

    void jump(JInstruction instruction)
    {
        std::cout << "jump " << instruction.address << "\n";
    }

    void jal(JInstruction instruction)
    {
        std::cout << "jal " << instruction.address << "\n";
    }

    std::array<Register, 32> register_bank;
};

constexpr static RInstructionHandlers make_r_handlers()
{
    auto handlers = RInstructionHandlers{&CPUInternals::unknown_r_instruction};

    handlers[Funct::ADD] = &CPUInternals::add;
    handlers[Funct::SLL] = &CPUInternals::sll;

    return handlers;
}

constexpr static IInstructionHandlers make_i_handlers()
{
    auto handlers = IInstructionHandlers{&CPUInternals::unknown_i_instruction};

    handlers[Opcode::BEQ] = &CPUInternals::beq;

    return handlers;
}

constexpr static JInstructionHandlers make_j_handlers()
{
    auto handlers = JInstructionHandlers{&CPUInternals::unknown_j_instruction};

    handlers[Opcode::J] = &CPUInternals::jump;
    handlers[Opcode::JAL] = &CPUInternals::jal;

    return handlers;
}

constexpr auto r_handlers = make_r_handlers();
constexpr auto i_handlers = make_i_handlers();
constexpr auto j_handlers = make_j_handlers();

void CPU::execute(RInstruction instruction)
{
    auto&& handler = r_handlers[instruction.funct];

    (*impl.*handler)(instruction);
}

void CPU::execute(IInstruction instruction)
{
    auto&& handler = i_handlers[instruction.opcode];

    (*impl.*handler)(instruction);
}

void CPU::execute(JInstruction instruction)
{
    auto&& handler = j_handlers[instruction.opcode];

    (*impl.*handler)(instruction);
}

CPU::CPU(): impl{std::make_unique<CPUInternals>()} {}

CPU::~CPU() = default;

template <class... Ts> struct overload: Ts... {
    using Ts::operator()...;
};

template <class... Ts> overload(Ts...) -> overload<Ts...>;

void CPU::execute(RawInstruction raw_instruction)
{
    auto const decoded = decode(raw_instruction);

    if (not decoded) {
        std::cout << "Unknown instruction.\n";
        return;
    }

    std::visit([&](auto instruction) { execute(instruction); }, *decoded);
}

}
