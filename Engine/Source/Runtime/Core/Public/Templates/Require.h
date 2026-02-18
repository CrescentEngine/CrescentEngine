// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Templates/TypeTraits/EnableIf.h"

#define CC_REQUIRES(...) , typename TEnableIf<__VA_ARGS__, int>::Type = 0
