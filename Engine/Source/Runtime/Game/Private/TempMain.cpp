// Copyright (C) 2026 ychgen, all rights reserved.
// This file is a temporary TU, it will be deleted in favor of module semantics.

#include "HAL/Memory.h"
#include <iostream>

int main(int ArgC, char** ArgV)
{
    int* Array = (int*) FMemory::Malloc(4 * sizeof(int));
    Array[0] = 10;
    Array[1] = 20;
    Array[2] = 30;
    Array[3] = 40;

    for (int i = 0; i < 4; i++)
    {
        std::cout << Array[i] << '\n';
    }
    FMemory::Free(Array);
    for (int i = 0; i < 4; i++)
    {
        std::cout << Array[i] << '\n';
    }

    std::cin.get();
    return 0;
}
