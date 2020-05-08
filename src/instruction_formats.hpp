#ifndef MERCURY_INSTRUCTION_FORMATS_HPP
#define MERCURY_INSTRUCTION_FORMATS_HPP

#include <cstdint>
#include <variant>

namespace mercury {

using RawInstruction = std::uint32_t;

enum class Opcode: std::uint8_t {
    RInst = 0x00,
    ANDI = 0x0c,
    BEQ = 0x04,
    BNE = 0x05,
    J = 0x02,
    JAL = 0x03,
    LBU = 0x24,
    LHU = 0x25,
    LL = 0x30,
    LUI = 0x0f,
    LW = 0x23,
    ORI = 0x0d,
    SLTI = 0x0a,
    SLTIU = 0x0b,
    SB = 0x28,
    SC = 0x38,
    SH = 0x29,
    SW = 0x2b,
};

enum class Funct: std::uint8_t {
    ADD = 0x20,
    ADDU = 0x21,
    AND = 0x24,
    JR = 0x08,
    NOR = 0x27,
    OR = 0x25,
    SLT = 0x2a,
    SLTU = 0x2b,
    SLL = 0x00,
    SRL = 0x02,
    SUB = 0x22,
    SUBU = 0x23,
};

struct RInstruction {
    // Opcode can only be 0 so it can be omitted.
    std::uint8_t rs;
    std::uint8_t rt;
    std::uint8_t rd;
    std::uint8_t shamt;
    Funct funct;
};

constexpr bool operator==(RInstruction lhs, RInstruction rhs)
{
    return (lhs.rs == rhs.rs) and (lhs.rt == rhs.rt) and (lhs.rd == rhs.rd) and
           (lhs.shamt == rhs.shamt) and (lhs.funct == rhs.funct);
}

struct IInstruction {
    // We must validate the opcode.
    Opcode opcode;
    std::uint8_t rs;
    std::uint8_t rt;
    std::uint16_t immediate;
};

constexpr bool operator==(IInstruction lhs, IInstruction rhs)
{
    return (lhs.opcode == rhs.opcode) and (lhs.rs == rhs.rs) and
           (lhs.rt == rhs.rt) and (lhs.immediate == rhs.immediate);
}

struct JInstruction {
    // Opcode must be validated before construction.
    Opcode opcode;
    std::uint32_t address;
};

constexpr bool operator==(JInstruction lhs, JInstruction rhs)
{
    return (lhs.opcode == rhs.opcode) and (lhs.address == rhs.address);
}

using Instruction = std::variant<RInstruction, IInstruction, JInstruction>;

}

#endif
