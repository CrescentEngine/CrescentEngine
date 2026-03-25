// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "Templates/TypeTraits/IntegralConstant.h"

template <typename T>
inline constexpr bool TIsDestructible_v = __is_destructible(T);

template <typename T>
inline constexpr bool TIsTriviallyDestructible_v = __is_trivially_destructible(T);

template <typename T>
inline constexpr bool TIsNoThrowDestructible_v = __is_nothrow_destructible(T);

template <typename T>
struct TIsDestructible : TBoolConstant<TIsDestructible_v<T>>
{
};

template <typename T>
struct TIsTriviallyDestructible : TBoolConstant<TIsTriviallyDestructible_v<T>>
{
};

template <typename T>
struct TIsNoThrowDestructible : TBoolConstant<TIsNoThrowDestructible_v<T>>
{
};
