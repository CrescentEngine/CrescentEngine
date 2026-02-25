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

template <typename To, typename From>
inline constexpr bool TIsAssignable_v = TIsAssignable<To, From>::Value;

template <typename To, typename From>
inline constexpr bool TIsTriviallyAssignable_v = TIsTriviallyAssignable<To, From>::Value;

template <typename To, typename From>
inline constexpr bool TIsNoThrowAssignable_v = TIsNoThrowAssignable<To, From>::Value;

// Copy assignment

template <typename T>
struct TIsCopyAssignable : TIsAssignable<TAddLValueReference_t<T>, TAddConst_t<TAddLValueReference_t<T>>>
{
};

template <typename T>
struct TIsTriviallyCopyAssignable : TIsTriviallyAssignable<TAddLValueReference_t<T>, TAddConst_t<TAddLValueReference_t<T>>>
{
};

template <typename T>
struct TIsNoThrowCopyAssignable : TIsNoThrowAssignable<TAddLValueReference_t<T>, TAddConst_t<TAddLValueReference_t<T>>>
{
};

template <typename T>
inline constexpr bool TIsCopyAssignable_v = TIsCopyAssignable<T>::Value;

template <typename T>
inline constexpr bool TIsTriviallyCopyAssignable_v = TIsTriviallyCopyAssignable<T>::Value;

template <typename T>
inline constexpr bool TIsNoThrowCopyAssignable_v = TIsNoThrowCopyAssignable<T>::Value;

// Move assignment

template <typename T>
struct TIsMoveAssignable : TIsAssignable<TAddLValueReference_t<T>, TAddRValueReference_t<T>>
{
};

template <typename T>
struct TIsTriviallyMoveAssignable : TIsTriviallyAssignable<TAddLValueReference_t<T>, TAddRValueReference_t<T>>
{
};

template <typename T>
struct TIsNoThrowMoveAssignable : TIsNoThrowAssignable<TAddLValueReference_t<T>, TAddRValueReference_t<T>>
{
};

template <typename T>
inline constexpr bool TIsMoveAssignable_v = TIsMoveAssignable<T>::Value;

template <typename T>
inline constexpr bool TIsTriviallyMoveAssignable_v = TIsTriviallyMoveAssignable<T>::Value;

template <typename T>
inline constexpr bool TIsNoThrowMoveAssignable_v = TIsNoThrowMoveAssignable<T>::Value;
