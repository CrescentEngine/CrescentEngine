// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "CoreDefines.h"
#include "HAL/Platform.h"

#include "Templates/TypeTraits/IsIntegral.h"
#include "Templates/TypeTraits/IsPointer.h"
#include "Templates/Require.h"

template
<
	typename T
	CC_REQUIRES(TIsIntegral<T>::Value || TIsPointer<T>::Value)
>
constexpr FORCEINLINE T Align(T Value, uint64 Alignment)
{
	return (T)(((uint64) Value + Alignment - 1) & ~(Alignment - 1));
}
