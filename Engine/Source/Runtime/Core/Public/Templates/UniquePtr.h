// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "Templates/Memory/DefaultDelete.h"

#include "Templates/TypeTraits/RemoveReference.h"
#include "Templates/TypeTraits/RemovePointer.h"
#include "Templates/TypeTraits/RemoveExtent.h"
#include "Templates/TypeTraits/AddReference.h"

#include "Templates/TypeTraits/IsConstructible.h"
#include "Templates/TypeTraits/IsAssignable.h"
#include "Templates/TypeTraits/IsReference.h"
#include "Templates/TypeTraits/IsPointer.h"
#include "Templates/TypeTraits/IsArray.h"
#include "Templates/TypeTraits/IsSame.h"
#include "Templates/TypeTraits/Void.h"

// For TEfficientPair, TUniquePtr uses that to support EBO in case of stateless Deleter types
#include "Templates/Utility/EfficientPair.h"

#include "Templates/Utility/Forward.h"
#include "Templates/Utility/Move.h"
#include "Templates/Utility/Swap.h"

namespace CCUniquePtr_Private
{
	template <typename Deleter>
	using TDeleterConstraint = TEnableIf_t<!TIsPointer_v<Deleter> && TIsDefaultConstructible_v<Deleter>, int>;

	template <typename Deleter>
	using TDeleterCopyConstraint = TEnableIf_t<TIsConstructible_v<Deleter, const Deleter&>, int>;

	template <typename Deleter>
	using TDeleterMoveConstraint = TEnableIf_t<!TIsReference_v<Deleter> && TIsConstructible_v<Deleter, Deleter>, int>;

	template <typename Deleter>
	using TDeleterCannotMoveConstraint = TEnableIf_t<TIsReference_v<Deleter> && TIsConstructible_v<Deleter, TRemoveReference_t<Deleter>>, int>;

	template <typename U, typename ElementType, typename PointerType>
	using TArrayPtrConstraint = TEnableIf_t<TIsSame_v<U, PointerType> || TIsSame_v<U, TypeOfNullptr> ||
								(TIsSame_v<PointerType, ElementType*> && TIsConvertible_v<TRemovePointer_t<U>(*)[], ElementType(*)[]>), int>;

	template <typename T, typename Del, typename = void>
	struct TPointerType
	{
		typedef T* Type;
	};

	template <typename T, typename Del>
	struct TPointerType<T, Del, TVoid<typename TRemoveReference_t<Del>::PointerType>> // If type doesn't exist in TVoid<>, it's a substitution error
																				  // which makes it fall back to the primary template and use T*
	{
		typedef	typename TRemoveReference_t<Del>::PointerType Type;
	};
}

template <typename T, typename TDeleter = TDefaultDelete<T>>
class TUniquePtr
{
	template <typename OtherT, typename OtherDeleter>
	friend class TUniquePtr;
public:
	typedef     T    ElementType;
	typedef TDeleter DeleterType;

	typedef typename CCUniquePtr_Private::TPointerType<T, TDeleter>::Type PointerType;
public:
	// TUniquePtr cannot be copy constructed
	TUniquePtr(const TUniquePtr&) = delete;
	// TUniquePtr cannot be copy assigned
	TUniquePtr& operator=(const TUniquePtr&) = delete;

	template <typename Del = TDeleter, CCUniquePtr_Private::TDeleterConstraint<Del> = 0>
	constexpr FORCEINLINE TUniquePtr()
		: Pair()
	{
	}

	template <typename Del = TDeleter, CCUniquePtr_Private::TDeleterConstraint<Del> = 0>
	constexpr FORCEINLINE TUniquePtr(TypeOfNullptr)
		: Pair()
	{
	}

	template <typename Del = TDeleter, CCUniquePtr_Private::TDeleterConstraint<Del> = 0>
	explicit constexpr FORCEINLINE TUniquePtr(PointerType InPtr)
		: Pair(EFFICIENT_PAIR_CTOR_1ST, InPtr)
	{
	}

