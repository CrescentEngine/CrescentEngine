// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "Templates/TypeTraits/AddReference.h"

template <typename T>
TAddRValueReference_t<T> Declval() noexcept
{
	static_assert(false, "Declval can only be present in unevaluated contexts.");
}
