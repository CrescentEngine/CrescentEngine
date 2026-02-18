@echo off
setlocal enabledelayedexpansion

set MESSAGE_NOT_IN_ENGINE_ROOT=This script must be run directly from the root directory of the engine.

:: Fix CD when admin-launched
cd %~dp0
:: Some checks to see if we are on root directory of the engine.
if not exist Engine (
	echo %MESSAGE_NOT_IN_ENGINE_ROOT%
	goto Quit
)
if not exist Engine\Engine.mqroot (
	echo %MESSAGE_NOT_IN_ENGINE_ROOT%
	goto Quit
)
if not exist Engine\Build\Scripts\Windows\BuildTool.bat (
	echo %MESSAGE_NOT_IN_ENGINE_ROOT%
	goto Quit
)

if not defined CRESCENT_PATH (
	net session >nul 2>&1
	if %errorLevel% neq 0 (
		echo This script must be run with administrative privileges when the CRESCENT_PATH environment is unset.
			goto Quit
	)
	echo Setting system environment variable CRESCENT_PATH...
	set "PATH_SLASH=!CD:\=/!"
	setx /M CRESCENT_PATH "!PATH_SLASH!"
)

:: Build Moonquake
echo Building Moonquake (Build Tool)...
call .\Engine\Build\Scripts\Windows\BuildTool.bat NoPause

:: Generate Project Files
call .\Engine\Build\Scripts\Windows\GenerateProjectFiles.bat NoPause

:Quit
pause
exit /B