	template <typename Del = TDeleter, CCUniquePtr_Private::TDeleterCopyConstraint<Del> = 0>
	constexpr FORCEINLINE TUniquePtr(PointerType InPtr, const TDeleter& InDeleter)
		: Pair(InPtr, InDeleter)
	{
	}

	template <typename Del = TDeleter, CCUniquePtr_Private::TDeleterMoveConstraint<Del> = 0>
	constexpr FORCEINLINE TUniquePtr(PointerType InPtr, TDeleter&& InDeleter)
		: Pair(InPtr, Move(InDeleter))
	{
	}

	template <typename Del = TDeleter, CCUniquePtr_Private::TDeleterCannotMoveConstraint<Del> = 0>
	TUniquePtr(PointerType, TDeleter&&) = delete;

	template <typename Del = TDeleter, TEnableIf_t<TIsMoveConstructible_v<Del>, int> = 0>
	constexpr FORCEINLINE TUniquePtr(TUniquePtr&& Other)
		: Pair(Other.Release(), Move(Other.GetDeleter()))
	{
	}

	template <typename OtherT, typename OtherDeleter,
		TEnableIf_t<!TIsArray_v<OtherT>&& TIsConvertible_v<typename TUniquePtr<OtherT, OtherDeleter>::PointerType, PointerType> &&
		(TIsReference_v<TDeleter> ? TIsSame_v<OtherDeleter, TDeleter> : TIsConvertible_v<OtherDeleter, TDeleter>), int> = 0>
	constexpr FORCEINLINE TUniquePtr(TUniquePtr<OtherT, OtherDeleter>&& Other)
		: Pair(Other.Release(), Forward<OtherDeleter>(Other.GetDeleter()))
	{
	}

	constexpr FORCEINLINE ~TUniquePtr()
	{
		if (Get())
		{
			GetDeleter()(Get());
		}
	}

	constexpr FORCEINLINE TUniquePtr& operator=(TypeOfNullptr)
	{
		Reset();
		return *this;
	}

	template <typename Del = TDeleter, TEnableIf_t<TIsMoveConstructible_v<Del>, int> = 0>
	constexpr FORCEINLINE TUniquePtr& operator=(TUniquePtr&& Other)
	{
		Reset(Other.Release());
		Pair.Second() = Move(Other.GetDeleter());
		return *this;
	}

	template <typename OtherT, typename OtherDeleter,
			  TEnableIf_t<TIsAssignable_v<TDeleter&, OtherDeleter&&> && !TIsArray_v<OtherT> &&
						  TIsConvertible_v<typename TUniquePtr<OtherT, OtherDeleter>::PointerType, PointerType>, int> = 0>
	constexpr FORCEINLINE TUniquePtr& operator=(TUniquePtr<OtherT, OtherDeleter>&& Other)
	{
		Reset(Other.Release());
		Pair.Second() = Forward<OtherDeleter>(Other.GetDeleter());
		return *this;
	}

	constexpr FORCEINLINE PointerType Release()
	{
		PointerType Prev = Get();
		Pair.First() = nullptr;
		return Prev;
	}

	constexpr FORCEINLINE void Reset(PointerType InPtr = PointerType())
	{
		if (Get() != InPtr)
		{
			PointerType Prev = Get();
			Pair.First() = InPtr;

			if (Prev)
			{
				GetDeleter()(Prev);
			}
		}
	}

	constexpr FORCEINLINE void Swap(TUniquePtr& Other)
	{
		Swap(Pair, Other.Pair);
	}

	constexpr FORCEINLINE TAddLValueReference_t<T> operator*() const { return *Get(); }
	constexpr FORCEINLINE PointerType operator->() const { return Get(); }

	explicit constexpr FORCEINLINE operator bool() const { return IsValid(); }
	constexpr FORCEINLINE bool IsValid() const { return Get() != nullptr; }

