// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "CoreDefines.h"

#include "HAL/Platform.h"

#include "Templates/TypeTraits/EnableIf.h"
#include "Templates/TypeTraits/IsConstructible.h"
#include "Templates/TypeTraits/IsConvertible.h"
#include "Templates/TypeTraits/IsAssignable.h"
#include "Templates/TypeTraits/IsEmpty.h"
#include "Templates/TypeTraits/IsFinal.h"

#include "Templates/TypeTraits/Void.h"

#include "Templates/Utility/Forward.h"
#include "Templates/Utility/Move.h"
#include "Templates/Utility/Swap.h"

namespace CCEfficientPair_Private
{
	template <typename T>
	inline constexpr bool TNeedsEBO = TIsEmpty_v<T> && !TIsFinal_v<T>;

	// Order is a pseudo-tag that will ensure in case of TEfficientPair<T, T> we don't inherit from the same type since both
	// would evaluate to TTEfficientPairElement<T, SameBool>, TTEfficientPairElement<T, SameBool> and we'd inherit from the same
	// type twice which is not allowed.
	template <typename T, uint_t Order, bool bEmpty = TNeedsEBO<T>>
	struct TEfficientPairElement;

	template <typename T, uint_t Order>
	struct TEfficientPairElement<T, Order, false>
	{
		T Value;

		template <typename TX = T, TEnableIf_t<TIsDefaultConstructible_v<TX>, int> = 0>
		explicit(TIsImplicitlyDefaultConstructible_v<T>) constexpr TEfficientPairElement()
			: Value()
		{
		}

		template <typename TX = T, TEnableIf_t<TIsCopyConstructible_v<T>, int> = 0>
		explicit(!TIsConvertible_v<const T&, T>) constexpr FORCEINLINE TEfficientPairElement(const T& InValue)
			: Value(InValue)
		{
		}

		template <typename U = T, TEnableIf_t<TIsConstructible_v<T, U>, int> = 0>
		explicit(!TIsConvertible_v<U, T>) constexpr FORCEINLINE TEfficientPairElement(T&& InValue)
			: Value(Forward<U>(InValue))
		{
		}

		constexpr TEfficientPairElement(const TEfficientPairElement&) = default;
		constexpr TEfficientPairElement(TEfficientPairElement&&) = default;

		constexpr FORCEINLINE const T& Get() const { return Value; }
		constexpr FORCEINLINE T& Get() { return Value; }
	};

	template <typename T, uint_t Order>
	struct TEfficientPairElement<T, Order, true> : private T
	{
		template <typename TX = T, TEnableIf_t<TIsDefaultConstructible_v<TX>, int> = 0>
		explicit(!TIsImplicitlyDefaultConstructible_v<T>) constexpr TEfficientPairElement()
			: T()
		{
		}

		template <typename TX = T, TEnableIf_t<TIsCopyConstructible_v<T>, int> = 0>
		explicit(!TIsConvertible_v<const T&, T>) constexpr FORCEINLINE TEfficientPairElement(const T& InValue)
			: T(InValue)
		{
		}

		template <typename U = T, TEnableIf_t<TIsConstructible_v<T, U>, int> = 0>
		explicit(!TIsConvertible_v<U, T>) constexpr FORCEINLINE TEfficientPairElement(T&& InValue)
			: T(Forward<U>(InValue))
		{
		}

		constexpr TEfficientPairElement(const TEfficientPairElement&) = default;
		constexpr TEfficientPairElement(TEfficientPairElement&&) = default;

		constexpr FORCEINLINE const T& Get() const { return *this; }
		constexpr FORCEINLINE T& Get() { return *this; }
	};


	struct FOnlyConstructT1 { constexpr explicit FOnlyConstructT1() = default; };
	struct FOnlyConstructT2 { constexpr explicit FOnlyConstructT2() = default; };
}

inline constexpr CCEfficientPair_Private::FOnlyConstructT1 EFFICIENT_PAIR_CTOR_1ST{};
inline constexpr CCEfficientPair_Private::FOnlyConstructT2 EFFICIENT_PAIR_CTOR_2ND{};

