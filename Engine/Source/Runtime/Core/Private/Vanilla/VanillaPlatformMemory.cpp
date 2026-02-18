// Copyright (C) 2026 ychgen, all rights reserved.

#include "Vanilla/VanillaPlatformMemory.h"

#include "HAL/PlatformMemory.h"
#include "HAL/MallocAnsi.h"

void FVanillaPlatformMemory::Init()
{
}

NORETURN void FVanillaPlatformMemory::OutOfMemory()
{
}

FMalloc* FVanillaPlatformMemory::CreateBaseAllocatorInstance()
{
	return new FMallocAnsi();
}

const FPlatformMemoryConstants& FVanillaPlatformMemory::GetConstants()
{
	static FPlatformMemoryConstants Constants;
	return Constants;
}

FPlatformMemoryStats FVanillaPlatformMemory::GetStats()
{
	return {};
}
