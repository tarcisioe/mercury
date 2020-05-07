#include "decoder.hpp"

namespace mercury {

namespace size {

static_assert(
    opcode + rs + rt + rd + shamt + funct == instruction,
    "R instruction size incorrect.");
static_assert(
    opcode + rs + rt + immediate == instruction,
    "I instruction size incorrect.");
static_assert(opcode + address == instruction, "J instruction size incorrect.");

}

namespace mask {

static_assert(
    (opcode | rs | rt | rd | shamt | funct) == 0xFFFFFFFFu,
    "R instruction masks don't bitwise-or correctly.");
static_assert(
    (opcode | rs | rt | immediate) == 0xFFFFFFFFu,
    "I instruction masks don't bitwise-or correctly.");
static_assert(
    (opcode | address) == 0xFFFFFFFFu,
    "J instruction masks don't bitwise-or correctly.");

}

namespace {

template <typename T>
constexpr bool operator==(Instruction const& variant, T const& value)
{
    return std::get<T>(variant) == value;
}

// clang-format off
static_assert(
    *decode(0b000000'00001'00010'00011'00000'100000) ==
    RInstruction{1, 2, 3, 0, Funct::ADD}
);
static_assert(
    *decode(0b000100'00001'00010'0000000000101010) ==
    IInstruction{Opcode::BEQ, 1, 2, 42}
);
static_assert(
    *decode(0b000010'00000000000000000000001010) == JInstruction{Opcode::J, 10}
);
// clang-format on
//
}

}
