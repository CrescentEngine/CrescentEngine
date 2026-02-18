// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

template <typename T>
struct TRemoveConst
{
	typedef T Type;
};
template <typename T>
struct TRemoveConst<const T>
{
	typedef T Type;
};

template <typename T>
struct TRemoveVolatile
{
	typedef T Type;
};
template <typename T>
struct TRemoveVolatile<volatile T>
{
	typedef T Type;
};

template <typename T>
struct TRemoveConstVolatile
{
	typedef T Type;
};
template <typename T>
struct TRemoveConstVolatile<const T>
{
	typedef T Type;
};
template <typename T>
struct TRemoveConstVolatile<volatile T>
{
	typedef T Type;
};
template <typename T>
struct TRemoveConstVolatile<const volatile T>
{
	typedef T Type;
};
