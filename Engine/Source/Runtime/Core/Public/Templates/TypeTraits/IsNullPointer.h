// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "CoreTypes.h"

#include "Templates/TypeTraits/RemoveConstVolatile.h"
#include "Templates/TypeTraits/IntegralConstant.h"
#include "Templates/TypeTraits/IsSame.h"

template <typename T>
struct TIsNullPointer : TIsSame<TypeOfNullptr, typename TRemoveConstVolatile<T>::Type>
{
};

template <typename T>
inline constexpr bool TIsNullPointer_v = TIsNullPointer<T>::Value;
