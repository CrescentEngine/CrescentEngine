// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

template <bool Expression, typename T = void>
struct TEnableIf
{
};
template <typename T>
struct TEnableIf<true, T>
{
	typedef T Type;
};
