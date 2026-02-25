// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "HAL/Platform.h" // for uint_t

template <typename T>
struct TRemoveExtent
{
	typedef T Type;
};

template <typename T>
struct TRemoveExtent<T[]>
{
	typedef T Type;
};

template <typename T, uint_t N>
struct TRemoveExtent<T[N]>
{
	typedef T Type;
};

template <typename T>
using TRemoveExtent_t = typename TRemoveExtent<T>::Type;
