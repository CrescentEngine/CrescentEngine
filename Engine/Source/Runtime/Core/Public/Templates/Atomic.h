// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "HAL/PlatformAtomics.h"

#include "Templates/TypeTraits/SelectSignedInt.h"
#include "Templates/TypeTraits/IsArithmetic.h"
#include "Templates/TypeTraits/IsIntegral.h"
#include "Templates/TypeTraits/IsPointer.h"

enum class EMemoryOrder
{
	// Chaotic order, any operation might be done at any time no matter time of invocation.
	Relaxed,

	// Operations happen sequentially as in their invocation order by time.
	SequentiallyConsistent
};

template <typename T>
inline constexpr bool TIsAtomicType_v = (sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8);

// Abstraction over FPlatformAtomics using a template type that is can be treated as a fundamental integral type underneath by its size.
template <typename T, TEnableIf_t<TIsAtomicType_v<T>, int> = 0>
struct TAtomicUtility
{
	typedef TSelectSignedInt_t<sizeof(T)> IntType;
	
	static FORCEINLINE T Load_Relaxed(const volatile T* Dest)
	{
		return (T) FPlatformAtomics::AtomicRead_Relaxed((const volatile IntType*) Dest);
	}
	static FORCEINLINE T Load(const volatile T* Dest)
	{
		return (T) FPlatformAtomics::AtomicRead((const volatile IntType*) Dest);
	}

	static FORCEINLINE void Store_Relaxed(volatile T* Dest, T Value)
	{
		FPlatformAtomics::AtomicStore((volatile IntType*) Dest, (IntType) Value);
	}
	static FORCEINLINE void Store(volatile T* Dest, T Value)
	{
		FPlatformAtomics::AtomicStore_Relaxed((volatile IntType*) Dest, (IntType) Value);
	}

	static FORCEINLINE T Exchange(volatile T* Dest, T Value)
	{
		return (T) FPlatformAtomics::InterlockedExchange((volatile IntType*) Dest, (IntType) Value);
	}
	static FORCEINLINE T CompareExchange(volatile T* Dest, T Exchange, T Comperand)
	{
		return (T) FPlatformAtomics::InterlockedCompareExchange((volatile IntType*) Dest, (IntType) Exchange, (IntType) Comperand);
	}

	static FORCEINLINE T Increment(volatile T* Dest)
	{
		return (T) FPlatformAtomics::InterlockedIncrement((volatile T*) Dest);
	}
	static FORCEINLINE T Decrement(volatile T* Dest)
	{
		return (T) FPlatformAtomics::InterlockedDecrement((volatile T*) Dest);
	}
	static FORCEINLINE T Add(volatile T* Dest, T Amount)
	{
		return (T) FPlatformAtomics::InterlockedAdd((volatile T*) Dest, (T) Amount);
	}
	static FORCEINLINE T Sub(volatile T* Dest, T Amount)
	{
		return (T) FPlatformAtomics::InterlockedAdd((volatile T*) Dest, -(T)(Amount));
	}
};

namespace CCAtomic_Private
{
	// Atomic supporting base operations Store and Load only.
	template <typename T>
	struct alignas(TIsAtomicType_v<T> ? alignof(TSelectSignedInt_t<sizeof(T)>) : alignof(T)) TFundamentalAtomic
	{
	public:
		typedef                T  ValueType;
		typedef TAtomicUtility<T> Utility;
	public:
		FORCEINLINE void Store(T Value, EMemoryOrder MemoryOrder = EMemoryOrder::SequentiallyConsistent)
		{
			switch (MemoryOrder)
			{
			case EMemoryOrder::Relaxed:
			{
				return Utility::Store_Relaxed(&Storage, Value);
			}
			case EMemoryOrder::SequentiallyConsistent:
			{
				return Utility::Store(&Storage, Value);
			}
			}
			// unreachable()
		}

		FORCEINLINE T Load(EMemoryOrder MemoryOrder = EMemoryOrder::SequentiallyConsistent) const
		{
			switch (MemoryOrder)
			{
			case EMemoryOrder::Relaxed:
			{
				return Utility::Load_Relaxed(&Storage);
			}
			case EMemoryOrder::SequentiallyConsistent:
			{
				return Utility::Load(&Storage);
			}
			}
			// unreachable()
			return *(T*)0;
		}

		FORCEINLINE T Exchange(T Value)
		{
			return Utility::Exchange(&Storage, Value);
		}

