// Copyright (C) 2026 ychgen, all rights reserved.
#pragma once

enum class EModuleType
{
    Engine,
    Plugin,
    Game
};

class IModule
{
    virtual ~IModule()
    {
    }

    virtual void Init()
    {
    }
    virtual void Shutdown()
    {
    }

    virtual EModuleType GetType()
    {
        return EModuleType::Engine;
    }
    virtual bool IsHotReloadable()
    {
        return true;
    }
};
