// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Vanilla/VanillaPlatformMemory.h"

#if CC_PLATFORM_WINDOWS
	#include "Windows/WindowsPlatformMemory.h"
#elif CC_PLATFORM_LINUX
	#include "Linux/LinuxPlatformMemory.h"
#else
	#error Invalid platform
#endif
