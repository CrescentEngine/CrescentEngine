// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "Core.h"

enum : uint16
{
	DEFAULT_ALIGNMENT = 0
};

/**
 * @brief Classes that inherit from this class will use STL/system provided allocation policies
 * when using language native new and delete operators of all kinds.
 * This is usually used by internal engine classes related to memory in HAL.
 */
class CORE_API FIgnoreEngineAllocPolicy
{
public:
	void* operator new(size_t N);
	void  operator delete(void* Ptr);
	void* operator new[](size_t N);
	void  operator delete[](void* Ptr);
};

class CORE_API FMalloc : public FIgnoreEngineAllocPolicy
{
public:
	virtual void* Malloc(uint_t N, uint16 Alignment = DEFAULT_ALIGNMENT) = 0;
	virtual void* Realloc(void* Ptr, uint_t N, uint16 Alignment = DEFAULT_ALIGNMENT) = 0;
	virtual void  Free(void* Ptr) = 0;

	/**
	 * @brief If supported, returns the number of actually allocated bytes
	 * for given Ptr. If given Ptr wasn't allocated by the allocator, the behavior is undefined.
	 * 
	 * @param Pointer to get the allocation size information of.
	 * @param OutSize No. bytes allocated related to given Ptr.
	 * @return Whether or not this allocator supports acquiring allocation sizes.
	 */
	virtual bool GetAllocationSize(const void* Ptr, uint_t& OutSize)
	{
		return false;
	}
};
