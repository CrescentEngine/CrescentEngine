// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Templates/TypeTraits/IntegralConstant.h"
#include "Templates/TypeTraits/AddConstVolatile.h"
#include "Templates/TypeTraits/AddReference.h"

#if !defined CC_COMPILER_MSVC && !defined CC_COMPILER_CLANG && !defined CC_COMPILER_GCC
	#error Compiler is not guaranteed to support __is_constructible at all or in the expected syntax!
#endif

// Specialization construction

template <typename T, typename... Args>
struct TIsConstructible : TBoolConstant<__is_constructible(T, Args...)>
{
};

template <typename T, typename... Args>
struct TIsTriviallyConstructible : TBoolConstant<__is_trivially_constructible(T, Args...)>
{
};

template <typename T, typename... Args>
struct TIsNoThrowConstructible : TBoolConstant<__is_nothrow_constructible(T, Args...)>
{
};

// Default construction

template <typename T>
struct TIsDefaultConstructible : TIsConstructible<T>
{
};

template <typename T>
struct TIsTriviallyDefaultConstructible : TIsTriviallyConstructible<T>
{
};

template <typename T>
struct TIsNoThrowDefaultConstructible : TIsNoThrowConstructible<T>
{
};

// Copy construction

template <typename T>
struct TIsCopyConstructible : TIsConstructible<T, typename TAddConst<typename TAddLValueReference<T>::Type>::Type>
{
};

template <typename T>
struct TIsTriviallyCopyConstructible : TIsTriviallyConstructible<T, typename TAddConst<typename TAddLValueReference<T>::Type>::Type>
{
};

template <typename T>
struct TIsNoThrowCopyConstructible : TIsNoThrowConstructible<T, typename TAddConst<typename TAddLValueReference<T>::Type>::Type>
{
};

// Move construction

template <typename T>
struct TIsMoveConstructible : TIsConstructible<T, typename TAddRValueReference<T>::Type>
{
};

template <typename T>
struct TIsTriviallyMoveConstructible : TIsTriviallyConstructible<T, typename TAddRValueReference<T>::Type>
{
};

template <typename T>
struct TIsNoThrowMoveConstructible : TIsNoThrowConstructible<T, typename TAddRValueReference<T>::Type>
{
};
