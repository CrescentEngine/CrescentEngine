// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

// The C++ standard doesn't provide well-defined integer types with certain bit sizes.
// The core language lacks this, however <stdint.h> does provide standard ones. They are just not fundamental types.
#include <cstdint>

#define PLATFORM_HAS_NATIVE_INT128_TYPE 0

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
		typedef int32_t  isize;
		typedef uint32_t usize;
	#elif CC_ARCH_BITNESS == 64
		typedef int64_t  isize;
		typedef uint64_t usize;
	#else
		#error Invalid platform bitness
	#endif

	typedef decltype(nullptr) TypeOfNullPtr;
};
