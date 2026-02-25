// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Templates/TypeTraits/TypeIdentity.h"

namespace CCAddReference_Private
{
	template <typename T>
	auto AddLValueReference(int) -> TTypeIdentity<T&>;
	template <typename T>
	auto AddLValueReference(...) -> TTypeIdentity<T>;

	template <typename T>
	auto AddRValueReference(int) -> TTypeIdentity<T&&>;
	template <typename T>
	auto AddRValueReference(...) -> TTypeIdentity<T>;
}

template <typename T>
struct TAddLValueReference : decltype(CCAddReference_Private::AddLValueReference<T>(0))
{
};

template <typename T>
struct TAddRValueReference : decltype(CCAddReference_Private::AddRValueReference<T>(0))
{
};

template <typename T>
using TAddLValueReference_t = typename TAddLValueReference<T>::Type;

template <typename T>
using TAddRValueReference_t = typename TAddRValueReference<T>::Type;
