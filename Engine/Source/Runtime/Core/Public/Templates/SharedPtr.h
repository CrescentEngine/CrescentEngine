// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "Templates/TypeTraits/IsDestructible.h"
#include "Templates/TypeTraits/IsConvertible.h"
#include "Templates/TypeTraits/IsReference.h"
#include "Templates/TypeTraits/IsCallable.h"
#include "Templates/TypeTraits/IsArray.h"
#include "Templates/TypeTraits/IsSame.h"
#include "Templates/TypeTraits/IsVoid.h"

#include "Templates/TypeTraits/RemoveConstVolatile.h"
#include "Templates/TypeTraits/CompositePointer.h"
#include "Templates/TypeTraits/RemoveExtent.h"
#include "Templates/TypeTraits/Conjunction.h"
#include "Templates/TypeTraits/EnableIf.h"

// For operator<
#include "Templates/Functional/Less.h"

// Header containing control blocks.
#include "Templates/Memory/SharedControlBlock.h"

template <typename T, bool bThreadSafe = false>
class TSharedPtr;

namespace CCSharedPtr_Private
{
	template <typename U, typename = void>
	struct TIsValidExpr_Unbounded : FFalseType {};
	template <typename U>
	struct TIsValidExpr_Unbounded<U, TVoid<U(*)[]>> : FTrueType
	{
	};
	
	template <typename U, uint_t N, typename = void>
	struct TIsValidExpr_Bounded : FFalseType {};
	template <typename U, uint_t N>
	struct TIsValidExpr_Bounded<U, N, TVoid<U(*)[N]>> : FTrueType
	{
	};

	template <typename U, typename T>
	struct TIsPointerFor : TIsConvertible<U*, T*>::Type
	{
	};

	template <typename U, typename T>
	struct TIsPointerFor<U, T[]> : TConjunction<TIsValidExpr_Unbounded<U>, TIsConvertible<U(*)[], T(*)[]>> ::Type
	{
	};

	template <typename U, typename T, uint_t N>
	struct TIsPointerFor<U, T[N]> : TConjunction<TIsValidExpr_Bounded<U, N>, TIsConvertible<U(*)[N], T(*)[N]>> ::Type
	{
	};

	// ptr compatibility check in accordance to util.smartptr.shared.const of the C++ standard/specification .
	// furthermore, cppreference elaborates in a simpler way:
	//   For the purposes of the description below, a pointer type Y* is said to be compatible with a pointer type T* if either Y* is convertible
	//   to T* or Y is the array type U[N] and T is U cv [] (where cv is some set of cv-qualifiers). 

	template <typename U, typename T>
	struct TIsPointerCompatible : TIsConvertible<U*, T*>::Type
	{
	};

	template <typename U, uint_t N>
	struct TIsPointerCompatible<U[N], U[]> : FTrueType
	{
	};

	template <typename U, uint_t N>
	struct TIsPointerCompatible<U[N], const U[]> : FTrueType
	{
	};

	template <typename U, uint_t N>
	struct TIsPointerCompatible<U[N], volatile U[]> : FTrueType
	{
	};

	template <typename U, uint_t N>
	struct TIsPointerCompatible<U[N], const volatile U[]> : FTrueType
	{
	};

	template <typename T>
	struct TBoundSize
	{
	};
	template <typename T, uint_t N>
	struct TBoundSize<T[N]> : TIntegralConstant<uint_t, N>
	{
	};
	template <typename T>
	inline constexpr uint_t TBoundSize_v = TBoundSize<T>::Value;

	template <typename T, bool bThreadSafe>
	struct TSelectControlBlockTypeForEfficientCreateShared { using Type = TSharedControlBlock<T, bThreadSafe>; };
	template <typename T, bool bThreadSafe>
	struct TSelectControlBlockTypeForEfficientCreateShared<T[], bThreadSafe> { using Type = TSharedControlBlockForPlacementNewArray<T[], bThreadSafe>; };
	template <typename T, bool bThreadSafe, uint_t N>
	struct TSelectControlBlockTypeForEfficientCreateShared<T[N], bThreadSafe> { using Type = TSharedControlBlockForPlacementNewArray<T[N], bThreadSafe>; };

