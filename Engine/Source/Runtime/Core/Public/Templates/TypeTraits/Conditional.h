// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

template <bool Expression, typename TrueType, typename FalseType>
struct TConditional
{
	typedef TrueType Type;
};

template <typename TrueType, typename FalseType>
struct TConditional<false, TrueType, FalseType>
{
	typedef FalseType Type;
};

template <bool Expression, typename TrueType, typename FalseType>
using TConditional_t = typename TConditional<Expression, TrueType, FalseType>::Type;
