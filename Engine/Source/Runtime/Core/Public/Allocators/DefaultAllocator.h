// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "CoreTypes.h"
#include "HAL/Memory.h"

/**
 * @brief Default heap allocator that uses new and delete to allocate and release memory.
 * It conforms the standards of TIsAllocator type trait and is compatible with named requirement CrescentAllocator.
 * This type is the perfect example on the overview of CrescentAllocator NRQ if you need to implement a custom allocator.
 * Allocators are usually used in containers. Their main purpose is to allocate and release (not construct & destruct) elements of one type.
 * 
 * 
 * TDefaultAllocator is stateless.
 * TDefaultAllocator satisfies CrescentAllocator and CrescentBoundingAllocator.
 */
template < typename T, typename TSize = uint_t >
class TDefaultAllocator
{
public:
	typedef T     ValueType;
	typedef TSize SizeType;

	template <typename U, typename OtherSz>
	using Rebind = TDefaultAllocator<U, OtherSz>;
public:
	constexpr TDefaultAllocator() = default;
	constexpr TDefaultAllocator(const TDefaultAllocator&) = default;

	// Since TDefaultAllocator is stateless, any TDefaultAllocator specialization can be converted to one another.
	template <typename OtherT, typename OtherSize>
	constexpr FORCEINLINE TDefaultAllocator(const TDefaultAllocator<OtherT, OtherSize>&)
	{
	}
	constexpr ~TDefaultAllocator() = default;

	constexpr FORCEINLINE TDefaultAllocator& operator=(const TDefaultAllocator&)
	{
		return *this;
	}
	template <typename OtherT, typename OtherSize>
	constexpr FORCEINLINE TDefaultAllocator& operator=(const TDefaultAllocator<OtherT, OtherSize>&)
	{
		return *this;
	}

	constexpr FORCEINLINE T* Allocate(TSize N)
	{
		return (T*) FMemory::Malloc(N * sizeof(T), alignof(T));
	}

	constexpr FORCEINLINE void Deallocate(T* Ptr, TSize /*N*/)
	{
		FMemory::Free(Ptr);
	}

	// We don't have to provide Construct() or Destruct() since TAllocatorTraits provides default ones.
	// Allocators must implement at least Allocate() and Deallocate().
	
	constexpr FORCEINLINE bool operator==(const TDefaultAllocator&) const
	{
		return true;
	}
	constexpr FORCEINLINE bool operator!=(const TDefaultAllocator&) const
	{
		return false;
	}
};
