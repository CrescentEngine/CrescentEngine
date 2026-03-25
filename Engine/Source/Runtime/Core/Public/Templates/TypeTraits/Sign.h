// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "Templates/TypeTraits/IsIntegral.h"
#include "Templates/TypeTraits/EnableIf.h"

enum ESign
{
	SIGN_SIGNED        = 0,
	SIGN_UNSIGNED      = 1,
	SIGN_EXTRAORDINARY = 2  // Type can neither be signed or unsigned
};

template <typename T, typename = void>
struct TSign
{
	static constexpr ESign Value = SIGN_EXTRAORDINARY;
};

template <typename T>
struct TSign<T, TEnableIf_t<TIsIntegral_v<T>>>
{
	// This depends on ESign:: SIGNED=0 and UNSIGNED=1
	static constexpr ESign Value = (ESign)(T(0) < T(-1));
};

template <typename T>
inline constexpr ESign TSign_v = TSign<T>::Value;
