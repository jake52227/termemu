#include "emulator.hpp"

int main(void)
{
    Emulator emulator(800, 800, "hello world");
    emulator.start();
    return 0;
}
