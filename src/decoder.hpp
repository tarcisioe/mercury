#ifndef MERCURY_DECODER_HPP
#define MERCURY_DECODER_HPP

#include <cstdint>
#include <optional>
#include <stdexcept>
#include <iostream>

#include "bitwise.hpp"
#include "enum_tools.hpp"
#include "instruction_formats.hpp"

namespace mercury {

namespace size {

constexpr auto instruction = 32;
constexpr auto opcode = 6;
constexpr auto rs = 5;
constexpr auto rd = 5;
constexpr auto rt = 5;
constexpr auto shamt = 5;
constexpr auto funct = 6;
constexpr auto immediate = 16;
constexpr auto address = 26;

}

namespace position {

constexpr auto opcode = size::instruction - size::opcode;
constexpr auto rs = opcode - size::rs;
constexpr auto rt = rs - size::rt;
constexpr auto rd = rt - size::rd;
constexpr auto shamt = rd - size::shamt;
constexpr auto funct = shamt - size::funct;
constexpr auto immediate = rt - size::immediate;
constexpr auto address = opcode - size::address;

}

namespace mask {

constexpr auto opcode = bitwise::and_mask(size::opcode, position::opcode);
constexpr auto rs = bitwise::and_mask(size::rs, position::rs);
constexpr auto rt = bitwise::and_mask(size::rt, position::rt);
constexpr auto rd = bitwise::and_mask(size::rd, position::rd);
constexpr auto shamt = bitwise::and_mask(size::shamt, position::shamt);
constexpr auto funct = bitwise::and_mask(size::funct, position::funct);
constexpr auto immediate = bitwise::and_mask(size::immediate, position::immediate);
constexpr auto address = bitwise::and_mask(size::address, position::address);

}

struct FieldInfo {
    std::uint32_t mask;
    int position;
};

namespace info {

constexpr auto opcode = FieldInfo{mask::opcode, position::opcode};
constexpr auto rs = FieldInfo{mask::rs, position::rs};
constexpr auto rt = FieldInfo{mask::rt, position::rt};
constexpr auto rd = FieldInfo{mask::rd, position::rd};
constexpr auto shamt = FieldInfo{mask::shamt, position::shamt};
constexpr auto funct = FieldInfo{mask::funct, position::funct};
constexpr auto immediate = FieldInfo{mask::immediate, position::immediate};
constexpr auto address = FieldInfo{mask::address, position::address};

}

template <typename T = std::uint32_t>
constexpr auto get_field(RawInstruction raw, FieldInfo info)
{
    return static_cast<T>((raw & info.mask) >> info.position);
}

constexpr std::optional<RInstruction> r_instruction(RawInstruction raw)
{
    auto funct = get_field<Funct>(raw, info::funct);

    switch (funct) {
        case Funct::ADD:
        case Funct::ADDU:
        case Funct::AND:
        case Funct::JR:
        case Funct::NOR:
        case Funct::OR:
        case Funct::SLT:
        case Funct::SLTU:
        case Funct::SLL:
        case Funct::SRL:
        case Funct::SUB:
        case Funct::SUBU: {
            return RInstruction{
                get_field<std::uint8_t>(raw, info::rs),
                get_field<std::uint8_t>(raw, info::rt),
                get_field<std::uint8_t>(raw, info::rd),
                get_field<std::uint8_t>(raw, info::shamt),
                get_field<Funct>(raw, info::funct),
            };
        }
    }

    return std::nullopt;
}

constexpr auto j_instruction(Opcode opcode, RawInstruction raw)
{
    return JInstruction{
        opcode,
        get_field(raw, info::address),
    };
}

constexpr auto i_instruction(Opcode opcode, RawInstruction raw)
{
    return IInstruction{
        opcode,
        get_field<std::uint8_t>(raw, info::rs),
        get_field<std::uint8_t>(raw, info::rt),
        get_field<std::uint16_t>(raw, info::immediate),
    };
}

constexpr std::optional<Instruction> decode(RawInstruction raw)
{
    auto opcode = get_field<Opcode>(raw, info::opcode);

    switch (opcode) {
        case Opcode::RInst: {
            auto decoded = r_instruction(raw);
            if (not decoded) {
                return std::nullopt;
            }

            return *decoded;
        }
        case Opcode::J:
        case Opcode::JAL: {
            return j_instruction(opcode, raw);
        }
        case Opcode::BEQ:
        case Opcode::BNE:
        case Opcode::SLTI:
        case Opcode::SLTIU:
        case Opcode::ANDI:
        case Opcode::ORI:
        case Opcode::LUI:
        case Opcode::LW:
        case Opcode::LBU:
        case Opcode::LHU:
        case Opcode::SB:
        case Opcode::SH:
        case Opcode::SW:
        case Opcode::LL:
        case Opcode::SC: {
            return i_instruction(opcode, raw);
        }
    }

    return std::nullopt;
}

}

#endif
