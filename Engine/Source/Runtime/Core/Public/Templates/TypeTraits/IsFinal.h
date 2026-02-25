// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "Templates/TypeTraits/IntegralConstant.h"

#if !defined CC_COMPILER_MSVC && !defined CC_COMPILER_CLANG && !defined CC_COMPILER_GCC
	#error Compiler is not guaranteed to support __is_final!
#endif

template <typename T>
struct TIsFinal : TBoolConstant<__is_final(T)>
{
};

template <typename T>
inline constexpr bool TIsFinal_v = TIsFinal<T>::Value;
