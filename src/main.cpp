#include "cpu.hpp"

int main()
{
    auto cpu = mercury::CPU{};

    cpu.execute(0x00000000);
    cpu.execute(0x08000000);
    cpu.execute(0x10000000);
    cpu.execute(0x000000FF);
    cpu.execute(0xFF0000FF);
}
