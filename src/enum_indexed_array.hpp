#ifndef MERCURY_ENUM_INDEXED_ARRAY
#define MERCURY_ENUM_INDEXED_ARRAY

#include <array>

#include "enum_tools.hpp"

template <typename Enum, typename Type, Enum Max> class EnumIndexedArray {
public:
    constexpr EnumIndexedArray(Type default_value = Type{}): contents{}
    {
        for (auto& c: contents) {
            c = default_value;
        }
    }

    constexpr Type& operator[](Enum index)
    {
        return contents[value_of(index)];
    }

    constexpr Type const& operator[](Enum index) const
    {
        return contents[value_of(index)];
    }

private:
    using Contents = std::array<Type, value_of(Max) + 1>;

    Contents contents;
};

#endif