	// Creates a shared pointer with one allocation instead of two that traditionally happens by using ptr-allowing ctor of shared pointer.
	// This version allocates space enough for object(s) and the control block with one allocation.
	template <typename T, bool bThreadSafe, bool bNoDefaultInit, typename... Args>
	FORCEINLINE TSharedPtr<T, bThreadSafe> EfficientCreateShared(uint_t N, Args&&... InArgs)
	{
		using ElementType = TRemoveExtent_t<T>;
		using ControlBlockType = typename TSelectControlBlockTypeForEfficientCreateShared<T, bThreadSafe>::Type;

		const uint_t AlignOfObject = alignof(ElementType);
		const uint_t AlignOfRefCnt = alignof(ControlBlockType);
		const uint_t MaxAlign      = AlignOfObject < AlignOfRefCnt ? AlignOfRefCnt : AlignOfObject;

		const uint_t SizeOfObjects = sizeof(ElementType) * N;
		const uint_t SizeOfRefCnt  = sizeof(ControlBlockType);
		const uint_t OffsetOfRefCnt = (SizeOfObjects + AlignOfRefCnt - 1) / AlignOfRefCnt * AlignOfRefCnt;

		const uint_t AllocationSize = SizeOfObjects + SizeOfRefCnt + AlignOfRefCnt;
		uint8* Buffer = (uint8*)::operator new(AllocationSize);

		ElementType* Objects = (ElementType*) Buffer;
		for (uint_t i = 0; i < N; i++)
		{
			if constexpr (bNoDefaultInit)
			{
				new ((void*) &Objects[i]) ElementType;
			}
			else
			{
				new ((void*) &Objects[i]) ElementType(Forward<Args>(InArgs)...);
			}
		}

		void* PtrRefCnt = (void*)(Buffer + OffsetOfRefCnt);
		if constexpr (TBound_v<T> == ETypeBound::None) // For non-array, TSharedControlBlock is chosen (ctor only takes ptr)
		{
			new (PtrRefCnt) ControlBlockType(Objects);
		}
		else // For array, TSharedControlBlockForPlacementNewArray is chosen (ctor takes ptr + array num)
		{
			new (PtrRefCnt) ControlBlockType(Objects, N);
		}

		// Private ctor `TSharedPtr(U*, TControlBlock*)` available to EfficientCreateShared because it is marked as friend.
		return TSharedPtr<T, bThreadSafe>(Objects, (ControlBlockType*) PtrRefCnt);
	}

	template <typename T> auto       HasWellFormedNormalDelete(int) -> decltype(delete Declval<T*>(), FTrueType{});
	template <typename>   FFalseType HasWellFormedNormalDelete(...);
	
	// T must be array type
	template <typename T> auto       HasWellFormedScalarDelete(int) -> decltype(delete[] Declval<TRemoveExtent_t<T>*>(), FTrueType{});
	template <typename>   FFalseType HasWellFormedScalarDelete(...);

	template <typename T>
	struct TIsWellFormedDelete : decltype(HasWellFormedNormalDelete<T>(0))
	{
	};

	template <typename T>
	struct TIsWellFormedDelete<T[]> : decltype(HasWellFormedScalarDelete<T>(0))
	{
	};
}

template <typename T, typename Deleter>
class TUniquePtr;

template <typename T, bool bThreadSafe>
class TWeakPtr;

template <typename T, bool bThreadSafe>
class TSharedPtr
{
public:
	typedef TRemoveExtent_t<T>       ElementType;
	typedef TWeakPtr<T, bThreadSafe> WeakType;

	static constexpr bool bIsThreadSafe = bThreadSafe;
private: // Private section is supposed to be at lower section, but since it contains constraints used in public section, we put it high up.
	template <typename OtherT, bool bOtherThreadSafe>
	friend class TSharedPtr;

	template <typename OtherT, bool bOtherThreadSafe>
	friend class TWeakPtr;

	// Mark ::CCSharedPtr_Private::EfficientCreateShared as friend so it can access constructor `TSharedPtr(U*, TSharedControlBlock*)`
	template <typename OtherT, bool bOtherThreadSafe, bool bNoDefaultInit, typename... Args>
	friend TSharedPtr<OtherT, bOtherThreadSafe> CCSharedPtr_Private::EfficientCreateShared(uint_t, Args&&...);

	using BaseControlBlock          = TSharedControlBlock<T, bThreadSafe>;
	template <typename Deleter>
	using WithDeleterControlBlock   = TSharedControlBlockWithDeleter<T, Deleter, bThreadSafe>;

