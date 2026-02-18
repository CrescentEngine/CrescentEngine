// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Templates/TypeTraits/IntegralConstant.h"

template <typename T>
struct TIsReference : FFalseType
{
};
template <typename T>
struct TIsReference<T&> : FTrueType
{
};
template <typename T>
struct TIsReference<T&&> : FTrueType
{
};
