// Copyright (C) 2026 ychgen, all rights reserved.
// This file is a temporary TU, it will be deleted in favor of module semantics.

#include "Module/ModuleOverrides.h"
OVERRIDES_NEW_DELETE_OPERATORS_ROUTE_TO_CORE_ALLOC

#include "Templates/UniquePtr.h"
#include "Templates/Atomic.h"
#include "HAL/Memory.h"

#include <iostream>

int main(int ArgC, char** ArgV)
{
    TAtomic<int32> Atomic;
    Atomic.Store(10);

    int32 AV = (int32)Atomic;
    Atomic++;
    int32 AV2 = Atomic.Load();

    std::cin.get();
    return 0;
}
