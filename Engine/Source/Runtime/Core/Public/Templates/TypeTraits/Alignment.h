// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "HAL/Platform.h"
#include "Templates/TypeTraits/IntegralConstant.h"

template <typename T>
struct TAlignmentOf : TIntegralConstant<uint_t, alignof(T)>
{
};
