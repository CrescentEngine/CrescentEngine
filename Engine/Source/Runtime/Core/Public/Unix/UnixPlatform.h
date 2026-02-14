// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Vanilla/VanillaPlatform.h"

struct FUnixPlatformTypes : public FVanillaPlatformTypes
{
	typedef __SIZE_TYPE__ usize;
};
typedef FUnixPlatformTypes FPlatformTypes;
