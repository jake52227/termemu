#include "emulator.hpp"

int main(void)
{
    std::unique_ptr<Emulator> emulator = Emulator::make(800, 800, "hello world");
    emulator->start();
    return 0;
}
