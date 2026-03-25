// Copyright (C) 2026 ychgen, all rights reserved.

#pragma once

#include "Templates/TypeTraits/IntegralConstant.h"
#include "Templates/TypeTraits/Conditional.h"

template <typename...>
struct TConjunction : FTrueType
{
};

template <typename _First>
struct TConjunction<_First> : _First
{
};

template <typename _First, typename... _Rest>
struct TConjunction<_First, _Rest...> : TConditional_t<bool(_First::Value), TConjunction<_Rest...>, _First>
{
};

template <typename... Conditionals>
inline constexpr bool TConjunction_v = TConjunction<Conditionals...>::Value;
