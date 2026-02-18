// Copyright (C) 2026 ychgen, all rights reserved.

#include "HAL/Memory.h"

#include "HAL/PlatformMemory.h"

CORE_API FMalloc* GMalloc = nullptr;

void FMemory::InitAllocator()
{
	if (!GMalloc)
	{
		GMalloc = FPlatformMemory::CreateBaseAllocatorInstance();
	}
}

void* FMemory::Malloc(size_t N, uint16 Alignment)
{
	if (!GMalloc)
	{
		InitAllocator();
	}
	return GMalloc->Malloc(N, Alignment);
}

void* FMemory::Realloc(void* Ptr, size_t N, uint16 Alignment)
{
	if (!GMalloc)
	{
		InitAllocator();
	}
	return GMalloc->Realloc(Ptr, N, Alignment);
}

void FMemory::Free(void* Ptr)
{
	if (!GMalloc)
	{
		InitAllocator();
	}
	GMalloc->Free(Ptr);
}

void* FIgnoreEngineAllocPolicy::operator new(size_t N)
{
	return FMemory::StandardMalloc(N);
}

void  FIgnoreEngineAllocPolicy::operator delete(void* Ptr)
{
	FMemory::StandardFree(Ptr);
}

void* FIgnoreEngineAllocPolicy::operator new[](size_t N)
{
	return FMemory::StandardMalloc(N);
}

void  FIgnoreEngineAllocPolicy::operator delete[](void* Ptr)
{
	FMemory::StandardFree(Ptr);
}
