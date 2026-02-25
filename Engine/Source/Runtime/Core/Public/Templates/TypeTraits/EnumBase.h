// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Templates/TypeTraits/IsEnum.h"

#if !defined CC_COMPILER_MSVC && !defined CC_COMPILER_CLANG && !defined CC_COMPILER_GCC
	#error Compiler is not guaranteed to support __underlying_type!
#endif

template <typename T, bool = TIsEnum<T>::Value>
struct TEnumBase
{
};
template <typename T>
struct TEnumBase<T, true>
{
	typedef __underlying_type(T) Type;
};

template <typename T>
using TEnumBase_t = typename TEnumBase<T>::Type;