	template <typename U>
	using TPtrConstraint = TEnableIf_t<CCSharedPtr_Private::TIsPointerFor<U, T>::Value, int>;

	template <typename U>
	using TPtrCompatConstraint = TEnableIf_t<CCSharedPtr_Private::TIsPointerCompatible<U, T>::Value, int>;

	// Checks whether or not object of type Deleter can be invoked as DeleterObject(TRemoveExtent_t<U>*)
	template <typename Deleter, typename U>
	using TDelConstraint = TEnableIf_t<TIsCallable_v<Deleter, TRemoveExtent_t<U>*>, int>;
public:
	// Default ctor
	FORCEINLINE TSharedPtr()
		: ControlBlock(nullptr)
	{
	}
	// Nullptr ctor
	FORCEINLINE TSharedPtr(TypeOfNullptr)
		: TSharedPtr()
	{
	}

	// Ptr ctor
	template <typename U, TPtrConstraint<U> = 0, typename = TEnableIf_t<CCSharedPtr_Private::TIsWellFormedDelete<T>::Value>>
	explicit FORCEINLINE TSharedPtr(U* InPtr)
		: Ptr(InPtr), ControlBlock(new BaseControlBlock(InPtr))
	{
	}

	// Ptr+Del ctor
	template <typename U, typename Deleter, TPtrConstraint<U> = 0, TDelConstraint<Deleter, U> = 0>
	FORCEINLINE TSharedPtr(U* InPtr, Deleter Del)
		: Ptr(InPtr), ControlBlock(new WithDeleterControlBlock<Deleter>(InPtr, Move(Del)))
	{
	}
	// Nullptr+Del ctor
	template <typename Deleter, TDelConstraint<Deleter, ElementType> = 0>
	FORCEINLINE TSharedPtr(TypeOfNullptr, Deleter Del)
		: ControlBlock(new WithDeleterControlBlock<Deleter>(nullptr, Move(Del)))
	{
	}
	// Aliasing constructor. Participates in Other's object's lifetime, but always stores InPtr.
	// It is the caller's responsibility to manage InPtr. Sharedptr will always use InPtr.
	// If this Sharedptr is last to go out of scope, the original object will be destroyed, not InPtr,
	// since this sharedptr still participates in Other's lifetime.
	template <typename U>
	FORCEINLINE TSharedPtr(const TSharedPtr<U, bThreadSafe>& Other, ElementType* InPtr)
		: Ptr(InPtr), ControlBlock(Other.ControlBlock)
	{
		if (ControlBlock)
		{
			ControlBlock->AcquireStrong(); // We now participate in Other's lifetime
		}
	}
	// Aliasing constructor. Participates in Other's object's lifetime, but always stores InPtr.
	// It is the caller's responsibility to manage InPtr. Sharedptr will always use InPtr.
	// If this Sharedptr is last to go out of scope, the original object will be destroyed, not InPtr,
	// since this sharedptr still participates in Other's lifetime.
	template <typename U>
	FORCEINLINE TSharedPtr(TSharedPtr<U, bThreadSafe>&& Other, ElementType* InPtr)
		: Ptr(InPtr), ControlBlock(Other.ControlBlock)
	{
		Other.MemberwiseReset();
	}

	// Copy ctor
	FORCEINLINE TSharedPtr(const TSharedPtr& Other)
		: Ptr(Other.Ptr), ControlBlock(Other.ControlBlock)
	{
		if (ControlBlock)
		{
			ControlBlock->AcquireStrong();
		}
	}

	// Copy ctor of `compatible` ptr type (see CCSharedPtr_Private::TIsPointerCompatible)
	template <typename U, TPtrCompatConstraint<U> = 0>
	FORCEINLINE TSharedPtr(const TSharedPtr<U, bThreadSafe>& Other)
		: Ptr(Other.Ptr), ControlBlock(Other.ControlBlock)
	{
		if (ControlBlock)
		{
			ControlBlock->AcquireStrong();
		}
	}

	// Move ctor
	FORCEINLINE TSharedPtr(TSharedPtr&& Other)
		: Ptr(Other.Ptr), ControlBlock(Other.ControlBlock)
	{
		Other.MemberwiseReset();
	}

