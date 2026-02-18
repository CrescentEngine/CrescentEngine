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
struct TIsFloatingPoint : TIsConstVolatileUnqualifiedFloatingPoint<typename TRemoveConstVolatile<T>::Type>
{
};
