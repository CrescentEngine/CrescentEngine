// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Templates/TypeTraits/IsArithmetic.h"
#include "Templates/Require.h"

template <
	typename T
	CC_REQUIRES(TIsArithmetic<T>::Value)
>
struct TIsUnsigned : TBoolConstant<T(0) < T(-1)>
{
};

template <typename T>
inline constexpr bool TIsUnsigned_v = TIsUnsigned<T>::Value;
