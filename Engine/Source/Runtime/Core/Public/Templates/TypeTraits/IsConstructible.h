// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "Templates/TypeTraits/IntegralConstant.h"
#include "Templates/TypeTraits/AddConstVolatile.h"
#include "Templates/TypeTraits/AddReference.h"
#include "Templates/TypeTraits/Void.h"

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

template <typename T, typename... Args>
inline constexpr bool TIsConstructible_v = TIsConstructible<T, Args...>::Value;

template <typename T, typename... Args>
inline constexpr bool TIsTriviallyConstructible_v = TIsTriviallyConstructible<T, Args...>::Value;

template <typename T, typename... Args>
inline constexpr bool TIsNoThrowConstructible_v = TIsNoThrowConstructible<T, Args...>::Value;

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

template <typename T>
inline constexpr bool TIsDefaultConstructible_v = TIsDefaultConstructible<T>::Value;

template <typename T>
inline constexpr bool TIsTriviallyDefaultConstructible_v = TIsTriviallyDefaultConstructible<T>::Value;

template <typename T>
inline constexpr bool TIsNoThrowDefaultConstructible_v = TIsNoThrowDefaultConstructible<T>::Value;

// Copy construction

template <typename T>
struct TIsCopyConstructible : TIsConstructible<T, TAddConst_t<TAddLValueReference_t<T>>>
{
};

template <typename T>
struct TIsTriviallyCopyConstructible : TIsTriviallyConstructible<T, TAddConst_t<TAddLValueReference_t<T>>>
{
};

template <typename T>
struct TIsNoThrowCopyConstructible : TIsNoThrowConstructible<T, TAddConst_t<TAddLValueReference_t<T>>>
{
};

template <typename T>
inline constexpr bool TIsCopyConstructible_v = TIsCopyConstructible<T>::Value;

template <typename T>
inline constexpr bool TIsTriviallyCopyConstructible_v = TIsTriviallyCopyConstructible<T>::Value;

template <typename T>
inline constexpr bool TIsNoThrowCopyConstructible_v = TIsNoThrowCopyConstructible<T>::Value;

// Move construction

template <typename T>
struct TIsMoveConstructible : TIsConstructible<T, TAddRValueReference_t<T>>
{
};

template <typename T>
struct TIsTriviallyMoveConstructible : TIsTriviallyConstructible<T, TAddRValueReference_t<T>>
{
};

template <typename T>
struct TIsNoThrowMoveConstructible : TIsNoThrowConstructible<T, TAddRValueReference_t<T>>
{
};

template <typename T>
inline constexpr bool TIsMoveConstructible_v = TIsMoveConstructible<T>::Value;

template <typename T>
inline constexpr bool TIsTriviallyMoveConstructible_v = TIsTriviallyMoveConstructible<T>::Value;

template <typename T>
inline constexpr bool TIsNoThrowMoveConstructible_v = TIsNoThrowMoveConstructible<T>::Value;

// Extra

namespace CCIsConstructible_Private
{
	template <typename T>
	void ImplicitlyDefaultConstruct(const T&);
}

template <typename T, typename = void>
struct TIsImplicitlyDefaultConstructible : FFalseType
{
};

template <typename T>
struct TIsImplicitlyDefaultConstructible<T, TVoid<decltype(CCIsConstructible_Private::ImplicitlyDefaultConstruct<T>({})) >> : FTrueType
{
};

template <typename T>
inline constexpr bool TIsImplicitlyDefaultConstructible_v = TIsImplicitlyDefaultConstructible<T>::Value;