		FORCEINLINE bool CompareExchange(T& Expected, T Desired)
		{
			T Prev = Utility::CompareExchange(&Storage, Expected, Desired);
			const bool bResult = Prev == Expected;
			Expected = Prev;
			return bResult;
		}
	protected:
		TFundamentalAtomic() = default;
		FORCEINLINE TFundamentalAtomic(T Value)
			: Storage(Value)
		{
		}
		volatile T Storage;
	};

	// Atomic implemented via mutexes. Used for large types and those who do not conform TIsAtomicType_v<T>.
	// Based on signatures of TFundamentalAtomic however is a separate implementation.
	template <typename T>
	struct TMutexAtomic
	{
		// TODO: Implement when we have FMutex
	};

	// Atomic for arithmetic types. Allows atomic arithmetic like add/subtract.
	// Based on TFundamentalAtomic.
	template <typename T, typename DiffType = T>
	struct TArithmeticAtomic : public TFundamentalAtomic<T>
	{
	public:
		typedef TAtomicUtility<T> Utility;
		typedef DiffType          DifferenceType;

		FORCEINLINE DiffType operator+=(DiffType Amount)
		{
			return Utility::Add(&this->Storage, Amount) + Amount;
		}
		
		FORCEINLINE DiffType operator-=(DiffType Amount)
		{
			return Utility::Sub(&this->Storage, Amount) - Amount;
		}

		FORCEINLINE T operator++()
		{
			return Utility::Increment(&this->Storage) + 1;
		}
		
		FORCEINLINE T operator++(int)
		{
			return Utility::Increment(&this->Storage);
		}

		FORCEINLINE T operator--()
		{
			return Utility::Decrement(&this->Storage) - 1;
		}

		FORCEINLINE T operator--(int)
		{
			return Utility::Decrement(&this->Storage);
		}
	protected:
		TArithmeticAtomic() = default;
		FORCEINLINE TArithmeticAtomic(T Value)
			: TFundamentalAtomic<T>(Value)
		{
		}
	};

	// Atomic for integral types. Allows bitwise operations.
	// Based on TArithmeticAtomic.
	template <typename T>
	struct TIntegralAtomic : public TArithmeticAtomic<T>
	{
	public:
		// Add bitwise functions when FPlatformAtomics API receives them
	protected:
		TIntegralAtomic() = default;
		FORCEINLINE TIntegralAtomic(T Value)
			: TArithmeticAtomic<T>(Value)
		{
		}
	};

	// Atomic for pointer types. Uses proper difference type for operations.
	// Based on TArithmeticAtomic.
	template <typename T>
	struct TPointerAtomic : public TArithmeticAtomic<T, PTRINT>
	{
		// The reason this type exists is to provide PTRINT\
		// No special behavior needed
	protected:
		TPointerAtomic() = default;
		FORCEINLINE TPointerAtomic(T Value)
			: TArithmeticAtomic<T, PTRINT>(Value)
		{
		}
	};

	template
	<
		typename T,
		bool bIsAtomicType  = TIsAtomicType_v<T>,
		bool bIsArithmetic  = TIsArithmetic_v<T>,
		bool bIsIntegral    = TIsIntegral_v<T>,
		bool bIsPointerType = TIsPointer_v<T>
	>
	struct TSelectAtomicType
	{
	};

	template <typename T>
	struct TSelectAtomicType<T, false, false, false, false>
	{
		typedef TMutexAtomic<T> Type;
	};

	template <typename T>
	struct TSelectAtomicType<T, true, false, false, false>
	{
		typedef TFundamentalAtomic<T> Type;
	};

	template <typename T>
	struct TSelectAtomicType<T, true, true, false, false>
	{
		typedef TArithmeticAtomic<T> Type;
	};

	template <typename T>
	struct TSelectAtomicType<T, true, false, false, true>
	{
		typedef TPointerAtomic<T> Type;
	};

	template <typename T>
	struct TSelectAtomicType<T, true, true, true, false>
	{
		typedef TIntegralAtomic<T> Type;
	};

	template <typename T>
	using TSelectAtomicType_t = typename TSelectAtomicType<T>::Type;
}

template <typename T>
struct TAtomic final : public CCAtomic_Private::TSelectAtomicType_t<T>
{
	TAtomic(const TAtomic&) = delete;
	TAtomic(TAtomic&&) = delete;
	TAtomic& operator=(const TAtomic&) = delete;
	TAtomic& operator=(TAtomic&&) = delete;

	TAtomic() = default;

	FORCEINLINE TAtomic(T Value)
		: CCAtomic_Private::TSelectAtomicType_t<T>(Value)
	{
	}

	FORCEINLINE T operator=(T Value)
	{
		this->Exchange(Value);
		return *this;
	}

	FORCEINLINE operator T() const
	{
		return this->Load();
	}
};