	constexpr FORCEINLINE PointerType Get() const { return Pair.First(); }
	constexpr FORCEINLINE const TDeleter& GetDeleter() const { return Pair.Second(); }
	constexpr FORCEINLINE TDeleter& GetDeleter() { return Pair.Second(); }
private:
	TEfficientPair<PointerType, DeleterType> Pair;
};

template <typename T, typename TDeleter>
class TUniquePtr<T[], TDeleter>
{
	template <typename OtherT, typename OtherDeleter>
	friend class TUniquePtr;
public:
	typedef     T    ElementType;
	typedef TDeleter DeleterType;

	typedef typename CCUniquePtr_Private::TPointerType<T, TDeleter>::Type PointerType;
public:
	// TUniquePtr cannot be copy constructed
	TUniquePtr(const TUniquePtr&) = delete;
	// TUniquePtr cannot be copy assigned
	TUniquePtr& operator=(const TUniquePtr&) = delete;

	template <typename Del = TDeleter, CCUniquePtr_Private::TDeleterConstraint<Del> = 0>
	constexpr FORCEINLINE TUniquePtr()
		: Pair()
	{
	}

	template <typename Del = TDeleter, CCUniquePtr_Private::TDeleterConstraint<Del> = 0>
	constexpr FORCEINLINE TUniquePtr(TypeOfNullptr)
		: Pair()
	{
	}

	template <typename U, typename Del = TDeleter, CCUniquePtr_Private::TArrayPtrConstraint<U, ElementType, PointerType> = 0,
			  CCUniquePtr_Private::TDeleterConstraint<Del> = 0>
	explicit constexpr FORCEINLINE TUniquePtr(U InPtr)
		: Pair(EFFICIENT_PAIR_CTOR_1ST, InPtr)
	{
	}

	template <typename U, typename Del = TDeleter, CCUniquePtr_Private::TArrayPtrConstraint<U, ElementType, PointerType> = 0,
			  CCUniquePtr_Private::TDeleterCopyConstraint<Del> = 0>
	constexpr FORCEINLINE TUniquePtr(U InPtr, const TDeleter& InDeleter)
		: Pair(InPtr, InDeleter)
	{
	}
	
	template <typename U, typename Del = TDeleter, CCUniquePtr_Private::TArrayPtrConstraint<U, ElementType, PointerType> = 0,
			  CCUniquePtr_Private::TDeleterMoveConstraint<Del> = 0>
	constexpr FORCEINLINE TUniquePtr(U InPtr, TDeleter&& InDeleter)
		: Pair(InPtr, Move(InDeleter))
	{
	}

	template <typename U, typename Del = TDeleter, CCUniquePtr_Private::TArrayPtrConstraint<U, ElementType, PointerType> = 0,
			  CCUniquePtr_Private::TDeleterCannotMoveConstraint<Del> = 0>
	TUniquePtr(PointerType InPtr, TDeleter&& InDeleter) = delete;

	template <typename Del = TDeleter, TEnableIf_t<TIsMoveConstructible_v<Del>, int> = 0>
	constexpr FORCEINLINE TUniquePtr(TUniquePtr&& Other)
		: Pair(Other.Release(), Move(Other.GetDeleter()))
	{
	}

	template <typename OtherT, typename OtherDeleter,
		TEnableIf_t<TIsArray_v<OtherT> && TIsSame_v<PointerType, ElementType*> &&
		TIsSame_v<typename TUniquePtr<OtherT, OtherDeleter>::PointerType, typename TUniquePtr<OtherT, OtherDeleter>::ElementType*> &&
		TIsConvertible_v<typename TUniquePtr<OtherT, OtherDeleter>::ElementType(*)[], ElementType(*)[]> &&
		(TIsReference_v<TDeleter> ? TIsSame_v<OtherDeleter, TDeleter> : TIsConvertible_v<OtherDeleter, TDeleter>), int> = 0> // Big fucking boy holy overload prerequisite hell
	constexpr FORCEINLINE TUniquePtr(TUniquePtr<OtherT, OtherDeleter>&& Other)
		: Pair(Other.Release(), Forward<OtherDeleter>(Other.GetDeleter()))
	{
	}

