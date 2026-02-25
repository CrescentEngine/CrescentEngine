// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Templates/TypeTraits/SelectSignedInt.h"
#include "Templates/TypeTraits/EnumBase.h"
#include "Templates/TypeTraits/IsEnum.h"

#define MAKE_SIGNED_BASE(Specialize, Declare) template <> struct TMakeSigned<               Specialize, false> { typedef                Declare Type; }; \
											  template <> struct TMakeSigned<const          Specialize, false> { typedef const          Declare Type; }; \
											  template <> struct TMakeSigned<      volatile Specialize, false> { typedef       volatile Declare Type; }; \
											  template <> struct TMakeSigned<const volatile Specialize, false> { typedef const volatile Declare Type; };

#define MAKE_SIGNED_CHAR(X) MAKE_SIGNED_BASE(X, typename TSelectSignedInt<sizeof(X)>::Type)

template <typename T, bool = TIsEnum<T>::Value>
struct TMakeSigned
{
};

// Base Types
MAKE_SIGNED_BASE(         char     , signed char     );
MAKE_SIGNED_BASE(  signed char     , signed char     );
MAKE_SIGNED_BASE(         short    , signed short    );
MAKE_SIGNED_BASE(         int      , signed int      );
MAKE_SIGNED_BASE(         long     , signed long     );
MAKE_SIGNED_BASE(         long long, signed long long);
MAKE_SIGNED_BASE(unsigned char     , signed char     );
MAKE_SIGNED_BASE(unsigned short    , signed short    );
MAKE_SIGNED_BASE(unsigned int      , signed int      );
MAKE_SIGNED_BASE(unsigned long     , signed long     );
MAKE_SIGNED_BASE(unsigned long long, signed long long);
#if PLATFORM_HAS_NATIVE_INT128_TYPE
MAKE_SIGNED_BASE(FPlatformTypes::int128 , FPlatformTypes::int128);
MAKE_SIGNED_BASE(FPlatformTypes::uint128, FPlatformTypes::int128);
#endif // PLATFORM_HAS_NATIVE_INT128_TYPE

// wchart & charx_t Types
MAKE_SIGNED_CHAR(wchar_t);
#if CC_LANGUAGE_STANDARD >= 2020
MAKE_SIGNED_CHAR(char8_t);
#endif
MAKE_SIGNED_CHAR(char16_t);
MAKE_SIGNED_CHAR(char32_t);

// Enum types
template <typename T>
struct TMakeSigned<               T, true> { typedef                typename TMakeSigned<typename TEnumBase<T>::Type>::Type Type; };
template <typename T>
struct TMakeSigned<const          T, true> { typedef const          typename TMakeSigned<typename TEnumBase<T>::Type>::Type Type; };
template <typename T>
struct TMakeSigned<      volatile T, true> { typedef       volatile typename TMakeSigned<typename TEnumBase<T>::Type>::Type Type; };
template <typename T>
struct TMakeSigned<const volatile T, true> { typedef const volatile typename TMakeSigned<typename TEnumBase<T>::Type>::Type Type; };

#undef MAKE_SIGNED_BASE
#undef MAKE_SIGNED_CHAR

template <typename T>
using TMakeSigned_t = typename TMakeSigned<T>::Type;
