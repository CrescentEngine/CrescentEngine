// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

template <typename T, T Val>
struct TIntegralConstant
{
	typedef T ValueType;
	typedef TIntegralConstant Type;
	static constexpr T Value = Val;

	constexpr operator T() const noexcept { return Val; }
	constexpr T operator()() const noexcept { return Val; }
};

template <bool Value>
using TBoolConstant = TIntegralConstant<bool, Value>;

typedef TBoolConstant<false> FFalseType;
typedef TBoolConstant<true>  FTrueType;
