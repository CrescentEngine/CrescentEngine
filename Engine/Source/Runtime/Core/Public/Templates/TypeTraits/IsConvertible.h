// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Templates/TypeTraits/IntegralConstant.h"

#if defined CC_COMPILER_MSVC
	#define isconv __is_convertible_to
#elif defined CC_COMPILER_CLANG || defined CC_COMPILER_GCC
	#define isconv __is_convertible
#else
	#error Compiler is not guaranteed to support isconv at all or in the expected syntax!
#endif

template <typename From, typename To>
struct TIsConvertible : TBoolConstant<isconv(From, To)>
{
};

template <typename From, typename To>
inline constexpr bool TIsConvertible_v = TIsConvertible<From, To>::Value;
