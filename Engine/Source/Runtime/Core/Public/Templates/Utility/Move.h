// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "CoreDefines.h"
#include "Templates/TypeTraits/RemoveReference.h"

template <typename T>
FORCEINLINE typename TRemoveReference<T>::Type&& Move(T&& In)
{
	return static_cast<typename TRemoveReference<T>::Type&&>(In);
}
