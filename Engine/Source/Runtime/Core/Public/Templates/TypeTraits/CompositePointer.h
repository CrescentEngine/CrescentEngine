// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "Templates/TypeTraits/IsConvertible.h"
#include "Templates/TypeTraits/Conditional.h"

template <typename T, typename U>
struct TCompositePointer
{
	typedef TConditional_t<TIsConvertible_v<T*, U*>, U*, TConditional_t<TIsConvertible_v<U*, T*>, T*, void*>> Type;
};

template <typename T, typename U>
using TCompositePointer_t = typename TCompositePointer<T, U>::Type;
