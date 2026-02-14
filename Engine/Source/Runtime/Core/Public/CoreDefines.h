// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#define CC_CONCAT_INNER(L, R) L##R
#define CC_CONCAT(L, R) CC_CONCAT_INNER(L, R)
#define CC_STRINGIZE_INNER(X) #X
#define CC_STRINGIZE(X) CC_STRINGIZE_INNER(X)

#define CC_PLATFORM_NAME CC_STRINGIZE(CR_PLATFORM_ID)
#define CC_RESOLVE_PLATFORM_TYPE(Prefix, Type) CC_CONCAT(Prefix, CC_CONCAT(CR_PLATFORM_ID, Type))

// FORCEINLINE
#if __clang__ || __GNUC__
	#define FORCEINLINE inline __attribute__((always_inline))
#elif _MSC_VER
	#define FORCEINLINE __forceinline
#else
	#define FORCEINLINE inline
#endif
