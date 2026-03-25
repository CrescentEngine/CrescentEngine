// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "CoreTypes.h"

#include "Templates/TypeTraits/IntegralConstant.h"
#include "Templates/TypeTraits/Sign.h"

#include "Templates/TypeTraits/EnableIf.h"

template <typename From, typename To, typename = void>
struct TIsIntegralSafeConvertible : FFalseType
{
};

// For two integral types to be safely convertible, their signs must be the same and type to convert to must be larger than or equal to type From, preventing narrowing.
template <typename From, typename To>
struct TIsIntegralSafeConvertible<From, To, TEnableIf_t<TIsIntegral_v<From> && TIsIntegral_v<To>>> : TBoolConstant
<
	TSign_v<To> == TSign_v<From> && // SIGN check
	sizeof (To) >= sizeof (From)    // SIZE check
>
{
};

template <typename From, typename To>
inline constexpr bool TIsIntegralSafeConvertible_v = TIsIntegralSafeConvertible<From, To>::Value;