template <typename T1, typename T2>
struct TEfficientPair : private CCEfficientPair_Private::TEfficientPairElement<T1, 0>,
						private CCEfficientPair_Private::TEfficientPairElement<T2, 1>
{
	typedef CCEfficientPair_Private::TEfficientPairElement<T1, 0> Base1;
	typedef CCEfficientPair_Private::TEfficientPairElement<T2, 1> Base2;

	typedef T1 FirstType;
	typedef T2 SecondType;

	template <typename = void, typename = TEnableIf_t<TIsDefaultConstructible_v<T1> && TIsDefaultConstructible_v<T2>>>
	explicit(!TIsImplicitlyDefaultConstructible_v<T1> || !TIsImplicitlyDefaultConstructible_v<T2>) constexpr TEfficientPair()
		: Base1(),
		  Base2()
	{
	}

	template <typename = void, typename = TEnableIf_t<TIsCopyConstructible_v<T1> && TIsCopyConstructible_v<T2>>>
	explicit(!TIsConvertible_v<const T1&, T1> || !TIsConvertible_v<const T2&, T2>)
		constexpr FORCEINLINE TEfficientPair(const T1& InFirst, const T2& InSecond)
		: Base1(InFirst),
		  Base2(InSecond) {}

	template <typename U1 = T1, typename U2 = T2,
		TEnableIf_t<TIsConstructible_v<T1, U1> && TIsConstructible_v<T2, U2>, int> = 0>
	explicit(!TIsConvertible_v<U1, T1> || !TIsConvertible_v<U2, T2>)
		constexpr FORCEINLINE TEfficientPair(U1&& InFirst, U2&& InSecond)
		: Base1(Forward<U1>(InFirst)),
		  Base2(Forward<U2>(InSecond)) {}

	template <typename = void, typename = TEnableIf_t<TIsCopyConstructible_v<T1> && TIsDefaultConstructible_v<T2>>>
	explicit(!TIsConvertible_v<const T1&, T1> || !TIsImplicitlyDefaultConstructible_v<T2>)
		constexpr FORCEINLINE TEfficientPair(CCEfficientPair_Private::FOnlyConstructT1, const T1& InFirst)
		: Base1(InFirst),
		  Base2() {}

	template <typename U1 = T1, TEnableIf_t<TIsConstructible_v<T1, U1> && TIsDefaultConstructible_v<T2>, int> = 0>
	explicit(!TIsConvertible_v<U1, T1> || !TIsImplicitlyDefaultConstructible_v<T2>)
		constexpr FORCEINLINE TEfficientPair(CCEfficientPair_Private::FOnlyConstructT1, U1&& InFirst)
		: Base1(Forward<U1>(InFirst)),
		  Base2() {}

	template <typename = void, typename = TEnableIf_t<TIsDefaultConstructible_v<T1> && TIsCopyConstructible_v<T2>>>
	explicit(!TIsImplicitlyDefaultConstructible_v<T1> || !TIsConvertible_v<const T2&, T2>)
		constexpr FORCEINLINE TEfficientPair(CCEfficientPair_Private::FOnlyConstructT2, const T2& InSecond)
		: Base1(),
		  Base2(InSecond) {}

	template <typename U2 = T2, TEnableIf_t<TIsDefaultConstructible_v<T1> && TIsConstructible_v<T2, U2>, int> = 0>
	explicit(!TIsImplicitlyDefaultConstructible_v<T1> || !TIsConvertible_v<U2, T2>)
		constexpr FORCEINLINE TEfficientPair(CCEfficientPair_Private::FOnlyConstructT2, U2&& InSecond)
		: Base1(),
		  Base2(Forward<U2>(InSecond)) {}

	template <typename U1, typename U2,
		TEnableIf_t<TIsConstructible_v<T1, U1&> && TIsConstructible_v<T2, U2&>, int> = 0>
	explicit(!TIsConvertible_v<U1&, T1> || !TIsConvertible_v<U2&, T2>)
		constexpr FORCEINLINE TEfficientPair(TEfficientPair<U1, U2>& Other)
		: Base1(Other.First()),
		  Base2(Other.Second()) {}

	template <typename U1, typename U2,
		TEnableIf_t<TIsConstructible_v<T1, const U1&> && TIsConstructible_v<T2, const U2&>, int> = 0>
	explicit(!TIsConvertible_v<const U1&, T1> || !TIsConvertible_v<const U2&, T2>)
		constexpr FORCEINLINE TEfficientPair(const TEfficientPair<U1, U2>& Other)
		: Base1(Other.First()),
		  Base2(Other.Second()) {}

	template <typename U1, typename U2,
		TEnableIf_t<TIsConstructible_v<T1, U1> && TIsConstructible_v<T2, U2>, int> = 0>
	explicit(!TIsConvertible_v<U1, T1> || !TIsConvertible_v<U2, T2>)
		constexpr FORCEINLINE TEfficientPair(TEfficientPair<U1, U2>&& Other)
		: Base1(Forward<U1>(Other.First())),
		  Base2(Forward<U2>(Other.Second())) {}

	constexpr TEfficientPair(const TEfficientPair&) = default;
	constexpr TEfficientPair(TEfficientPair&&) = default;

	template <typename = void, typename = TEnableIf_t<TIsCopyAssignable_v<T1> && TIsCopyAssignable_v<T2>>>
	constexpr TEfficientPair& operator=(const TEfficientPair& Other)
	{
		First()  = Other.First();
		Second() = Other.Second();
		return *this;
	}

	template <typename U1, typename U2, TEnableIf_t<TIsAssignable_v<T1&, const U1&> && TIsAssignable_v<T2&, const U2&>, int> = 0>
	constexpr FORCEINLINE TEfficientPair& operator=(const TEfficientPair<U1, U2>& Other)
	{
		First()  = Other.First();
		Second() = Other.Second();
		return *this;
	}

	template <typename = void, typename = TEnableIf_t<TIsMoveAssignable_v<T1> && TIsMoveAssignable_v<T2>>>
	constexpr FORCEINLINE TEfficientPair& operator=(TEfficientPair&& Other)
	{
		First()  = Move(Other.First());
		Second() = Move(Other.Second());
		return *this;
	}

	template <typename U1, typename U2, TEnableIf_t<TIsAssignable_v<T1&, U1> && TIsAssignable_v<T2&, U2>, int> = 0>
	constexpr FORCEINLINE TEfficientPair& operator=(TEfficientPair<U1, U2>&& Other)
	{
		First()  = Forward<U1>(Other.First());
		Second() = Forward<U2>(Other.Second());
		return *this;
	}

	constexpr FORCEINLINE void Swap(TEfficientPair& Other)
	{
		::Swap(First(), Other.First());
		::Swap(Second(), Other.Second());
	}

	constexpr FORCEINLINE const T1& First() const { return Base1::Get(); }
	constexpr FORCEINLINE T1& First() { return Base1::Get(); }

	constexpr FORCEINLINE const T2& Second() const { return Base2::Get(); }
	constexpr FORCEINLINE T2& Second() { return Base2::Get(); }
};

