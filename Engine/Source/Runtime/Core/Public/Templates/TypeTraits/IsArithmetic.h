// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Templates/TypeTraits/IsFloatingPoint.h"
#include "Templates/TypeTraits/IsIntegral.h"

template <typename T>
struct TIsConstVolatileUnqualifiedArithmetic
	: TBoolConstant<TIsConstVolatileUnqualifiedIntegral<T>::Value || TIsConstVolatileUnqualifiedFloatingPoint<T>::Value>
{
};

template <typename T>
struct TIsArithmetic
	: TBoolConstant<TIsIntegral<T>::Value || TIsFloatingPoint<T>::Value>
{
};

