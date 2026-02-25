// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

template <typename T>
struct TTypeIdentity
{
	typedef T Type;
};

template <typename T>
using TTypeIdentity_t = typename TTypeIdentity<T>::Type;
