// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

template <typename T>
struct TAddConst
{
	typedef const T Type;
};

template <typename T>
struct TAddVolatile
{
	typedef volatile T Type;
};

template <typename T>
struct TAddConstVolatile
{
	typedef const volatile T Type;
};

template <typename T>
using TAddConst_t = typename TAddConst<T>::Type;

template <typename T>
using TAddVolatile_t = typename TAddVolatile<T>::Type;

template <typename T>
using TAddConstVolatile_t = typename TAddConstVolatile<T>::Type;
