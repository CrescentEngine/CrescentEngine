// Copyright (C) 2026 ychgen, all rights reserved.

// Crescent Engine template metaprogramming library
// type traits (roughly equivalent to STL's <type_traits>
// Includes every trait header in 'Templates/TypeTraits'.

#pragma once

#include "Templates/TypeTraits/RemoveConstVolatile.h"
#include "Templates/TypeTraits/RemoveReference.h"
#include "Templates/TypeTraits/RemovePointer.h"
#include "Templates/TypeTraits/RemoveExtent.h"

#include "Templates/TypeTraits/AddConstVolatile.h"
#include "Templates/TypeTraits/AddReference.h"

#include "Templates/TypeTraits/CompositePointer.h"
#include "Templates/TypeTraits/IntegralConstant.h"
#include "Templates/TypeTraits/TypeIdentity.h"
#include "Templates/TypeTraits/Conjunction.h"
#include "Templates/TypeTraits/Conditional.h"
#include "Templates/TypeTraits/Alignment.h"
#include "Templates/TypeTraits/EnumBase.h"
#include "Templates/TypeTraits/EnableIf.h"
#include "Templates/TypeTraits/Void.h"
#include "Templates/TypeTraits/Sign.h"

#include "Templates/TypeTraits/SelectUnsignedInt.h"
#include "Templates/TypeTraits/SelectSignedInt.h"

#include "Templates/TypeTraits/MakeUnsigned.h"
#include "Templates/TypeTraits/MakeSigned.h"

#include "Templates/TypeTraits/IsIntegralSafeConvertible.h"
#include "Templates/TypeTraits/IsEqualityComparable.h"
#include "Templates/TypeTraits/IsConstructible.h"
#include "Templates/TypeTraits/IsFloatingPoint.h"
#include "Templates/TypeTraits/IsDestructible.h"
#include "Templates/TypeTraits/IsNullPointer.h"
#include "Templates/TypeTraits/IsConvertible.h"
#include "Templates/TypeTraits/IsArithmetic.h"
#include "Templates/TypeTraits/IsAssignable.h"
#include "Templates/TypeTraits/IsReference.h"
#include "Templates/TypeTraits/IsUnsigned.h"
#include "Templates/TypeTraits/IsIntegral.h"
#include "Templates/TypeTraits/IsCallable.h"
#include "Templates/TypeTraits/IsTrivial.h"
#include "Templates/TypeTraits/IsPointer.h"
#include "Templates/TypeTraits/IsSigned.h"
#include "Templates/TypeTraits/IsEmpty.h"
#include "Templates/TypeTraits/IsFinal.h"
#include "Templates/TypeTraits/IsArray.h"
#include "Templates/TypeTraits/IsEnum.h"
#include "Templates/TypeTraits/IsSame.h"
#include "Templates/TypeTraits/IsVoid.h"
