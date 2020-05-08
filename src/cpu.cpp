#include "cpu.hpp"

#include <iostream>

#include "bitwise.hpp"
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

constexpr auto as_signed(std::uint32_t u)
{
    return static_cast<std::int32_t>(u);
}

constexpr auto as_unsigned(std::int32_t s)
{
    return static_cast<std::uint32_t>(s);
}

constexpr auto as_signed(std::uint16_t u)
{
    return static_cast<std::int16_t>(u);
}

constexpr auto as_unsigned(std::int16_t s)
{
    return static_cast<std::uint16_t>(s);
}

constexpr auto zero_extend(std::uint16_t u)
{
    return static_cast<std::uint32_t>(u);
}

constexpr auto sign_extend(std::int16_t u)
{
    return static_cast<std::int32_t>(u);
}

constexpr auto sign_extend(std::uint16_t u)
{
    return sign_extend(as_signed(u));
}

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

    /* Basic R instructions */

    void add(RInstruction instruction)
    {
        auto rs = as_signed(register_bank[instruction.rs]);
        auto rt = as_signed(register_bank[instruction.rt]);

        register_bank[instruction.rd] = as_unsigned(rs + rt);
    }

    void addu(RInstruction instruction)
    {
        auto rs = register_bank[instruction.rs];
        auto rt = register_bank[instruction.rt];

        register_bank[instruction.rd] = rs + rt;
    }

    void bitwise_and(RInstruction instruction)
    {
        auto rs = register_bank[instruction.rs];
        auto rt = register_bank[instruction.rt];

        register_bank[instruction.rd] = rs & rt;
    }

    void jr(RInstruction instruction)
    {
        std::cout << "jr " << register_bank[instruction.rs] << "\n";
    }

    void nor(RInstruction instruction)
    {
        auto rs = register_bank[instruction.rs];
        auto rt = register_bank[instruction.rt];

        register_bank[instruction.rd] = ~(rs | rt);
    }

    void bitwise_or(RInstruction instruction)
    {
        auto rs = register_bank[instruction.rs];
        auto rt = register_bank[instruction.rt];

        register_bank[instruction.rd] = rs | rt;
    }

    void slt(RInstruction instruction)
    {
        auto rs = as_signed(register_bank[instruction.rs]);
        auto rt = as_signed(register_bank[instruction.rt]);

        register_bank[instruction.rd] = rs < rt;
    }

    void sltu(RInstruction instruction)
    {
        auto rs = register_bank[instruction.rs];
        auto rt = register_bank[instruction.rt];

        register_bank[instruction.rd] = rs < rt;
    }

    void sll(RInstruction instruction)
    {
        auto rt = as_signed(register_bank[instruction.rt]);

        register_bank[instruction.rd] = as_unsigned(rt << instruction.shamt);
    }

    void srl(RInstruction instruction)
    {
        auto rt = as_signed(register_bank[instruction.rt]);

        register_bank[instruction.rd] = as_unsigned(rt << instruction.shamt);
    }

    void sub(RInstruction instruction)
    {
        auto rs = as_signed(register_bank[instruction.rs]);
        auto rt = as_signed(register_bank[instruction.rt]);

        register_bank[instruction.rd] = as_unsigned(rs - rt);
    }

    void subu(RInstruction instruction)
    {
        auto rs = register_bank[instruction.rs];
        auto rt = register_bank[instruction.rt];

        register_bank[instruction.rd] = rs - rt;
    }

    /* Multiplication R instructions */

    void mfhi(RInstruction instruction)
    {
        register_bank[instruction.rd] = hi;
    }

    void mflo(RInstruction instruction)
    {
        register_bank[instruction.rd] = lo;
    }

    void mult(RInstruction instruction)
    {
        auto rs =
            static_cast<int64_t>(as_signed(register_bank[instruction.rs]));
        auto rt =
            static_cast<int64_t>(as_signed(register_bank[instruction.rt]));

        auto result = static_cast<uint64_t>(rs * rt);
        lo = static_cast<Register>((result & 0xFFFFFFFF00000000) >> 32);
        hi = static_cast<Register>(result & 0x00000000FFFFFFFF);
    }

    void multu(RInstruction instruction)
    {
        auto rs = static_cast<uint64_t>(register_bank[instruction.rs]);
        auto rt = static_cast<uint64_t>(register_bank[instruction.rt]);

        auto result = rs * rt;
        lo = static_cast<Register>((result & 0xFFFFFFFF00000000) >> 32);
        hi = static_cast<Register>(result & 0x00000000FFFFFFFF);
    }

    void div(RInstruction instruction)
    {
        auto rs = as_signed(register_bank[instruction.rs]);
        auto rt = as_signed(register_bank[instruction.rt]);

        lo = as_unsigned(rs / rt);
        hi = as_unsigned(rs % rt);
    }

    void divu(RInstruction instruction)
    {
        auto rs = register_bank[instruction.rs];
        auto rt = register_bank[instruction.rt];

        lo = rs / rt;
        hi = rs % rt;
    }

    /* I instructions */

    void addi(IInstruction instruction)
    {
        auto rs = as_signed(register_bank[instruction.rs]);

        register_bank[instruction.rt] =
            as_unsigned(rs + sign_extend(instruction.immediate));
    }

    void addiu(IInstruction instruction)
    {
        auto rs = register_bank[instruction.rs];

        register_bank[instruction.rt] =
            rs + as_unsigned(sign_extend(instruction.immediate));
    }

    void andi(IInstruction instruction)
    {
        auto rs = register_bank[instruction.rs];

        register_bank[instruction.rt] = rs & zero_extend(instruction.immediate);
    }

    void beq(IInstruction instruction)
    {
        std::cout << "beq " << static_cast<unsigned>(instruction.rt) << ' '
                  << static_cast<unsigned>(instruction.rs) << ' '
                  << instruction.immediate << "\n";
    }

    void bne(IInstruction instruction)
    {
        std::cout << "bne " << static_cast<unsigned>(instruction.rt) << ' '
                  << static_cast<unsigned>(instruction.rs) << ' '
                  << instruction.immediate << "\n";
    }

    void ori(IInstruction instruction)
    {
        auto rs = register_bank[instruction.rs];

        register_bank[instruction.rt] = rs | zero_extend(instruction.immediate);
    }

    void slti(IInstruction instruction)
    {
        auto rs = register_bank[instruction.rs];

        register_bank[instruction.rt] =
            as_signed(rs) < sign_extend(instruction.immediate);
    }

    void sltiu(IInstruction instruction)
    {
        auto rs = register_bank[instruction.rs];

        register_bank[instruction.rt] =
            rs < as_unsigned(sign_extend(instruction.immediate));
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
    Register hi, lo;
};

