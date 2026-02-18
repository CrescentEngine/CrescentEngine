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
