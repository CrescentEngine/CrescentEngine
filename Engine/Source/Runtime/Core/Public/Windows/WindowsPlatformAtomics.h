// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "CoreTypes.h"

#include "Vanilla/VanillaPlatformAtomics.h"

#include <intrin.h> // for _Interlocked* compiler intrinsics

struct FWindowsPlatformAtomics : public FVanillaPlatformAtomics
{
	/** MSVC specific. Instrinsics explicitly need these types as is. */
	typedef char      SIZE8_;  static_assert(sizeof(SIZE8_)  == sizeof(int8),  "FWindowsPlatformAtomics:  8-bit atomic abstraction type mismatch.");
	typedef short     SIZE16_; static_assert(sizeof(SIZE16_) == sizeof(int16), "FWindowsPlatformAtomics: 16-bit atomic abstraction type mismatch.");
	typedef long      SIZE32_; static_assert(sizeof(SIZE32_) == sizeof(int32), "FWindowsPlatformAtomics: 32-bit atomic abstraction type mismatch.");
	typedef long long SIZE64_; static_assert(sizeof(SIZE64_) == sizeof(int64), "FWindowsPlatformAtomics: 64-bit atomic abstraction type mismatch.");

	// Exchange 8bit
	static FORCEINLINE int8 InterlockedExchange(volatile int8* InValue, int8 Exchange)
	{
		return (int8) _InterlockedExchange8((volatile SIZE8_*) InValue, (SIZE8_) Exchange);
	}
	// Exchange 16bit
	static FORCEINLINE int16 InterlockedExchange(volatile int16* InValue, int16 Exchange)
	{
		return (int16) _InterlockedExchange16((volatile SIZE16_*) InValue, (SIZE16_) Exchange);
	}
	// Exchange 32bit
	static FORCEINLINE int32 InterlockedExchange(volatile int32* InValue, int32 Exchange)
	{
		return (int32) _InterlockedExchange((volatile SIZE32_*) InValue, (SIZE32_) Exchange);
	}
	// Exchange 64bit
	static FORCEINLINE int64 InterlockedExchange(volatile int64* InValue, int64 Exchange)
	{
		return (int64) _InterlockedExchange64((volatile SIZE64_*) InValue, (SIZE64_) Exchange);
	}
	// Exchange POINTER
	static FORCEINLINE void* InterlockedExchange(void** InDest, void* Exchange)
	{
	#if CC_ARCH_BITNESS >= 64
		return (void*) _InterlockedExchange64((SIZE64_*) InDest, (SIZE64_) Exchange);
	#else
		return (void*) _InterlockedExchange((SIZE32_*) InDest, (SIZE32_) Exchange);
	#endif
	}

	// CompareExchange 8bit
	static FORCEINLINE int8 InterlockedCompareExchange(volatile int8* InDest, int8 Exchange, int8 Comparand)
	{
		return (int8) _InterlockedCompareExchange8((volatile SIZE8_*) InDest, (SIZE8_) Exchange, (SIZE8_) Comparand);
	}
	// CompareExchange 16bit
	static FORCEINLINE int16 InterlockedCompareExchange(volatile int16* InDest, int16 Exchange, int16 Comparand)
	{
		return (int16) _InterlockedCompareExchange16((volatile SIZE16_*) InDest, (SIZE16_) Exchange, (SIZE16_) Comparand);
	}
	// CompareExchange 32bit
	static FORCEINLINE int32 InterlockedCompareExchange(volatile int32* InDest, int32 Exchange, int32 Comparand)
	{
		return (int32) _InterlockedCompareExchange((volatile SIZE32_*) InDest, (SIZE32_) Exchange, (SIZE32_) Comparand);
	}
	// CompareExchange 64bit
	static FORCEINLINE int64 InterlockedCompareExchange(volatile int64* InDest, int64 Exchange, int64 Comparand)
	{
		return (int64) _InterlockedCompareExchange64((volatile SIZE64_*) InDest, (SIZE64_) Exchange, (SIZE64_) Comparand);
	}
	// CompareExchange POINTER
	static FORCEINLINE void* InterlockedCompareExchange(void** InDest, void* Exchange, void* Comparand)
	{
	#if CC_ARCH_BITNESS >= 64
		return (void*) _InterlockedCompareExchange64((SIZE64_*) InDest, (SIZE64_) Exchange, (SIZE64_) Comparand);
	#else
		return (void*) _InterlockedCompareExchange((SIZE32_*) InDest, (SIZE32_) Exchange, (SIZE32_) Comparand);
	#endif
	}

