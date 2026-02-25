// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

template <typename T>
struct TRemoveReference
{
	typedef T Type;
};

template <typename T>
struct TRemoveReference<T&>
{
	typedef T Type;
};

template <typename T>
struct TRemoveReference<T&&>
{
	typedef T Type;
};

template <typename T>
using TRemoveReference_t = typename TRemoveReference<T>::Type;
