// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "HAL/Malloc.h"

class CORE_API FMallocAnsi : public FMalloc
{
public:
	virtual void* Malloc(uint_t N, uint16 Alignment = DEFAULT_ALIGNMENT) override;
	virtual void* Realloc(void* Ptr, uint_t N, uint16 Alignment = DEFAULT_ALIGNMENT) override;
	virtual void  Free(void* Ptr) override;
	virtual bool  GetAllocationSize(const void* Ptr, uint_t& OutSize) override;
};
