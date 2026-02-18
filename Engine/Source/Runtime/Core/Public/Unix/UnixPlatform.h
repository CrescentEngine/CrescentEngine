// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#ifdef __SIZEOF_INT128__
	#undef  PLATFORM_HAS_NATIVE_INT128_TYPE
	#define PLATFORM_HAS_NATIVE_INT128_TYPE 1
#endif

struct FUnixPlatformTypes : public FVanillaPlatformTypes
{
#if PLATFORM_HAS_NATIVE_INT128_TYPE
	typedef signed   __int128 int128;
	typedef unsigned __int128 uint128;
#endif

	typedef __SIZE_TYPE__ uint_t;
};
typedef FUnixPlatformTypes FPlatformTypes;
