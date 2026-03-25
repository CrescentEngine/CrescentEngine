// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "HAL/Platform.h"

#include "Templates/TypeTraits/Bound.h"

template <typename T>
struct TIsArray : TBoolConstant<TBound_v<T> != ETypeBound::None>
{
};

template <typename T>
struct TIsBoundedArray : TBoolConstant<TBound_v<T> == ETypeBound::Bounded>
{
};

template <typename T>
struct TIsUnboundedArray : TBoolConstant<TBound_v<T> == ETypeBound::Unbounded>
{
};


template <typename T>
inline constexpr bool TIsArray_v = TIsArray<T>::Value;

template <typename T>
inline constexpr bool TIsBoundedArray_v = TIsBoundedArray<T>::Value;

template <typename T>
inline constexpr bool TIsUnboundedArray_v = TIsUnboundedArray<T>::Value;
