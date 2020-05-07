#ifndef MERCURY_SIZED_LITERALS_HPP
#define MERCURY_SIZED_LITERALS_HPP

#include <cstdint>

namespace mercury::sized_literals {

constexpr std::uint64_t operator""_u64(unsigned long long v)
{
    return static_cast<std::uint64_t>(v);
}

}

#endif
