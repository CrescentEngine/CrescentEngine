// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "Templates/TypeTraits/IntegralConstant.h"

#if !defined CC_COMPILER_MSVC && !defined CC_COMPILER_CLANG && !defined CC_COMPILER_GCC
	#error Compiler is not guaranteed to support __is_empty!
#endif

template <typename T>
struct TIsEmpty : TBoolConstant<__is_empty(T)>
{
};

template <typename T>
inline constexpr bool TIsEmpty_v = TIsEmpty<T>::Value;
