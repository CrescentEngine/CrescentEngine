// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Templates/TypeTraits/EnableIf.h"

// Use in angular bracket scope
#define CC_REQUIRES(...) , typename TEnableIf<(__VA_ARGS__), int>::Type = 0
