// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "Templates/Utility/Forward.h"

template <typename T>
struct TLess
{
	constexpr bool operator()(const T& LHS, const T& RHS) const
	{
		return LHS < RHS;
	}
};

template <>
struct TLess<void>
{
	using IsTransparent = void;

	template <typename T, typename U>
	// Deduce type using trailing type and LHS<RHS expr evaluation
	constexpr auto operator()(T&& LHS, U&& RHS) const -> decltype(Forward<T>(LHS) < Forward<U>(RHS))
	{
		return Forward<T>(LHS) < Forward<U>(RHS);
	}
};
