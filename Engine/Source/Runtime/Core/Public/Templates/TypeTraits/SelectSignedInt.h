// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "HAL/Platform.h"

template <uint_t N>
struct TSelectSignedInt
{
	static_assert("Invalid integral type size.");
};
template <>
struct TSelectSignedInt<1>
{
	typedef int8 Type;
};
template <>
struct TSelectSignedInt<2>
{
	typedef int16 Type;
};
template <>
struct TSelectSignedInt<4>
{
	typedef int32 Type;
};
template <>
struct TSelectSignedInt<8>
{
	typedef int64 Type;
};

template <uint_t N>
using TSelectSignedInt_t = typename TSelectSignedInt<N>::Type;
