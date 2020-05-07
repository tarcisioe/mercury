#ifndef MERCURY_BITWISE_HPP
#define MERCURY_BITWISE_HPP

#include <stdexcept>

#include "sized_literals.hpp"

namespace mercury::bitwise {

constexpr std::uint32_t base_and_mask(int size)
{
    using namespace sized_literals;

    if (size > 32 or size < 1) {
        throw std::out_of_range(
            "Mask cannot be larger than 32 or smaller than 1 bit.");
    }

    return static_cast<std::uint32_t>((1_u64 << size) - 1);
}

constexpr std::uint32_t and_mask(int size, int position)
{
    if (size > 32 or size < 1) {
        throw std::out_of_range(
            "Mask cannot be larger than 32 or smaller than 1 bit.");
    }

    if (position > 31 or position < 0) {
        throw std::out_of_range(
            "Mask position cannot be larger than 31 or smaller than 0.");
    }

    return base_and_mask(size) << (position);
}

}

#endif