	constexpr FORCEINLINE ~TUniquePtr()
	{
		if (Get())
		{
			GetDeleter()(Get());
		}
	}

	constexpr FORCEINLINE PointerType Release()
	{
		PointerType Prev = Get();
		Pair.First() = nullptr;
		return Prev;
	}

	template <typename U, TEnableIf_t<TIsSame_v<U, PointerType> || (TIsSame_v<PointerType, ElementType*> &&
		TIsPointer_v<U> && TIsConvertible_v<TRemovePointer_t<U>(*)[], ElementType(*)[]>), int> = 0>
	constexpr FORCEINLINE void Reset(U InPtr)
	{
		if (Get() != InPtr)
		{
			PointerType Prev = Get();
			Pair.First() = InPtr;

			if (Prev)
			{
				GetDeleter()(Prev);
			}
		}
	}

	constexpr FORCEINLINE void Reset(TypeOfNullptr = nullptr)
	{
		Reset(PointerType());
	}

	constexpr FORCEINLINE void Swap(TUniquePtr& Other)
	{
		Swap(Pair, Other.Pair);
	}

	constexpr FORCEINLINE T& operator[](uint_t Index) const
	{
		return Get()[Index];
	}

	explicit constexpr FORCEINLINE operator bool() const { return IsValid(); }
	constexpr FORCEINLINE bool IsValid() const { return Get() != nullptr; }

	constexpr FORCEINLINE PointerType Get() const { return Pair.First(); }
	constexpr FORCEINLINE const TDeleter& GetDeleter() const { return Pair.Second(); }
	constexpr FORCEINLINE TDeleter& GetDeleter() { return Pair.Second(); }
private:
	TEfficientPair<PointerType, DeleterType> Pair;
};

template <typename T, typename Deleter>
constexpr FORCEINLINE void Swap(TUniquePtr<T, Deleter>& LHS, TUniquePtr<T, Deleter>& RHS)
{
	LHS.Swap(RHS);
}

template <typename T, typename... Args, TEnableIf_t<!TIsArray_v<T>, int> = 0>
constexpr FORCEINLINE TUniquePtr<T> MakeUnique(Args&&... InArgs)
{
	return TUniquePtr<T>(new T(Forward<Args>(InArgs)...));
}

template <typename T, TEnableIf_t<TIsUnboundedArray_v<T>, int> = 0>
constexpr FORCEINLINE TUniquePtr<T> MakeUnique(uint_t N)
{
	return TUniquePtr<T>(new TRemoveExtent_t<T>[N]()); // default constructed N elements
}

template <typename T, typename... Args, TEnableIf_t<TIsBoundedArray_v<T>, int> = 0>
constexpr FORCEINLINE void MakeUnique(Args&&...) = delete; // explicitly deleted per standard

template <typename T, TEnableIf_t<!TIsArray_v<T>, int> = 0>
constexpr FORCEINLINE TUniquePtr<T> MakeUniqueForOverwrite()
{
	return TUniquePtr<T>(new T); // default initialized (not explicitly constructed)
}

template <typename T, TEnableIf_t<TIsUnboundedArray_v<T>, int> = 0>
constexpr FORCEINLINE TUniquePtr<T> MakeUniqueForOverwrite(uint_t N)
{
	return TUniquePtr<T>(new TRemoveExtent_t<T>[N]); // default initialized N elements
}

template <typename T, typename... Args, TEnableIf_t<TIsBoundedArray_v<T>, int> = 0>
constexpr FORCEINLINE void MakeUniqueForOverwrite(Args&&...) = delete; // explicitly deleted per standard

