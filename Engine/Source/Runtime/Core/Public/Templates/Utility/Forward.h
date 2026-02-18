// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "CoreDefines.h"
#include "Templates/TypeTraits/RemoveReference.h"

template <typename T>
FORCEINLINE T&& Forward(typename TRemoveReference<T>::Type& In) noexcept
{
	return static_cast<T&&>(In);
}
template <typename T>
FORCEINLINE T&& Forward(typename TRemoveReference<T>::Type&& In) noexcept
{
	return static_cast<T&&>(In);
}
