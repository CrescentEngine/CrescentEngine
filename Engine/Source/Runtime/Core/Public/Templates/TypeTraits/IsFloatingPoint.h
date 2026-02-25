// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Templates/TypeTraits/RemoveConstVolatile.h"
#include "Templates/TypeTraits/IntegralConstant.h"

template <typename T>
struct TIsConstVolatileUnqualifiedFloatingPoint : FFalseType
{
};
template <>
struct TIsConstVolatileUnqualifiedFloatingPoint<float> : FTrueType
{
};
template <>
struct TIsConstVolatileUnqualifiedFloatingPoint<double> : FTrueType
{
};
template <>
struct TIsConstVolatileUnqualifiedFloatingPoint<long double> : FTrueType
{
};

template <typename T>
struct TIsFloatingPoint : TIsConstVolatileUnqualifiedFloatingPoint<TRemoveConstVolatile_t<T>>
{
};

template <typename T>
inline constexpr bool TIsConstVolatileUnqualifiedFloatingPoint_v = TIsConstVolatileUnqualifiedFloatingPoint<T>::Value;

template <typename T>
inline constexpr bool TIsFloatingPoint_v = TIsFloatingPoint<T>::Value;