constexpr static RInstructionHandlers make_r_handlers()
{
    auto handlers = RInstructionHandlers{&CPUInternals::unknown_r_instruction};

    handlers[Funct::ADD] = &CPUInternals::add;
    handlers[Funct::ADDU] = &CPUInternals::addu;
    handlers[Funct::AND] = &CPUInternals::bitwise_and;
    handlers[Funct::DIV] = &CPUInternals::div;
    handlers[Funct::DIVU] = &CPUInternals::divu;
    handlers[Funct::JR] = &CPUInternals::jr;
    handlers[Funct::MFHI] = &CPUInternals::mfhi;
    handlers[Funct::MFLO] = &CPUInternals::mflo;
    handlers[Funct::MULT] = &CPUInternals::mult;
    handlers[Funct::MULTU] = &CPUInternals::multu;
    handlers[Funct::NOR] = &CPUInternals::nor;
    handlers[Funct::OR] = &CPUInternals::bitwise_or;
    handlers[Funct::SLT] = &CPUInternals::slt;
    handlers[Funct::SLTU] = &CPUInternals::sltu;
    handlers[Funct::SLL] = &CPUInternals::sll;
    handlers[Funct::SRL] = &CPUInternals::sll;
    handlers[Funct::SUB] = &CPUInternals::sub;
    handlers[Funct::SUBU] = &CPUInternals::subu;

    return handlers;
}

constexpr static IInstructionHandlers make_i_handlers()
{
    auto handlers = IInstructionHandlers{&CPUInternals::unknown_i_instruction};

    handlers[Opcode::ADDI] = &CPUInternals::addi;
    handlers[Opcode::ADDIU] = &CPUInternals::addiu;
    handlers[Opcode::ANDI] = &CPUInternals::andi;
    handlers[Opcode::BEQ] = &CPUInternals::beq;
    handlers[Opcode::BNE] = &CPUInternals::bne;
    handlers[Opcode::ORI] = &CPUInternals::ori;
    handlers[Opcode::SLTI] = &CPUInternals::slti;
    handlers[Opcode::SLTIU] = &CPUInternals::sltiu;

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
