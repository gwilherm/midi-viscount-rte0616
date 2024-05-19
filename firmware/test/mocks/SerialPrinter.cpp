#include "SerialPrinter.h"

#include <iostream>

size_t SerialPrinter::print(int v)
{
    std::cout << v;
    return 0;
}

size_t SerialPrinter::print(const char* v)
{
    std::cout << v;
    return 0;
}

size_t SerialPrinter::println(void)
{
    std::cout << std::endl;
    return 0;
}