	// Move ctor of `compatible` ptr type (see CCSharedPtr_Private::TIsPointerCompatible)
	template <typename U, TPtrCompatConstraint<U> = 0>
	FORCEINLINE TSharedPtr(TSharedPtr<U, bThreadSafe>&& Other)
		: Ptr(Other.Ptr), ControlBlock(Other.ControlBlock)
	{
		Other.MemberwiseReset();
	}

	// Ctor from weakptr
	// Different behavior from C++ standard: if Weak has expired, overload acts same as default ctor.
	template <typename U, TPtrCompatConstraint<U> = 0>
	explicit FORCEINLINE TSharedPtr(const TWeakPtr<U, bThreadSafe>& Weak)
	{
		if (Weak.ControlBlock && Weak.ControlBlock->TryAcquireStrong())
		{
			ControlBlock = Weak.ControlBlock;
			Ptr = ControlBlock->Ptr;
		}
	}

	template <typename U, typename Deleter, TPtrConstraint<U> = 0, TDelConstraint<Deleter, U> = 0,
		TEnableIf_t<TConjunction_v<CCSharedPtr_Private::TIsPointerCompatible<U, T>, TIsConvertible<typename TUniquePtr<U, Deleter>::PointerType, ElementType*>>, int> = 0>
	FORCEINLINE TSharedPtr(TUniquePtr<U, Deleter>&& Unique)
	{
		// Standard specifies; if Unique.Get() is a null pointer, this overload is equivalent to the default ctor.
		// Default ctor does nothing (leaves Ptr & ControlBlock as nullptr).
		// So we only do stuff if Unique.Get() != nullptr;
		if (Unique.Get())
		{
			Ptr = Unique.Release();

			if (Ptr)
			{
				// TODO(?): Reévaluate for reference types, C++ standard says std::ref for ref del types. However, we just Move them too.
				ControlBlock = new WithDeleterControlBlock<U, Deleter>(Ptr, Move(Unique.GetDeleter()));
			}
		}
	}

	FORCEINLINE ~TSharedPtr()
	{
		if (ControlBlock)
		{
			ControlBlock->ReleaseStrong();
		}
	}

	FORCEINLINE TSharedPtr& operator=(const TSharedPtr& Other)
	{
		TSharedPtr(Other).Swap(*this);
		return *this;
	}

	template <typename U, TPtrCompatConstraint<U> = 0>
	FORCEINLINE TSharedPtr& operator=(const TSharedPtr<U, bThreadSafe>& Other)
	{
		TSharedPtr<U, bThreadSafe>(Other).Swap(*this);
		return *this;
	}

	FORCEINLINE TSharedPtr& operator=(TSharedPtr&& Other)
	{
		TSharedPtr(Move(Other)).Swap(*this);
		return *this;
	}

	template <typename U, TPtrCompatConstraint<U> = 0>
	FORCEINLINE TSharedPtr& operator=(TSharedPtr<U, bThreadSafe>&& Other)
	{
		TSharedPtr<U, bThreadSafe>(Move(Other)).Swap(*this);
		return *this;
	}

	template <typename U, typename Deleter, TDelConstraint<Deleter, U> = 0,
		TEnableIf_t<TConjunction_v<CCSharedPtr_Private::TIsPointerCompatible<U, T>, TIsConvertible<typename TUniquePtr<U, Deleter>::PointerType, ElementType*>>, int> = 0>
	FORCEINLINE TSharedPtr& operator=(TUniquePtr<U, Deleter>&& Unique)
	{
		if (ControlBlock)
		{
			ControlBlock->ReleaseStrong();
		}
		if (Unique.Get())
		{
			Ptr = Unique.Release();

			if (Ptr)
			{
				// TODO(?): Check UniquePtr ctor for TODO(?) comment.
				ControlBlock = new WithDeleterControlBlock<U, Deleter>(Ptr, Move(Unique.GetDeleter()));
			}
		}
		else
		{
			MemberwiseReset();
		}
		return *this;
	}

	FORCEINLINE void Reset()
	{
		TSharedPtr().Swap(*this);
	}

	template <typename U, TPtrConstraint<U> = 0>
	FORCEINLINE void Reset(U* InPtr)
	{
		TSharedPtr(InPtr).Swap(*this);
	}

