// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "Vanilla/VanillaPlatformAtomics.h"

#if CC_PLATFORM_WINDOWS
	#include "Windows/WindowsPlatformAtomics.h"
#elif CC_PLATFORM_LINUX
	#include "Linux/LinuxPlatformAtomics.h"
#else
	#error Invalid platform
#endif
