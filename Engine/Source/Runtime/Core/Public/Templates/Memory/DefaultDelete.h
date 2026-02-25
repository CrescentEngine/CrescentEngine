// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "CoreDefines.h"

#include "Templates/TypeTraits/IsConvertible.h"
#include "Templates/TypeTraits/EnableIf.h"

namespace CCDefaultDelete_Private
{
	template <typename T, typename U>
	using TPtrConstraint = TEnableIf_t<TIsConvertible_v<U*, T*>, int>;

	template <typename T, typename U>
	using TArrayPtrConstraint = TEnableIf_t<TIsConvertible_v<U(*)[], T(*)[]>, int>;
}

template <typename T>
struct TDefaultDelete
{
	constexpr TDefaultDelete() = default;

	template <typename U, CCDefaultDelete_Private::TPtrConstraint<T, U> = 0>
	constexpr FORCEINLINE TDefaultDelete(const TDefaultDelete<U>&) {}

	constexpr FORCEINLINE void operator()(T* Ptr) const
	{
		delete Ptr;
	}
};

template <typename T>
struct TDefaultDelete<T[]>
{
	constexpr TDefaultDelete() = default;

	template <typename U, CCDefaultDelete_Private::TArrayPtrConstraint<T, U> = 0>
	constexpr FORCEINLINE TDefaultDelete(const TDefaultDelete<U[]>&) {}

	template <typename U, CCDefaultDelete_Private::TArrayPtrConstraint<T, U> = 0>
	constexpr FORCEINLINE void operator()(U* Ptr) const
	{
		delete[] Ptr;
	}
};