	template <typename U, typename Deleter, TDelConstraint<Deleter, U> = 0>
	FORCEINLINE void Reset(U* InPtr, Deleter Del)
	{
		TSharedPtr(InPtr, Move(Del)).Swap(*this);
	}

	FORCEINLINE void Swap(TSharedPtr& Other)
	{
		::Swap(     Ptr    ,      Other.Ptr    );
		::Swap(ControlBlock, Other.ControlBlock);
	}

	// Provides ordering support for shared pointers, useful for use with certain containers.
	template <typename U>
	FORCEINLINE bool IsOwnerBefore(const TSharedPtr<U, bThreadSafe>& Other) const
	{
		return ControlBlock < Other.ControlBlock;
	}
	template <typename U>
	FORCEINLINE bool IsOwnerBefore(const TWeakPtr<U, bThreadSafe>& Other) const
	{
		return ControlBlock < Other.ControlBlock;
	}

	/**
	 * @brief This method makes it possible to access a shared pointer's deleter if one was provided through the
	 * Deleter allowing constructor or was constructed from another that had a deleter.
	 * This method requires RTTI to be enabled to function. If not, it will always return null pointer.
	 * 
	 * @return The stored deleter if it exists and is of the given template type Deleter.
	 */
	template <typename Deleter, typename _ExpectedType = WithDeleterControlBlock<Deleter>>
	Deleter* GetDeleter() const
	{
		_ExpectedType* CastControl = dynamic_cast<_ExpectedType*>(ControlBlock);
		return CastControl ? &(CastControl->Deleter) : nullptr;
	}

	// Returns the stored pointer. It may not be the same as the `referenced` pointer. See aliasing constructor for more details.
	FORCEINLINE ElementType* Get() const
	{
		return Ptr;
	}

	// Standard specifies it is unspecified whether operator* is defined or not if:
	//   1. T is an array type
	//   2. T is possibly cv-qualified `void`
	// Our implementation doesn't define operator* under these circumstances.
	template <typename _T = T, TEnableIf_t<!(TIsArray_v<_T> || TIsVoid_v<_T>), int> = 0>
	FORCEINLINE _T& operator*() const
	{
		return *Ptr;
	}

	// Standard specifies it is unspecified whether operator-> is defined or not if:
	//   : T is an array type
	// Our implementation doesn't define operator-> under this circumstance.
	template <typename _T = T, TEnableIf_t<!TIsArray_v<_T>, int> = 0>
	FORCEINLINE _T* operator->() const
	{
		return Ptr;
	}

	// Standard specifies it is unspecified whether operator[] is defined or not if:
	//   : T is not an array type
	// Our implementation doesn't define operator[] under this circumstance.
	template <typename _T = T, TEnableIf_t<TIsArray_v<_T>, int> = 0>
	FORCEINLINE ElementType& operator[](uint_t Idx) const
	{
		// TODO: Debug assert if T is bounded array type and Idx >= ArrayN
		return Ptr[Idx];
	}

	/**
	 * @brief Returns the precise number of shared pointers referencing this object/ptr, inclusive.
	 * Prefer using GetRefCount_Fast whenever possible, as this version is slower with thread unsafe shared ptrs.
	 * Mostly suitable for debugging and tracking purposes.
	 * 
	 * @return Precise number of shared pointers referencing this object/ptr, inclusive.
	 */
	FORCEINLINE uint_t GetRefCount() const
	{
		return ControlBlock ? ControlBlock->PreciseGetStrongCount() : 0;
	}
	
	/**
	 * @brief Returns the approximate number of shared pointers referencing this object/ptr, inclusive.
	 * Never consider the return value of this method as precise if called on a thread safe shared ptr.
	 * 
	 * @return Precise number of shared pointers referencing this object/ptr, inclusive.
	 */
	FORCEINLINE uint_t GetRefCount_Fast() const
	{
		return ControlBlock ? ControlBlock->ImpreciseGetStrongCount() : 0;
	}

	/**
	 * @brief Checks if this shared pointer is the only one that references the managed object/pointer.
	 * This method is slow to call with thread safe shared ptrs.
	 * Mostly suitable for debugging and tracking purposes.
	 * 
	 * @return Whether or not this shared pointer uniquely references the managed object/pointer.
	 */
	FORCEINLINE bool IsUnique() const
	{
		return GetRefCount() == 1;
	}

