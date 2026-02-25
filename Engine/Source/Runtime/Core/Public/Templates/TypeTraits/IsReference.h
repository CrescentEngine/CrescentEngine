// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Templates/TypeTraits/IntegralConstant.h"

template <typename T>
struct TIsReference : FFalseType
{
};
template <typename T>
struct TIsReference<T&> : FTrueType
{
};
template <typename T>
struct TIsReference<T&&> : FTrueType
{
};

template <typename T>
struct TIsLValueReference : FFalseType
{
};
template <typename T>
struct TIsLValueReference<T&> : FTrueType
{
};

template <typename T>
struct TIsRValueReference : FFalseType
{
};
template <typename T>
struct TIsRValueReference<T&&> : FTrueType
{
};

template <typename T>
inline constexpr bool TIsReference_v = TIsReference<T>::Value;

template <typename T>
inline constexpr bool TIsLValueReference_v = TIsLValueReference<T>::Value;

template <typename T>
inline constexpr bool TIsRValueReference_v = TIsRValueReference<T>::Value;