template <typename T1, typename D1, typename T2, typename D2>
constexpr FORCEINLINE bool operator==(const TUniquePtr<T1, D1>& LHS, const TUniquePtr<T2, D2>& RHS)
{
	return LHS.Get() == RHS.Get();
}
template <typename T1, typename D1, typename T2, typename D2>
constexpr FORCEINLINE bool operator!=(const TUniquePtr<T1, D1>& LHS, const TUniquePtr<T2, D2>& RHS)
{
	return LHS.Get() != RHS.Get();
}
template <typename T1, typename D1, typename T2, typename D2>
constexpr FORCEINLINE bool operator<(const TUniquePtr<T1, D1>& LHS, const TUniquePtr<T2, D2>& RHS)
{
	return LHS.Get() < RHS.Get();
}
template <typename T1, typename D1, typename T2, typename D2>
constexpr FORCEINLINE bool operator<=(const TUniquePtr<T1, D1>& LHS, const TUniquePtr<T2, D2>& RHS)
{
	return !(RHS < LHS);
}
template <typename T1, typename D1, typename T2, typename D2>
constexpr FORCEINLINE bool operator>(const TUniquePtr<T1, D1>& LHS, const TUniquePtr<T2, D2>& RHS)
{
	return RHS < LHS;
}
template <typename T1, typename D1, typename T2, typename D2>
constexpr FORCEINLINE bool operator>=(const TUniquePtr<T1, D1>& LHS, const TUniquePtr<T2, D2>& RHS)
{
	return !(LHS < RHS);
}
template <typename T, typename D>
constexpr FORCEINLINE bool operator==(const TUniquePtr<T, D>& Ptr, TypeOfNullptr)
{
	return !Ptr.IsValid();
}
template <typename T, typename D>
constexpr FORCEINLINE bool operator==(TypeOfNullptr, const TUniquePtr<T, D>& Ptr)
{
	return !Ptr.IsValid();
}
template <typename T, typename D>
constexpr FORCEINLINE bool operator!=(const TUniquePtr<T, D>& Ptr, TypeOfNullptr)
{
	return Ptr.IsValid();
}
template <typename T, typename D>
constexpr FORCEINLINE bool operator!=(TypeOfNullptr, const TUniquePtr<T, D>& Ptr)
{
	return Ptr.IsValid();
}
template <typename T, typename D>
constexpr FORCEINLINE bool operator<(const TUniquePtr<T, D>& Ptr, TypeOfNullptr)
{
	return Ptr.Get() < nullptr;
}
template <typename T, typename D>
constexpr FORCEINLINE bool operator<(TypeOfNullptr, const TUniquePtr<T, D>& Ptr)
{
	return nullptr < Ptr.Get();
}
template <typename T, typename D>
constexpr FORCEINLINE bool operator<=(const TUniquePtr<T, D>& Ptr, TypeOfNullptr)
{
	return !(nullptr < Ptr);
}
template <typename T, typename D>
constexpr FORCEINLINE bool operator<=(TypeOfNullptr, const TUniquePtr<T, D>& Ptr)
{
	return !(Ptr < nullptr);
}
template <typename T, typename D>
constexpr FORCEINLINE bool operator>(const TUniquePtr<T, D>& Ptr, TypeOfNullptr)
{
	return nullptr < Ptr;
}
template <typename T, typename D>
constexpr FORCEINLINE bool operator>(TypeOfNullptr, const TUniquePtr<T, D>& Ptr)
{
	return Ptr < nullptr;
}
template <typename T, typename D>
constexpr FORCEINLINE bool operator>=(const TUniquePtr<T, D>& Ptr, TypeOfNullptr)
{
	return !(Ptr < nullptr);
}
template <typename T, typename D>
constexpr FORCEINLINE bool operator>=(TypeOfNullptr, const TUniquePtr<T, D>& Ptr)
{
	return !(nullptr < Ptr);
}
