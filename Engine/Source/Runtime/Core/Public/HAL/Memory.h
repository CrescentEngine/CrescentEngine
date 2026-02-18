// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "HAL/PlatformMemory.h"
#include "HAL/Malloc.h"

// for malloc() and free()
#include <stdlib.h>

// Main allocator instance (used by FMemory)
extern CORE_API FMalloc* GMalloc;

struct CORE_API FMemory
{
	static void InitAllocator();

	static void* Malloc(size_t N, uint16 Alignment = DEFAULT_ALIGNMENT);
	static void* Realloc(void* Ptr, size_t N, uint16 Alignment = DEFAULT_ALIGNMENT);
	static void  Free(void* Ptr);
	
	static FORCEINLINE void* StandardMalloc(size_t N)
	{
		return ::malloc(N);
	}
	static FORCEINLINE void StandardFree(void* Ptr)
	{
		::free(Ptr);
	}

	static FORCEINLINE void* Memcpy(void* RESTRICT InDest, const void* RESTRICT InSrc, uint_t N)
	{
		return FPlatformMemory::Memcpy(InDest, InSrc, N);
	}
	static FORCEINLINE void* Memmove(void* InDest, const void* InSrc, uint_t N)
	{
		return FPlatformMemory::Memmove(InDest, InSrc, N);
	}
	static FORCEINLINE void* Memzero(void* InDest, uint_t N)
	{
		return FPlatformMemory::Memzero(InDest, N);
	}
	static FORCEINLINE void* Memset(void* InDest, int32 Value, uint_t N)
	{
		return FPlatformMemory::Memset(InDest, Value, N);
	}
	static FORCEINLINE int32 Memcmp(const void* LHS, const void* RHS, uint_t N)
	{
		return FPlatformMemory::Memcmp(LHS, RHS, N);
	}
};

