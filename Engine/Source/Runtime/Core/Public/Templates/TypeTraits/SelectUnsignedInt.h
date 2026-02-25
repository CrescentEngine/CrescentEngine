// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "HAL/Platform.h"

template <uint_t N>
struct TSelectUnsignedInt
{
	static_assert("Invalid integral type size.");
};
template <>
struct TSelectUnsignedInt<1>
{
	typedef uint8 Type;
};
template <>
struct TSelectUnsignedInt<2>
{
	typedef uint16 Type;
};
template <>
struct TSelectUnsignedInt<4>
{
	typedef uint32 Type;
};
template <>
struct TSelectUnsignedInt<8>
{
	typedef uint64 Type;
};

template <uint_t N>
using TSelectUnsignedInt_t = typename TSelectUnsignedInt<N>::Type;
