// Copyright (C) 2026 ychgen, all rights reserved.
// This file is a temporary TU, it will be deleted in favor of module semantics.

#include "Module/ModuleOverrides.h"
OVERRIDES_NEW_DELETE_OPERATORS_ROUTE_TO_CORE_ALLOC

#include "Allocators/DefaultAllocator.h"
#include "Templates/SharedPtr.h"
#include "Templates/UniquePtr.h"
#include "Templates/Atomic.h"
#include "HAL/Memory.h"

#include <iostream>
#include <memory>

int main(int ArgC, char** ArgV)
{
    TSharedPtr<int[]> csex = TSharedPtr<int[]>(new int[5]);
    int* Ptr = nullptr;
    {
        TSharedPtr<int, false> SP1(new int(6), TDefaultDelete<int>{});
        Ptr = SP1.Get();
        {
            TSharedPtr<int, false> SP2 = SP1;
            {
                TSharedPtr<int, false> SP3 = SP1;
                bool a1 = SP2 == SP3;
                bool a = SP2 < SP3;
            }
        }
    }

    TSharedPtr<int, false > P1;
    TSharedPtr<float, false> P2;
    bool c = P1 < P2;
    bool m = P1 < nullptr;
    bool mc = nullptr < P1;

    P1.Reset(new int, TDefaultDelete<int>{});

    auto a = CreateShared<int>(5);
    auto b = CreateShared<int[]>(5);
    auto cc = CreateShared<int[5]>();

    // holy random code bruh

    std::cin.get();
    return 0;
}
