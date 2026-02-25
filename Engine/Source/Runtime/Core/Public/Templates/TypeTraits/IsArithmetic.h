// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Templates/TypeTraits/IsFloatingPoint.h"
#include "Templates/TypeTraits/IsIntegral.h"

template <typename T>
struct TIsConstVolatileUnqualifiedArithmetic
	: TBoolConstant<TIsConstVolatileUnqualifiedIntegral_v<T>|| TIsConstVolatileUnqualifiedFloatingPoint_v<T>>
{
};

template <typename T>
struct TIsArithmetic : TBoolConstant<TIsIntegral_v<T> || TIsFloatingPoint_v<T>>
{
};

template <typename T>
inline constexpr bool TIsConstVolatileUnqualifiedArithmetic_v = TIsConstVolatileUnqualifiedArithmetic<T>::Value;

template <typename T>
inline constexpr bool TIsArithmetic_v = TIsArithmetic<T>::Value;
