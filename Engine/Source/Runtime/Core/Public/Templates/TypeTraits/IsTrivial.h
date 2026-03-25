// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "Templates/TypeTraits/IntegralConstant.h"

template <typename T>
inline constexpr bool TIsTrivial_v = __is_trivial(T);

template <typename T>
struct TIsTrivial : TBoolConstant<TIsTrivial_v<T>>
{
};
