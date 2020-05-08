#ifndef MERCURY_ENUM_TOOLS
#define MERCURY_ENUM_TOOLS

#include <type_traits>


namespace mercury {

template <typename Enum>
constexpr std::underlying_type_t<Enum> value_of(Enum value)
{
    return static_cast<std::underlying_type_t<Enum>>(value);
}

}


#endif
