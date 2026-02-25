// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Vanilla/VanillaPlatform.h"

#if CC_PLATFORM_WINDOWS
	#include "Windows/WindowsPlatform.h"
#elif CC_PLATFORM_LINUX
	#include "Linux/LinuxPlatform.h"
#else
	#error Invalid platform
#endif

typedef FPlatformTypes::int8   int8;
typedef FPlatformTypes::int16  int16;
typedef FPlatformTypes::int32  int32;
typedef FPlatformTypes::int64  int64;

typedef FPlatformTypes::uint8  uint8;
typedef FPlatformTypes::uint16 uint16;
typedef FPlatformTypes::uint32 uint32;
typedef FPlatformTypes::uint64 uint64;

typedef FPlatformTypes::isize  sint_t;
typedef FPlatformTypes::usize  uint_t;

typedef FPlatformTypes::TypeOfNullptr TypeOfNullptr;
