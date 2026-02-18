// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "Vanilla/VanillaPlatformMemory.h"

struct FPlatformMemoryStats : public FVanillaPlatformMemoryStats
{
};

struct CORE_API FUnixPlatformMemory : public FVanillaPlatformMemory
{
};
typedef FUnixPlatformMemory FPlatformMemory;
