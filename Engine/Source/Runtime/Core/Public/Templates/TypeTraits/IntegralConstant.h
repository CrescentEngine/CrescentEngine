// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

template <typename T, T Val>
struct TIntegralConstant
{
	typedef T ValueType;
	typedef TIntegralConstant Type;
	static constexpr T Value = Val;

	static constexpr operator T() const noexcept { return Val; }
	static constexpr T operator()() const noexcept { return Val; }
};

template <bool Value>
using TBoolConstant = TIntegralConstant<bool, Value>;

typedef TBoolConstant<false> FFalseType;
typedef TBoolConstant<true>  FTrueType;