	// Add 8bit
	static FORCEINLINE int8 InterlockedAdd(volatile int8* Addend, int8 Value)
	{
		return (int8) _InterlockedExchangeAdd8((volatile SIZE8_*) Addend, (SIZE8_) Value);
	}
	// Add 16bit
	static FORCEINLINE int16 InterlockedAdd(volatile int16* Addend, int16 Value)
	{
		return (int16) _InterlockedExchangeAdd16((volatile SIZE16_*) Addend, (SIZE16_) Value);
	}
	// Add 32bit
	static FORCEINLINE int32 InterlockedAdd(volatile int32* Addend, int32 Value)
	{
		return (int32) _InterlockedExchangeAdd((volatile SIZE32_*) Addend, (SIZE32_) Value);
	}
	// Add 64bit
	static FORCEINLINE int64 InterlockedAdd(volatile int64* Addend, int64 Value)
	{
	#if CC_ARCH_BITNESS >= 64
		return (int64) _InterlockedExchangeAdd64((volatile SIZE64_*) Addend, (SIZE64_) Value);
	#else
		// No _InterlockedExchangeAdd64 function exists on x86
		while (true)
		{
			int64 Prev = *Addend;
			if (_InterlockedCompareExchange64(Addend, Prev + Value, Prev) == Prev)
			{
				return Prev + Value;
			}
		}
	#endif
	}

	// Increment 8bit
	static FORCEINLINE int8 InterlockedIncrement(volatile int8* InValue)
	{
		// No _InterlockedIncrement8 function exists
		return (int8) _InterlockedExchangeAdd8((volatile SIZE8_*) InValue, 1) + 1;
	}
	// Increment 16bit
	static FORCEINLINE int16 InterlockedIncrement(volatile int16* InValue)
	{
		return (int16) _InterlockedIncrement16((volatile SIZE16_*) InValue);
	}
	// Increment 32bit
	static FORCEINLINE int32 InterlockedIncrement(volatile int32* InValue)
	{
		return (int32) _InterlockedIncrement((volatile SIZE32_*) InValue);
	}
	// Increment 64bit
	static FORCEINLINE int64 InterlockedIncrement(volatile int64* InValue)
	{
	#if CC_ARCH_BITNESS >= 64
		return (int64) _InterlockedIncrement64((volatile SIZE64_*) InValue);
	#else
		// No _InterlockedIncrement64 function exists on x86
		while (true)
		{
			int64 Prev = *InValue;
			if (_InterlockedCompareExchange64(InValue, Prev + 1, Prev) == Prev)
			{
				return Prev + 1;
			}
		}
	#endif
	}

	// Decrement 8bit
	static FORCEINLINE int8 InterlockedDecrement(volatile int8* InValue)
	{
		// No _InterlockedDecrement8 function exists
		return (int8) _InterlockedExchangeAdd8((volatile SIZE8_*) InValue, -1) - 1;
	}
	// Decrement 16bit
	static FORCEINLINE int16 InterlockedDecrement(volatile int16* InValue)
	{
		return (int16) _InterlockedDecrement16((volatile SIZE16_*) InValue);
	}
	// Decrement 32bit
	static FORCEINLINE int32 InterlockedDecrement(volatile int32* InValue)
	{
		return (int32) _InterlockedDecrement((volatile SIZE32_*) InValue);
	}
	// Decrement 64bit
	static FORCEINLINE int64 InterlockedDecrement(volatile int64* InValue)
	{
	#if CC_ARCH_BITNESS >= 64
		return (int64) _InterlockedDecrement64((volatile SIZE64_*) InValue);
	#else
		// No _InterlockedDecrement64 function exists on x86
		while (true)
		{
			int64 Prev = *InValue;
			if (_InterlockedCompareExchange64(InValue, Prev - 1, Prev) == Prev)
			{
				return Prev - 1;
			}
		}
	#endif
	}

