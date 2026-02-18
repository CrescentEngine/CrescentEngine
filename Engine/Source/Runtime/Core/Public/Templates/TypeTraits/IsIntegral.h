// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Templates/TypeTraits/RemoveConstVolatile.h"
#include "Templates/TypeTraits/IntegralConstant.h"

template <typename T>
struct TIsConstVolatileUnqualifiedIntegral : FFalseType
{
};
template <>
struct TIsConstVolatileUnqualifiedIntegral<bool> : FTrueType
{
};
template <>
struct TIsConstVolatileUnqualifiedIntegral<char> : FTrueType
{
};
template <>
struct TIsConstVolatileUnqualifiedIntegral<signed char> : FTrueType
{
};
template <>
struct TIsConstVolatileUnqualifiedIntegral<unsigned char> : FTrueType
{
};
template <>
struct TIsConstVolatileUnqualifiedIntegral<wchar_t> : FTrueType
{
};
#if CC_LANGUAGE_STANDARD >= 2020
template <>
struct TIsConstVolatileUnqualifiedIntegral<char8_t> : FTrueType
{
};
#endif
template <>
struct TIsConstVolatileUnqualifiedIntegral<char16_t> : FTrueType
{
};
template <>
struct TIsConstVolatileUnqualifiedIntegral<char32_t> : FTrueType
{
};
template <>
struct TIsConstVolatileUnqualifiedIntegral<short> : FTrueType
{
};
template <>
struct TIsConstVolatileUnqualifiedIntegral<int> : FTrueType
{
};
template <>
struct TIsConstVolatileUnqualifiedIntegral<long> : FTrueType
{
};
template <>
struct TIsConstVolatileUnqualifiedIntegral<long long> : FTrueType
{
};
template <>
struct TIsConstVolatileUnqualifiedIntegral<unsigned short> : FTrueType
{
};
template <>
struct TIsConstVolatileUnqualifiedIntegral<unsigned int> : FTrueType
{
};
template <>
struct TIsConstVolatileUnqualifiedIntegral<unsigned long> : FTrueType
{
};
template <>
struct TIsConstVolatileUnqualifiedIntegral<unsigned long long> : FTrueType
{
};

template <typename T>
struct TIsIntegral : TIsConstVolatileUnqualifiedIntegral<typename TRemoveConstVolatile<T>::Type>
{
};
