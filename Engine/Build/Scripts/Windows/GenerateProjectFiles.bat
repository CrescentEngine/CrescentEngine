@echo off

set BUILD_TOOL=%CRESCENT_PATH%/Engine/Binaries/DotNET/BuildTool/Moonquake.exe
if not exist %BUILD_TOOL% (
	echo "Engine Build Tool (Moonquake) could not be located. Make sure you have run Setup.bat (recommended and required) or BuildTool.bat (only builds Moonquake)."
	goto Quit
)

echo Generating project files...
call %BUILD_TOOL% generate -d VisualStudio %CRESCENT_PATH%\Engine CrescentEngine
pause
exit /B
