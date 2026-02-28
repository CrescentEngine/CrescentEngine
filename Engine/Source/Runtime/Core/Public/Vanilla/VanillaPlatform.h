// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

// The C++ standard doesn't provide well-defined integer types with certain bit sizes.
// The core language lacks this, however <stdint.h> does provide standard ones. They are just not fundamental types.
#include <cstdint>

#define PLATFORM_HAS_NATIVE_INT128_TYPE 0

namespace CCPlatform_Private
{
	// We cannot use Templates/TypeTraits/SelectSignedInt.h or SelectUnsignedInt.h as those work with
	// engine provided types from FPlatformTypes. This file is ultimate root source of FPlatformTypes
	// so we cannot use that as the types it expects aren't available yet.

	template <typename Type__N_EQ_1, typename Type__N_EQ_2, typename Type__N_EQ_4, typename Type__N_EQ_8, int N>
	struct TPrimitiveSelectBySize
	{
	};
	template <typename Type__N_EQ_1, typename Type__N_EQ_2, typename Type__N_EQ_4, typename Type__N_EQ_8>
	struct TPrimitiveSelectBySize<Type__N_EQ_1, Type__N_EQ_2, Type__N_EQ_4, Type__N_EQ_8, 1>
	{
		typedef Type__N_EQ_1 Type;
	};
	template <typename Type__N_EQ_1, typename Type__N_EQ_2, typename Type__N_EQ_4, typename Type__N_EQ_8>
	struct TPrimitiveSelectBySize<Type__N_EQ_1, Type__N_EQ_2, Type__N_EQ_4, Type__N_EQ_8, 2>
	{
		typedef Type__N_EQ_2 Type;
	};
	template <typename Type__N_EQ_1, typename Type__N_EQ_2, typename Type__N_EQ_4, typename Type__N_EQ_8>
	struct TPrimitiveSelectBySize<Type__N_EQ_1, Type__N_EQ_2, Type__N_EQ_4, Type__N_EQ_8, 4>
	{
		typedef Type__N_EQ_4 Type;
	};
	template <typename Type__N_EQ_1, typename Type__N_EQ_2, typename Type__N_EQ_4, typename Type__N_EQ_8>
	struct TPrimitiveSelectBySize<Type__N_EQ_1, Type__N_EQ_2, Type__N_EQ_4, Type__N_EQ_8, 8>
	{
		typedef Type__N_EQ_8 Type;
	};

	template <typename Type__N_EQ_1, typename Type__N_EQ_2, typename Type__N_EQ_4, typename Type__N_EQ_8, int N>
	using TPrimitiveSelectBySize_t = typename TPrimitiveSelectBySize<Type__N_EQ_1, Type__N_EQ_2, Type__N_EQ_4, Type__N_EQ_8, N>::Type;
}

struct FVanillaPlatformTypes
{
	typedef int8_t   int8;
	typedef int16_t  int16;
	typedef int32_t  int32;
	typedef int64_t  int64;

	typedef uint8_t  uint8;
	typedef uint16_t uint16;
	typedef uint32_t uint32;
	typedef uint64_t uint64;

	#if CC_ARCH_BITNESS == 32
		typedef int32_t  sint_t;
		typedef uint32_t uint_t;
	#elif CC_ARCH_BITNESS == 64
		typedef int64_t  sint_t;
		typedef uint64_t uint_t;
	#else
		#error Invalid platform bitness
	#endif

	typedef CCPlatform_Private::TPrimitiveSelectBySize_t< int8_t,  int16_t,  int32_t,  int64_t, sizeof(void*)> PTRINT;
	typedef CCPlatform_Private::TPrimitiveSelectBySize_t<uint8_t, uint16_t, uint32_t, uint64_t, sizeof(void*)> UPTRINT;

	typedef decltype(nullptr) TypeOfNullptr;
};
