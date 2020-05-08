add_executable(mercury)

target_sources(
    mercury
        PRIVATE
            bitwise.cpp
            bitwise.hpp
            cpu.cpp
            cpu.hpp
            decoder.cpp
            decoder.hpp
            enum_indexed_array.hpp
            enum_indexed_array.cpp
            enum_tools.hpp
            enum_tools.cpp
            instruction_formats.cpp
            instruction_formats.hpp
            sized_literals.cpp
            sized_literals.hpp
            main.cpp
)

target_link_libraries(
    mercury
        PRIVATE
            project_options
)
