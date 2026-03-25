// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "Templates/TypeTraits/EnableIf.h"

// Use in angular bracket scope
#define CC_REQUIRES(...) , TEnableIf_t<(__VA_ARGS__), int> = 0
