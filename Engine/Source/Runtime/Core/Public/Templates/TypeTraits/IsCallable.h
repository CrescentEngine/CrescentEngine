// Copyright (C) 2026 ychgen, all rights reserved.

/**
 * We really should base TIsCallable on other type traits such as TInvocable* set of traits,
 * however they'd take longer to implement and aren't needed for now, but TIsCallable
 * is currently needed for TSharedControlBlock SFINAE stuff.
 */

#pragma once

#include "Templates/TypeTraits/IntegralConstant.h"
#include "Templates/TypeTraits/Void.h"

#include "Templates/Utility/Declval.h"

namespace CCIsCallable_Private
{
	template <typename T, typename... Args>
	auto IsCallable(int) -> decltype(Declval<T>()(Declval<Args>()...), FTrueType{});

	template <typename, typename...>
	FFalseType IsCallable(...);
}

template <typename T, typename... Args>
struct TIsCallable : decltype(CCIsCallable_Private::IsCallable<T, Args...>(0))
{
};

template <typename T, typename... Args>
inline constexpr bool TIsCallable_v = TIsCallable<T, Args...>::Value;