	FORCEINLINE bool IsValid() const
	{
		return Ptr != nullptr;
	}

	FORCEINLINE explicit operator bool() const
	{
		return IsValid();
	}
private:
	// Used by ::CCSharedPtr_Private::EfficientCreateShared
	template <typename U, TPtrConstraint<U> = 0>
	FORCEINLINE TSharedPtr(U* InPtr, TSharedControlBlock<T, bThreadSafe>* InControlBlock)
		: Ptr(InPtr), ControlBlock(InControlBlock)
	{
	}

	FORCEINLINE void MemberwiseReset()
	{
		Ptr = nullptr;
		ControlBlock = nullptr;
	}
private:
	// In most cases, copy of ControlBlock->Ptr unless the shared pointer was alias-constructed.
	ElementType* Ptr                                  = nullptr;
	// Shared control block between all shared pointers that hold this object.
	// Basically, "the ownership" itself.
	TSharedControlBlock<T, bThreadSafe>* ControlBlock = nullptr;
};

// ADL impl of Swap for TSharedPtr

template <typename T, bool bThreadSafe>
FORCEINLINE void Swap(TSharedPtr<T, bThreadSafe>& LHS, TSharedPtr<T, bThreadSafe>& RHS)
{
	LHS.Swap(RHS);
}

// CreateShared implementations

template <typename T, bool bThreadSafe = false, typename... Args>
FORCEINLINE TEnableIf_t<!TIsArray_v<T>, TSharedPtr<T, bThreadSafe>> CreateShared(Args&&... InArgs)
{
	return CCSharedPtr_Private::EfficientCreateShared<T, bThreadSafe, false>(1, Forward<Args>(InArgs)...);
}

template <typename T, bool bThreadSafe = false, typename = TEnableIf_t<TIsUnboundedArray_v<T>>>
FORCEINLINE TSharedPtr<T, bThreadSafe> CreateShared(uint_t N)
{
	return CCSharedPtr_Private::EfficientCreateShared<T, bThreadSafe, false>(N);
}

template <typename T, bool bThreadSafe = false, typename = TEnableIf_t<TIsBoundedArray_v<T>>>
FORCEINLINE TSharedPtr<T, bThreadSafe> CreateShared()
{
	return CCSharedPtr_Private::EfficientCreateShared<T, bThreadSafe, false>(CCSharedPtr_Private::TBoundSize_v<T>);
}

template <typename T, bool bThreadSafe = false, typename = TEnableIf_t<TIsUnboundedArray_v<T>>>
FORCEINLINE TSharedPtr<T, bThreadSafe> CreateShared(uint_t N, const TRemoveExtent_t<T>& InitFrom)
{
	return CCSharedPtr_Private::EfficientCreateShared<T, bThreadSafe, false>(N, InitFrom);
}

template <typename T, bool bThreadSafe = false, typename = TEnableIf_t<TIsBoundedArray_v<T>>>
FORCEINLINE TSharedPtr<T, bThreadSafe> CreateShared(const TRemoveExtent_t<T>& InitFrom)
{
	return CCSharedPtr_Private::EfficientCreateShared<T, bThreadSafe, false>(CCSharedPtr_Private::TBoundSize_v<T>, InitFrom);
}

template <typename T, bool bThreadSafe = false, typename = TEnableIf_t<TBound_v<T> != ETypeBound::Unbounded>>
FORCEINLINE TSharedPtr<T, bThreadSafe> CreateSharedForOverwrite()
{
	if constexpr (TBound_v<T> == ETypeBound::None) // Non-array
	{
		return CCSharedPtr_Private::EfficientCreateShared<T, bThreadSafe, true>(1);
	}
	else // Bounded-array, guaranteed to be bounded since only array type we accept is bounded (we disallow Unbounded, ::None & ::Bounded are accepted), see SFINAE check above.
	{
		return CCSharedPtr_Private::EfficientCreateShared<T, bThreadSafe, true>(CCSharedPtr_Private::TBoundSize_v<T>);
	}
}

template <typename T, bool bThreadSafe = false, typename = TEnableIf<TIsUnboundedArray_v<T>>>
FORCEINLINE TSharedPtr<T, bThreadSafe> CreateSharedForOverwrite(uint_t N)
{
	return CCSharedPtr_Private::EfficientCreateShared<T, bThreadSafe, true>(N);
}

