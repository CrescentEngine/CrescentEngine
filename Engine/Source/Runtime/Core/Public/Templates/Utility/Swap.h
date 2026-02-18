// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "HAL/Platform.h"

#include "Templates/TypeTraits/IsConstructible.h"
#include "Templates/TypeTraits/IsAssignable.h"
#include "Templates/Utility/Move.h"
#include "Templates/Require.h"

template
<
	typename T
	CC_REQUIRES(TIsMoveConstructible<T>::Value && TIsMoveAssignable<T>::Value)
>
constexpr FORCEINLINE void Swap(T& LHS, T& RHS)
{
	T Temp = Move(LHS);
	LHS    = Move(RHS);
	RHS    = Move(Temp);
}

template
<
	typename T,
	uint_t   N
	CC_REQUIRES(TIsMoveConstructible<T>::Value && TIsMoveAssignable<T>::Value)
>
constexpr FORCEINLINE void Swap(T(&LHS)[N], T(&RHS)[N])
{
	for (const T* LeftCur = LHS, const T* LeftEnd = LHS + N, const T* RightCur = RHS;
		 LeftCur != LeftEnd; LeftCur++, RightCur++)
	{
		Swap(*LeftCur, *RightCur);
	}
}
