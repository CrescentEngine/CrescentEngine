// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "Templates/TypeTraits/RemoveExtent.h"
#include "Templates/TypeTraits/Conditional.h"
#include "Templates/TypeTraits/IsArray.h"

#include "Templates/Utility/Move.h"

// Our TAtomic isn't mature enough.
// #include "Templates/Atomic.h"
#include <atomic>

namespace CCSharedControlBlock_Private
{
	template <typename T>
	class TSharedControlBlockBase
	{
	public:
		typedef TRemoveExtent_t<T> ElementType;
		static constexpr bool bIsArrayType = TIsArray_v<T>;
	
		virtual void DestroyObject()
		{
			// Since the allocation policy of shared pointers are transferred and constant, we can do it like this.
			if constexpr (bIsArrayType)
			{
				delete[] Ptr;
			}
			else
			{
				delete Ptr;
			}
		}
		/*virtual*/ FORCEINLINE void DestroySelf() // No longer virtual since we dropped allocator-aware shared pointer support.
		{
			delete this;
		}

		FORCEINLINE ElementType* Get() const { return Ptr; }
	protected:
		template <typename U>
		FORCEINLINE TSharedControlBlockBase(U* InPtr)
			: Ptr(InPtr)
		{
		}
	private:
		ElementType* Ptr = nullptr; // Use ElementType which removes extent from potential T[] so we don't form T[]*, only T*
	};

	template <typename T>
	class TSharedControlBlockThreadUnsafe : public TSharedControlBlockBase<T>
	{
	public:
		template <typename U>
		FORCEINLINE TSharedControlBlockThreadUnsafe(U* InPtr, uint_t InStrongCount, uint_t InWeakCount)
			: TSharedControlBlockBase<T>(InPtr), StrongCount(InStrongCount), WeakCount(InWeakCount)
		{
		}

		FORCEINLINE void AcquireStrong()
		{
			StrongCount++;
		}

		FORCEINLINE bool TryAcquireStrong()
		{
			if (StrongCount)
			{
				StrongCount++;
				return true;
			}
			return false;
		}

		FORCEINLINE void AcquireWeak()
		{
			WeakCount++;
		}

		FORCEINLINE void ReleaseStrong()
		{
			if (!(--StrongCount))
			{
				this->DestroyObject();

				if (!(--WeakCount))
				{
					this->DestroySelf();
				}
			}
		}

		FORCEINLINE void ReleaseWeak()
		{
			if (!(--WeakCount))
			{
				this->DestroySelf();
			}
		}

		FORCEINLINE uint_t PreciseGetStrongCount()   const { return StrongCount; }
		FORCEINLINE uint_t PreciseGetWeakCount()     const { return WeakCount;   }
		
		FORCEINLINE uint_t ImpreciseGetStrongCount() const { return StrongCount; }
		FORCEINLINE uint_t ImpreciseGetWeakCount()   const { return WeakCount;   }
	private:
		uint_t StrongCount, WeakCount;
	};

	template <typename T>
	class TSharedControlBlockThreadSafe : public TSharedControlBlockBase<T>
	{
	public:
		template <typename U>
		FORCEINLINE TSharedControlBlockThreadSafe(U* InPtr, uint_t InStrongCount, uint_t InWeakCount)
			: TSharedControlBlockBase<T>(InPtr), StrongCount{InStrongCount}, WeakCount{InWeakCount}
		{
		}

		FORCEINLINE void AcquireStrong()
		{
			StrongCount.fetch_add(1, std::memory_order_relaxed);
		}

		FORCEINLINE bool TryAcquireStrong()
		{
			uint_t N = StrongCount.load(std::memory_order_acquire);

			while (N)
			{
				if (StrongCount.compare_exchange_weak(N, N + 1, std::memory_order_acq_rel, std::memory_order_acquire))
				{
					return true;
				}
			}

			return false;
		}

		FORCEINLINE void AcquireWeak()
		{
			WeakCount.fetch_add(1, std::memory_order_relaxed);
		}

		FORCEINLINE void ReleaseStrong()
		{
			if (StrongCount.fetch_sub(1, std::memory_order_acq_rel) == 1)
			{
				this->DestroyObject();

				if (WeakCount.fetch_sub(1, std::memory_order_acq_rel) == 1)
				{
					this->DestroySelf();
				}
			}
		}

		FORCEINLINE void ReleaseWeak()
		{
			if (WeakCount.fetch_sub(1, std::memory_order_acq_rel) == 1)
			{
				this->DestroySelf();
			}
		}

		FORCEINLINE uint_t PreciseGetStrongCount()   const { return StrongCount.load(std::memory_order_acquire); }
		FORCEINLINE uint_t PreciseGetWeakCount()     const { return WeakCount.load(std::memory_order_acquire);   }
		
		FORCEINLINE uint_t ImpreciseGetStrongCount() const { return StrongCount.load(std::memory_order_relaxed); }
		FORCEINLINE uint_t ImpreciseGetWeakCount()   const { return WeakCount.load(std::memory_order_relaxed);   }
	private:
		std::atomic<uint_t> StrongCount, WeakCount;
	};

	template <typename T, bool bThreadSafe>
	using TSelectSharedControlBlockByThreadSafety = TConditional_t<bThreadSafe, TSharedControlBlockThreadSafe<T>, TSharedControlBlockThreadUnsafe<T>>;
}

template <typename T, bool bThreadSafe>
class TSharedControlBlock : public CCSharedControlBlock_Private::TSelectSharedControlBlockByThreadSafety<T, bThreadSafe>
{
public:
	static constexpr bool bIsThreadSafe = bThreadSafe;

	template <typename U>
	FORCEINLINE TSharedControlBlock(U* Ptr = nullptr)
		// StrongCount=1(sharedptr that inits this block), WeakCount=1(control block implicitly counted as inherent weak reference)
		: CCSharedControlBlock_Private::TSelectSharedControlBlockByThreadSafety<T, bThreadSafe>(Ptr, 1, 1)
	{
	}
};

template <typename T, bool bThreadSafe>
class TSharedControlBlockForPlacementNewArray : public TSharedControlBlock<T, bThreadSafe>
{
public:
	typedef TRemoveExtent_t<T> ElementType;

	template <typename U>
	FORCEINLINE TSharedControlBlockForPlacementNewArray(U* Ptr, uint_t InN)
		: TSharedControlBlock<T, bThreadSafe>(Ptr), N(InN)
	{
	}

	virtual void DestroyObject() override
	{
		// Don't waste time looping if element type is trivially destructible.
		if constexpr (!TIsTriviallyDestructible_v<ElementType>)
		{
			for (uint_t i = 0; i < N; i++)
			{
				this->Get()[i].~ElementType();
			}
		}
		::operator delete(this->Get());
	}
private:
	uint_t N;
};

template <typename T, typename TDeleter, bool bThreadSafe>
class TSharedControlBlockWithDeleter : public TSharedControlBlock<T, bThreadSafe>
{
public:
	typedef TDeleter DeleterType;
	TDeleter Deleter;

	FORCEINLINE TSharedControlBlockWithDeleter(TDeleter InDeleter)
		: TSharedControlBlock<T, bThreadSafe>(), Deleter(Move(InDeleter))
	{
	}	
	template <typename U>
	FORCEINLINE TSharedControlBlockWithDeleter(U* InPtr, TDeleter InDeleter)
		: TSharedControlBlock<T, bThreadSafe>(InPtr), Deleter(Move(InDeleter))
	{
	}

	virtual void DestroyObject() override
	{
		Deleter(this->Get());
	}
};
