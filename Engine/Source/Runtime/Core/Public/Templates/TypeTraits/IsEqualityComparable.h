// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "Templates/TypeTraits/IntegralConstant.h"
#include "Templates/Utility/Declval.h"

namespace CCIsEqualityComparable_Private
{
	template <typename T, typename With>
	auto IsEqualityComparableImpl(int) -> decltype(Declval<T>() == Declval<With>(), FTrueType{});
	template <typename, typename>
	FFalseType IsEqualityComparableImpl(...);
}

template <typename T, typename With>
struct TIsEqualityComparableWith
	: TBoolConstant<decltype(CCIsEqualityComparable_Private::IsEqualityComparableImpl<T, With>(0))::Value>
{
};

template <typename T>
struct TIsEqualityComparable : TIsEqualityComparableWith<T, T>
{
};

template <typename T, typename With>
inline constexpr bool TIsEqualityComparableWith_v = TIsEqualityComparableWith<T, With>::Value;

template <typename T>
inline constexpr bool TIsEqualityComparable_v = TIsEqualityComparable<T>::Value;
