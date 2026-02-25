// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "CoreTypes.h"
#include "HAL/Memory.h"

// For std::nothrow_t
#include <new>

#define OVERRIDES_NEW_DELETE_OPERATORS_ROUTE_TO_CORE_ALLOC                                                       \
	void* operator new     (uint_t N                                   )          { return FMemory::Malloc(N); } \
	void* operator new[]   (uint_t N                                   )          { return FMemory::Malloc(N); } \
	void* operator new     (uint_t N,   const std::nothrow_t&          ) noexcept { return FMemory::Malloc(N); } \
	void* operator new[]   (uint_t N,   const std::nothrow_t&          ) noexcept { return FMemory::Malloc(N); } \
	void  operator delete  (void*  Ptr                                 ) noexcept { return FMemory::Free(Ptr); } \
	void  operator delete[](void*  Ptr                                 ) noexcept { return FMemory::Free(Ptr); } \
	void  operator delete  (void*  Ptr, uint_t N                       ) noexcept { return FMemory::Free(Ptr); } \
	void  operator delete[](void*  Ptr, uint_t N                       ) noexcept { return FMemory::Free(Ptr); } \
	void  operator delete  (void*  Ptr, const std::nothrow_t&          ) noexcept { return FMemory::Free(Ptr); } \
	void  operator delete[](void*  Ptr, const std::nothrow_t&          ) noexcept { return FMemory::Free(Ptr); } \
	void  operator delete  (void*  Ptr, uint_t N, const std::nothrow_t&) noexcept { return FMemory::Free(Ptr); } \
	void  operator delete[](void*  Ptr, uint_t N, const std::nothrow_t&) noexcept { return FMemory::Free(Ptr); }