// StaticPointerCast, DynamicPointerCast, ConstPointerCast and ReinterpretPointerCast implementations

template <typename T, bool bThreadSafe = false, typename U>
FORCEINLINE TSharedPtr<T, bThreadSafe> StaticPointerCast(const TSharedPtr<U, bThreadSafe>& Ptr)
{
	// Aliasing ctor is used
	return TSharedPtr<T, bThreadSafe>(Ptr, static_cast<typename TSharedPtr<T, bThreadSafe>::ElementType*>(Ptr.Get()));
}

template <typename T, bool bThreadSafe = false, typename U>
FORCEINLINE TSharedPtr<T, bThreadSafe> StaticPointerCast(TSharedPtr<U, bThreadSafe>&& Ptr)
{
	// Aliasing ctor is used (move overload exists)
	return TSharedPtr<T, bThreadSafe>(Move(Ptr), static_cast<typename TSharedPtr<T, bThreadSafe>::ElementType*>(Ptr.Get()));
}

template <typename T, bool bThreadSafe = false, typename U>
FORCEINLINE TSharedPtr<T, bThreadSafe> DynamicPointerCast(const TSharedPtr<U, bThreadSafe>& Ptr)
{
	auto Result = dynamic_cast<typename TSharedPtr<T, bThreadSafe>::ElementType*>(Ptr.Get());
	return Result ? TSharedPtr<T, bThreadSafe>(Ptr, Result) : TSharedPtr<T, bThreadSafe>();
}

template <typename T, bool bThreadSafe = false, typename U>
FORCEINLINE TSharedPtr<T, bThreadSafe> DynamicPointerCast(TSharedPtr<U, bThreadSafe>&& Ptr)
{
	auto Result = dynamic_cast<typename TSharedPtr<T, bThreadSafe>::ElementType*>(Ptr.Get());
	return Result ? TSharedPtr<T, bThreadSafe>(Move(Ptr), Result) : TSharedPtr<T, bThreadSafe>();
}

template <typename T, bool bThreadSafe = false, typename U>
FORCEINLINE TSharedPtr<T, bThreadSafe> ConstPointerCast(const TSharedPtr<U, bThreadSafe>& Ptr)
{
	// Aliasing ctor is used
	return TSharedPtr<T, bThreadSafe>(Ptr, const_cast<typename TSharedPtr<T, bThreadSafe>::ElementType*>(Ptr.Get()));
}

template <typename T, bool bThreadSafe = false, typename U>
FORCEINLINE TSharedPtr<T, bThreadSafe> ConstPointerCast(TSharedPtr<U, bThreadSafe>&& Ptr)
{
	// Aliasing ctor is used (move overload exists)
	return TSharedPtr<T, bThreadSafe>(Move(Ptr), const_cast<typename TSharedPtr<T, bThreadSafe>::ElementType*>(Ptr.Get()));
}

template <typename T, bool bThreadSafe = false, typename U>
FORCEINLINE TSharedPtr<T, bThreadSafe> ReinterpretPointerCast(const TSharedPtr<U, bThreadSafe>& Ptr)
{
	// Aliasing ctor is used
	return TSharedPtr<T, bThreadSafe>(Ptr, reinterpret_cast<typename TSharedPtr<T, bThreadSafe>::ElementType*>(Ptr.Get()));
}

template <typename T, bool bThreadSafe = false, typename U>
FORCEINLINE TSharedPtr<T, bThreadSafe> ReinterpretPointerCast(TSharedPtr<U, bThreadSafe>&& Ptr)
{
	// Aliasing ctor is used (move overload exists)
	return TSharedPtr<T, bThreadSafe>(Move(Ptr), reinterpret_cast<typename TSharedPtr<T, bThreadSafe>::ElementType*>(Ptr.Get()));
}

// Comparison operators between two SPs

template <typename T, typename U, bool bThreadSafe>
FORCEINLINE bool operator==(const TSharedPtr<T, bThreadSafe>& LHS, const TSharedPtr<U, bThreadSafe>& RHS)
{
	// Must cast to void* in case SP<T>::ElementType* operator== SP<U>::ElementType* is not well formed
	// Standard specifies no overload or behavioral check and this must resolve nicely each time, so we cast to void*
	// which all pointer types are castable to.
	return static_cast<void*>(LHS.Get()) == static_cast<void*>(RHS.Get());
}