	// Read 8bit
	static FORCEINLINE int8 AtomicRead(const volatile int8* InSrc)
	{
		return InterlockedCompareExchange((volatile int8*) InSrc, 0, 0);
	}
	// Read 16bit
	static FORCEINLINE int16 AtomicRead(const volatile int16* InSrc)
	{
		return InterlockedCompareExchange((volatile int16*) InSrc, 0, 0);
	}
	// Read 32bit
	static FORCEINLINE int32 AtomicRead(const volatile int32* InSrc)
	{
		return InterlockedCompareExchange((volatile int32*) InSrc, 0, 0);
	}
	// Read 64bit
	static FORCEINLINE int64 AtomicRead(const volatile int64* InSrc)
	{
		return InterlockedCompareExchange((volatile int64*) InSrc, 0, 0);
	}

	// Read_Relaxed 8bit
	static FORCEINLINE int8 AtomicRead_Relaxed(const volatile int8* InSrc)
	{
		return *InSrc;
	}
	// Read_Relaxed 16bit
	static FORCEINLINE int16 AtomicRead_Relaxed(const volatile int16* InSrc)
	{
		return *InSrc;
	}
	// Read_Relaxed 32bit
	static FORCEINLINE int32 AtomicRead_Relaxed(const volatile int32* InSrc)
	{
		return *InSrc;
	}
	// Read_Relaxed 64bit
	static FORCEINLINE int64 AtomicRead_Relaxed(const volatile int64* InSrc)
	{
	#if CC_ARCH_BITNESS >= 64
		return *InSrc;
	#else
		return InterlockedCompareExchange((volatile int64*) InSrc, 0, 0);
	#endif
	}

	// Store 8bit
	static FORCEINLINE void AtomicStore(volatile int8* InSrc, int8 Value)
	{
		InterlockedExchange(InSrc, Value);
	}
	// Store 16bit
	static FORCEINLINE void AtomicStore(volatile int16* InSrc, int16 Value)
	{
		InterlockedExchange(InSrc, Value);
	}
	// Store 32bit
	static FORCEINLINE void AtomicStore(volatile int32* InSrc, int32 Value)
	{
		InterlockedExchange(InSrc, Value);
	}
	// Store 64bit
	static FORCEINLINE void AtomicStore(volatile int64* InSrc, int64 Value)
	{
		InterlockedExchange(InSrc, Value);
	}

	// Store_Relaxed 8bit
	static FORCEINLINE void AtomicStore_Relaxed(volatile int8* InSrc, int8 Value)
	{
		*InSrc = Value;
	}
	// Store_Relaxed 16bit
	static FORCEINLINE void AtomicStore_Relaxed(volatile int16* InSrc, int16 Value)
	{
		*InSrc = Value;
	}
	// Store_Relaxed 32bit
	static FORCEINLINE void AtomicStore_Relaxed(volatile int32* InSrc, int32 Value)
	{
		*InSrc = Value;
	}
	// Store_Relaxed 64bit
	static FORCEINLINE void AtomicStore_Relaxed(volatile int64* InSrc, int64 Value)
	{
	#if CC_ARCH_BITNESS >= 64
		*InSrc = Value;
	#else
		InterlockedExchange(InSrc, Value);
	#endif
	}
};
typedef FWindowsPlatformAtomics FPlatformAtomics;
