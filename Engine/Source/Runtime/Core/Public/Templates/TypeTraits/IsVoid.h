// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "Templates/TypeTraits/IntegralConstant.h"

template <typename T>
struct TIsVoid : FFalseType
{
};

template <> struct TIsVoid<               void> : FTrueType {};
template <> struct TIsVoid<const          void> : FTrueType {};
template <> struct TIsVoid<      volatile void> : FTrueType {};
template <> struct TIsVoid<const volatile void> : FTrueType {};

template <typename T>
inline constexpr bool TIsVoid_v = TIsVoid<T>::Value;