template <typename T, typename U, bool bThreadSafe>
FORCEINLINE bool operator!=(const TSharedPtr<T, bThreadSafe>& LHS, const TSharedPtr<U, bThreadSafe>& RHS)
{
	return !(LHS == RHS);
}

template <typename  T, typename U, bool bThreadSafe>
FORCEINLINE bool operator<(const TSharedPtr<T, bThreadSafe>& LHS, const TSharedPtr<U, bThreadSafe>& RHS)
{
	return TLess<TCompositePointer_t<typename TSharedPtr<T, bThreadSafe>::ElementType, typename TSharedPtr<U, bThreadSafe>::ElementType>>()(LHS.Get(), RHS.Get());
}

template <typename  T, typename U, bool bThreadSafe>
FORCEINLINE bool operator>(const TSharedPtr<T, bThreadSafe>& LHS, const TSharedPtr<U, bThreadSafe>& RHS)
{
	return RHS < LHS;
}

template <typename  T, typename U, bool bThreadSafe>
FORCEINLINE bool operator<=(const TSharedPtr<T, bThreadSafe>& LHS, const TSharedPtr<U, bThreadSafe>& RHS)
{
	return !(RHS < LHS);
}

template <typename  T, typename U, bool bThreadSafe>
FORCEINLINE bool operator>=(const TSharedPtr<T, bThreadSafe>& LHS, const TSharedPtr<U, bThreadSafe>& RHS)
{
	return !(LHS < RHS);
}

// Comparison operators between a SP and Nullptr

template <typename T, bool bThreadSafe>
FORCEINLINE bool operator==(const TSharedPtr<T, bThreadSafe>& Ptr, TypeOfNullptr)
{
	return !Ptr.IsValid();
}

template <typename T, bool bThreadSafe>
FORCEINLINE bool operator==(TypeOfNullptr, const TSharedPtr<T, bThreadSafe>& Ptr)
{
	return !Ptr.IsValid();
}

template <typename T, bool bThreadSafe>
FORCEINLINE bool operator!=(const TSharedPtr<T, bThreadSafe>& Ptr, TypeOfNullptr)
{
	return Ptr.IsValid();
}

template <typename T, bool bThreadSafe>
FORCEINLINE bool operator!=(TypeOfNullptr, const TSharedPtr<T, bThreadSafe>& Ptr)
{
	return Ptr.IsValid();
}

template <typename T, bool bThreadSafe>
FORCEINLINE bool operator<(const TSharedPtr<T, bThreadSafe>& Ptr, TypeOfNullptr)
{
	return TLess<typename TSharedPtr<T, bThreadSafe>::ElementType*>()(Ptr.Get(), nullptr);
}

template <typename T, bool bThreadSafe>
FORCEINLINE bool operator<(TypeOfNullptr, const TSharedPtr<T, bThreadSafe>& Ptr)
{
	return TLess<typename TSharedPtr<T, bThreadSafe>::ElementType*>()(nullptr, Ptr.Get());
}

template <typename T, bool bThreadSafe>
FORCEINLINE bool operator>(const TSharedPtr<T, bThreadSafe>& Ptr, TypeOfNullptr)
{
	return nullptr < Ptr;
}

template <typename T, bool bThreadSafe>
FORCEINLINE bool operator>(TypeOfNullptr, const TSharedPtr<T, bThreadSafe>& Ptr)
{
	return Ptr < nullptr;
}

template <typename T, bool bThreadSafe>
FORCEINLINE bool operator<=(const TSharedPtr<T, bThreadSafe>& Ptr, TypeOfNullptr)
{
	return !(nullptr < Ptr);
}

template <typename T, bool bThreadSafe>
FORCEINLINE bool operator<=(TypeOfNullptr, const TSharedPtr<T, bThreadSafe>& Ptr)
{
	return !(Ptr < nullptr);
}

template <typename T, bool bThreadSafe>
FORCEINLINE bool operator>=(const TSharedPtr<T, bThreadSafe>& Ptr, TypeOfNullptr)
{
	return !(Ptr < nullptr);
}

template <typename T, bool bThreadSafe>
FORCEINLINE bool operator>=(TypeOfNullptr, const TSharedPtr<T, bThreadSafe>& Ptr)
{
	return !(nullptr < Ptr);
}