template <typename T1, typename T2>
constexpr FORCEINLINE void Swap(TEfficientPair<T1, T2>& LHS, TEfficientPair<T1, T2>& RHS)
{
	LHS.Swap(RHS);
}

template <typename T1, typename T2, typename U1, typename U2>
constexpr FORCEINLINE bool operator==(const TEfficientPair<T1, T2>& LHS, const TEfficientPair<U1, U2>& RHS)
{
	return LHS.First() == RHS.First() && LHS.Second() == RHS.Second();
}

template <typename T1, typename T2, typename U1, typename U2>
constexpr FORCEINLINE bool operator!=(const TEfficientPair<T1, T2>& LHS, const TEfficientPair<U1, U2>& RHS)
{
	return !(LHS == RHS);
}

template <typename T1, typename T2, typename U1, typename U2>
constexpr FORCEINLINE bool operator<(const TEfficientPair<T1, T2>& LHS, const TEfficientPair<U1, U2>& RHS)
{
	return LHS.First() < RHS.First() || (!(RHS.First() < LHS.First()) && LHS.Second() < RHS.Second());
}

template <typename T1, typename T2, typename U1, typename U2>
constexpr FORCEINLINE bool operator<=(const TEfficientPair<T1, T2>& LHS, const TEfficientPair<U1, U2>& RHS)
{
	return !(RHS < LHS);
}

template <typename T1, typename T2, typename U1, typename U2>
constexpr FORCEINLINE bool operator>(const TEfficientPair<T1, T2>& LHS, const TEfficientPair<U1, U2>& RHS)
{
	return RHS < LHS;
}

template <typename T1, typename T2, typename U1, typename U2>
constexpr FORCEINLINE bool operator>=(const TEfficientPair<T1, T2>& LHS, const TEfficientPair<U1, U2>& RHS)
{
	return !(LHS < RHS);
}
