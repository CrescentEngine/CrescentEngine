// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "HAL/Platform.h"

#include "Templates/TypeTraits/IntegralConstant.h"

template <typename T>
struct TIsArray : FFalseType
{
};

template <typename T>
struct TIsArray<T[]> : FTrueType
{
};

template <typename T, uint_t N>
struct TIsArray<T[N]> : FTrueType
{
};

template <typename T>
struct TIsBoundedArray : FFalseType
{
};

template <typename T, uint_t N>
struct TIsBoundedArray<T[N]> : FTrueType
{
};

template <typename T>
struct TIsUnboundedArray : FFalseType
{
};

template <typename T>
struct TIsUnboundedArray<T[]> : FTrueType
{
};

template <typename T>
inline constexpr bool TIsArray_v = TIsArray<T>::Value;

template <typename T>
inline constexpr bool TIsBoundedArray_v = TIsBoundedArray<T>::Value;

template <typename T>
inline constexpr bool TIsUnboundedArray_v = TIsUnboundedArray<T>::Value;
