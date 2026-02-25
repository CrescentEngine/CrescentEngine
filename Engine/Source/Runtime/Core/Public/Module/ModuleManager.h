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
	static FModuleManager& Get()
	{
		static FModuleManager Instance;
		return Instance;
	}
};
