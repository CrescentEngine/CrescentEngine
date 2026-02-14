// Copyright (C) 2026 ychgen, all rights reserved.

enum class EModuleLoadResult
{
	Successful,
	FileNotFound,
	LibraryLoadFailed,
	InitFailed
};

class CORE_API FModuleManager
{
public:
	// Will be removed, adding this so Core has a symbol to compile and become LIB
	void CompileSymbol();
public:
	static FModuleManager& Get()
	{
		static FModuleManager Instance;
		return Instance;
	}
};
