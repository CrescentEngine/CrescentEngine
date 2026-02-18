// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

struct FWindowsPlatformTypes : public FVanillaPlatformTypes
{
	typedef   signed __int8  int8;
	typedef   signed __int16 int16;
	typedef   signed __int32 int32;
	typedef   signed __int64 int64;

	typedef unsigned __int8  uint8;
	typedef unsigned __int16 uint16;
	typedef unsigned __int32 uint32;
	typedef unsigned __int64 uint64;

	#if CC_ARCH_BITNESS == 32
		typedef   signed __int32 sint_t;
		typedef unsigned __int32 uint_t;
	#elif CC_ARCH_BITNESS == 64
		typedef   signed __int64 sint_t;
		typedef unsigned __int64 uint_t;
	#else
		#error Invalid platform bitness
	#endif
};
typedef FWindowsPlatformTypes FPlatformTypes;
