// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Templates/TypeTraits/IntegralConstant.h"
#include "Templates/TypeTraits/AddConstVolatile.h"
#include "Templates/TypeTraits/AddReference.h"

#if !defined CC_COMPILER_MSVC && !defined CC_COMPILER_CLANG && !defined CC_COMPILER_GCC
	#error Compiler is not guaranteed to support __is_constructible at all or in the expected syntax!
#endif

// Direct assignment

template <typename To, typename From>
struct TIsAssignable : TBoolConstant<__is_assignable(To, From)>
{
};

template <typename To, typename From>
struct TIsTriviallyAssignable : TBoolConstant<__is_trivially_assignable(To, From)>
{
};

template <typename To, typename From>
struct TIsNoThrowAssignable : TBoolConstant<__is_nothrow_assignable(To, From)>
{
};

// Copy assignment

template <typename T>
struct TIsCopyAssignable : TIsAssignable<
	typename TAddLValueReference<T>::Type,
	typename TAddConst<typename TAddLValueReference<T>::Type>::Type
>
{
};

template <typename T>
struct TIsTriviallyCopyAssignable : TIsTriviallyAssignable<
	typename TAddLValueReference<T>::Type,
	typename TAddConst<typename TAddLValueReference<T>::Type>::Type
>
{
};

template <typename T>
struct TIsNoThrowCopyAssignable : TIsNoThrowAssignable<
	typename TAddLValueReference<T>::Type,
	typename TAddConst<typename TAddLValueReference<T>::Type>::Type
>
{
};

// Move assignment

template <typename T>
struct TIsMoveAssignable : TIsAssignable<
	typename TAddLValueReference<T>::Type,
	typename TAddRValueReference<T>::Type
>
{
};

template <typename T>
struct TIsTriviallyMoveAssignable : TIsTriviallyAssignable<
	typename TAddLValueReference<T>::Type,
	typename TAddRValueReference<T>::Type
>
{
};

template <typename T>
struct TIsNoThrowMoveAssignable : TIsNoThrowAssignable<
	typename TAddLValueReference<T>::Type,
	typename TAddRValueReference<T>::Type
>
{
};
