// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "HAL/Platform.h"

#include "Templates/TypeTraits/IntegralConstant.h"

enum class ETypeBound
{
	None,      // Non-array type,       example: int
	Unbounded, // Unbounded array type, example: int[]
	Bounded    // Bounded array type,   example: int[5]
};

template <typename T>
struct TBound : TIntegralConstant<ETypeBound, ETypeBound::None>
{
};

template <typename T>
struct TBound<T[]> : TIntegralConstant<ETypeBound, ETypeBound::Unbounded>
{
};

template <typename T, uint_t N>
struct TBound<T[N]> : TIntegralConstant<ETypeBound, ETypeBound::Bounded>
{
};

template <typename T>
inline constexpr ETypeBound TBound_v = TBound<T>::Value;
