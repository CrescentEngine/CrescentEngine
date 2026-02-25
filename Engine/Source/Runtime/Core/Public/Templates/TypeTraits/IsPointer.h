// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Templates/TypeTraits/RemoveConstVolatile.h"
#include "Templates/TypeTraits/IntegralConstant.h"

template <typename T>
struct TIsConstVolatileUnqualifiedPointer : FFalseType
{
};
template <typename T>
struct TIsConstVolatileUnqualifiedPointer<T*> : FTrueType
{
};

template <typename T>
struct TIsPointer : TIsConstVolatileUnqualifiedPointer<typename TRemoveConstVolatile<T>::Type>
{
};

template <typename T>
inline constexpr bool TIsPointer_v = TIsPointer<T>::Value;
