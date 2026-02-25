// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

template <typename T>
struct TRemovePointer
{
	typedef T Type;
};

template <typename T>
struct TRemovePointer<T*>
{
	typedef T Type;
};

template <typename T>
struct TRemovePointer<T* const>
{
	typedef T Type;
};

template <typename T>
struct TRemovePointer<T* volatile>
{
	typedef T Type;
};

template <typename T>
struct TRemovePointer<T* const volatile>
{
	typedef T Type;
};

template <typename T>
using TRemovePointer_t = typename TRemovePointer<T>::Type;
