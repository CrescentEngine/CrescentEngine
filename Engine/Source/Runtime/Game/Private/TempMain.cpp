// Copyright (C) 2026 ychgen, all rights reserved.
// This file is a temporary TU, it will be deleted in favor of module semantics.

#include "Module/ModuleOverrides.h"
OVERRIDES_NEW_DELETE_OPERATORS_ROUTE_TO_CORE_ALLOC

#include "Templates/UniquePtr.h"
#include "HAL/Memory.h"

#include <iostream>

struct FCustomDeleter
{
    FCustomDeleter() = delete;
};

struct FBase {};
struct FChild : FBase {};

int main(int ArgC, char** ArgV)
{
    // Check results to see if EBO works
    std::cout << sizeof(TEfficientPair<FCustomDeleter, FCustomDeleter>) << '\n';
    std::cout << sizeof(TUniquePtr<int>) << ", " << sizeof(TUniquePtr<int[]>) << '\n';

    int* Handle = nullptr;
    {
        TUniquePtr<int> SP = TUniquePtr<int>(new int);
        Handle = SP.Get();

        *SP = 10;
        std::cout << *SP << '\n';
    }

    TUniquePtr<int[]> UP = TUniquePtr<int[]>(new int[4]);
    UP[0] = 50;
    UP[1] = 55;
    UP[2] = 60;
    UP[3] = 65;

    for (int i = 0; i < 4; i++)
    {
        std::cout << UP[i] << '\n';
    }

    std::cin.get();
    return 0;
}
