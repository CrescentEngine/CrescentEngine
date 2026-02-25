// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "CoreDefines.h"
#include "Templates/TypeTraits/RemoveReference.h"

template <typename T>
FORCEINLINE TRemoveReference_t<T>&& Move(T&& In) noexcept
{
	return static_cast<TRemoveReference_t<T>&&>(In);
}
