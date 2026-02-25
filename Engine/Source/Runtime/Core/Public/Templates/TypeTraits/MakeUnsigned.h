// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#include "Templates/TypeTraits/SelectUnsignedInt.h"
#include "Templates/TypeTraits/EnumBase.h"
#include "Templates/TypeTraits/IsEnum.h"

#define MAKE_UNSIGNED_BASE(Specialize, Declare) template <> struct TMakeUnsigned<               Specialize, false> { typedef                Declare Type; }; \
												template <> struct TMakeUnsigned<const          Specialize, false> { typedef const          Declare Type; }; \
												template <> struct TMakeUnsigned<      volatile Specialize, false> { typedef       volatile Declare Type; }; \
												template <> struct TMakeUnsigned<const volatile Specialize, false> { typedef const volatile Declare Type; };

#define MAKE_UNSIGNED_CHAR(X) MAKE_UNSIGNED_BASE(X, typename TSelectUnsignedInt<sizeof(X)>::Type)

template <typename T, bool = TIsEnum<T>::Value>
struct TMakeUnsigned
{
};

// Base Types
MAKE_UNSIGNED_BASE(         char     , unsigned char     );
MAKE_UNSIGNED_BASE(  signed char     , unsigned char     );
MAKE_UNSIGNED_BASE(         short    , unsigned short    );
MAKE_UNSIGNED_BASE(         int      , unsigned int      );
MAKE_UNSIGNED_BASE(         long     , unsigned long     );
MAKE_UNSIGNED_BASE(         long long, unsigned long long);
MAKE_UNSIGNED_BASE(unsigned char     , unsigned char     );
MAKE_UNSIGNED_BASE(unsigned short    , unsigned short    );
MAKE_UNSIGNED_BASE(unsigned int      , unsigned int      );
MAKE_UNSIGNED_BASE(unsigned long     , unsigned long     );
MAKE_UNSIGNED_BASE(unsigned long long, unsigned long long);
#if PLATFORM_HAS_NATIVE_INT128_TYPE
MAKE_UNSIGNED_BASE(FPlatformTypes::int128 , FPlatformTypes::uint128);
MAKE_UNSIGNED_BASE(FPlatformTypes::uint128, FPlatformTypes::uint128);
#endif // PLATFORM_HAS_NATIVE_INT128_TYPE

// wchart & charx_t Types
MAKE_UNSIGNED_CHAR(wchar_t);
#if CC_LANGUAGE_STANDARD >= 2020
MAKE_UNSIGNED_CHAR(char8_t);
#endif
MAKE_UNSIGNED_CHAR(char16_t);
MAKE_UNSIGNED_CHAR(char32_t);

// Enum types
template <typename T>
struct TMakeUnsigned<               T, true> { typedef                typename TMakeUnsigned<typename TEnumBase<T>::Type>::Type Type; };
template <typename T>
struct TMakeUnsigned<const          T, true> { typedef const          typename TMakeUnsigned<typename TEnumBase<T>::Type>::Type Type; };
template <typename T>
struct TMakeUnsigned<      volatile T, true> { typedef       volatile typename TMakeUnsigned<typename TEnumBase<T>::Type>::Type Type; };
template <typename T>
struct TMakeUnsigned<const volatile T, true> { typedef const volatile typename TMakeUnsigned<typename TEnumBase<T>::Type>::Type Type; };

#undef MAKE_UNSIGNED_BASE
#undef MAKE_UNSIGNED_CHAR

template <typename T>
using TMakeUnsigned_t = typename TMakeUnsigned<T>::Type;
