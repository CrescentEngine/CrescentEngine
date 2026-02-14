@echo off

where dotnet >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo DotNET not installed. You must download the .NET SDK in order to build the engine.
	goto Quit
)

dotnet publish "%CRESCENT_PATH%/Engine/Source/Programs/Moonquake" -c Release -r win-x64 --self-contained true -o "%CRESCENT_PATH%/Engine/Binaries/DotNET/BuildTool"
:Quit
if "%1"=="NoPause" goto SkipPause
pause
:SkipPause
exit /B
