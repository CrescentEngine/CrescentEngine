// Copyright (C) 2026 ychgen, all rights reserved.

#include "HAL/MallocAnsi.h"

#include "HAL/PlatformMemory.h"
#include "HAL/Memory.h"

#include "Templates/Memory/Alignment.h"

// for malloc(), realloc() and free()
#include <stdlib.h>

void* FMallocAnsi::Malloc(uint_t N, uint16 Alignment)
{
	uint16 AlignBasedOnN = N > 16 ? 16 : 8;
	Alignment = AlignBasedOnN > Alignment ? AlignBasedOnN : Alignment;

	// We allocate and store:
	//   Data[N], then:
	//   Ptr[PtrSize], then:
	//   N[4/8]

	void* Ptr = malloc(N + Alignment + sizeof(void*) + sizeof(uint_t));
	if (Ptr == nullptr)
	{
		FPlatformMemory::OutOfMemory();
	}
	void* Result = Align((uint8*) Ptr + sizeof(void*) + sizeof(uint_t), (uint64) Alignment);
	*((void**)((uint8*) Result - sizeof(void*))) = Ptr;
	*((uint_t*)((uint8*) Result - sizeof(void*) - sizeof(uint_t))) = N;

	if (Result == nullptr)
	{
		FPlatformMemory::OutOfMemory();
	}

	return Result;
}

void* FMallocAnsi::Realloc(void* Ptr, uint_t N, uint16 Alignment)
{
	uint16 AlignBasedOnN = N > 16 ? 16 : 8;
	Alignment = AlignBasedOnN > Alignment ? AlignBasedOnN : Alignment;
	void* Result = nullptr;

	if (Ptr && N)
	{
		Result = Malloc(N, Alignment);
		uint_t AllocatedBytes;
		GetAllocationSize(Result, AllocatedBytes);
		FMemory::Memcpy(Result, Ptr, N < AllocatedBytes ? N : AllocatedBytes);
		Free(Ptr);
	}
	else if (!Ptr)
	{
		Result = Malloc(N, Alignment);
	}
	else
	{
		free(*((void**)((uint8*) Ptr - sizeof(void*))));
		Result = nullptr;
	}

	if (Result == nullptr && !N)
	{
		FPlatformMemory::OutOfMemory();
	}

	return Result;
}

void FMallocAnsi::Free(void* Ptr)
{
	if (Ptr)
	{
		free(*((void**)((uint8*) Ptr - sizeof(void*))));
	}
}

bool FMallocAnsi::GetAllocationSize(const void* Ptr, uint_t& OutSize)
{
	OutSize = *((uint_t*)((uint8*) Ptr - sizeof(void*) - sizeof(uint_t)));
	return true;
}
