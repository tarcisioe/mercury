add_executable(main)

target_sources(
    main
        PRIVATE
            bitwise.cpp
            bitwise.hpp
            decoder.cpp
            decoder.hpp
            instruction_formats.cpp
            instruction_formats.hpp
            sized_literals.cpp
            sized_literals.hpp
            main.cpp
)

target_link_libraries(
    main
        PRIVATE
            project_options
)
