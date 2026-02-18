// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

#define CC_CONCAT_INNER(L, R) L##R
#define CC_CONCAT(L, R) CC_CONCAT_INNER(L, R)
#define CC_STRINGIZE_INNER(X) #X
#define CC_STRINGIZE(X) CC_STRINGIZE_INNER(X)

#define CC_PLATFORM_NAME CC_STRINGIZE(CR_PLATFORM_ID)
#define CC_RESOLVE_PLATFORM_TYPE(Prefix, Type) CC_CONCAT(Prefix, CC_CONCAT(CR_PLATFORM_ID, Type))

#if CC_COMPILER_MSVC
	#define FORCEINLINE __forceinline
	#define NORETURN    __declspec(noreturn)
	#define RESTRICT    __restrict
#elif CC_COMPILER_CLANG || CC_COMPILER_GCC
	#define FORCEINLINE inline __attribute__((always_inline))
	#define NORETURN           __attribute__((noreturn))
	#define RESTRICT           __restrict__
#else
	#define FORCEINLINE inline
	#if CC_LANGUAGE_STANDARD >= 2011
		#define NORETURN    [[noreturn]]
	#else
		#define NORETURN
	#endif
	#define RESTRICT
#endif
