// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Templates/TypeTraits/IntegralConstant.h"

template <typename T, typename U>
struct TIsSame : FFalseType
{
};
template <typename T>
struct TIsSame<T, T> : FTrueType
{
};

template <typename T, typename U>
inline constexpr bool TIsSame_v = TIsSame<T, U>::Value;
