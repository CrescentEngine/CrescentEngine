// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "CoreTypes.h"
#include "HAL/Malloc.h"

#include <memory.h>

struct FVanillaPlatformMemoryConstants
{
	uint64 TotalPhysical = 0;
	uint64 TotalVirtual  = 0;
};
typedef FVanillaPlatformMemoryConstants FPlatformMemoryConstants;

struct CORE_API FVanillaPlatformMemoryStats
{
	uint64 UsedPhysical     = 0;
	uint64 UsedVirtual      = 0;
	uint64 FreePhysical     = 0;
	uint64 FreeVirtual      = 0;
	uint64 PeakUsedPhysical = 0;
	uint64 PeakUsedVirtual  = 0;
};
struct FPlatformMemoryStats;

struct CORE_API FVanillaPlatformMemory
{
	static void Init();
	static NORETURN void OutOfMemory();

	static FMalloc* CreateBaseAllocatorInstance();
	static const FPlatformMemoryConstants& GetConstants();
	static FPlatformMemoryStats GetStats();

	static FORCEINLINE void* Memcpy(void* RESTRICT InDest, const void* RESTRICT InSrc, uint_t N)
	{
		return ::memcpy(InDest, InSrc, N);
	}
	static FORCEINLINE void* Memmove(void* InDest, const void* InSrc, uint_t N)
	{
		return ::memmove(InDest, InSrc, N);
	}
	static FORCEINLINE void* Memzero(void* InDest, uint_t N)
	{
		return ::memset(InDest, 0, N);
	}
	static FORCEINLINE void* Memset(void* InDest, int32 Value, uint_t N)
	{
		return ::memset(InDest, Value, N);
	}
	static FORCEINLINE int32 Memcmp(const void* LHS, const void* RHS, uint_t N)
	{
		return ::memcmp(LHS, RHS, N);
	}
};
