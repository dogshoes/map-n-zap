REM ----THIS FILE IS NO LONGER USED.  ALL THE FUNCTIONS ARE PERFORMED
REM ----BY THE CUSTOM BUILD PROPERTIES IN "RobotWorld Resource.h"
REM ----AND "Map-N-Zap.hpj"   S.R. 10/23/98

@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by RobotWorld.HPJ. >"hlp\RobotWorld.hm"
echo. >>"hlp\RobotWorld.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\RobotWorld.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 "RobotWorld Resource.h" >>"hlp\RobotWorld.hm"
echo. >>"hlp\RobotWorld.hm"
echo // Prompts (IDP_*) >>"hlp\RobotWorld.hm"
makehm IDP_,HIDP_,0x30000 "RobotWorld Resource.h" >>"hlp\RobotWorld.hm"
echo. >>"hlp\RobotWorld.hm"
echo // Resources (IDR_*) >>"hlp\RobotWorld.hm"
makehm IDR_,HIDR_,0x20000 "RobotWorld Resource.h" >>"hlp\RobotWorld.hm"
echo. >>"hlp\RobotWorld.hm"
echo // Dialogs (IDD_*) >>"hlp\RobotWorld.hm"
makehm IDD_,HIDD_,0x20000 "RobotWorld Resource.h" >>"hlp\RobotWorld.hm"
echo. >>"hlp\RobotWorld.hm"
echo // Frame Controls (IDW_*) >>"hlp\RobotWorld.hm"
makehm IDW_,HIDW_,0x50000 "RobotWorld Resource.h" >>"hlp\RobotWorld.hm"
REM -- Make help for Project RobotWorld


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\Map-N-Zap.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\Map-N-Zap.hlp" goto :Error
if not exist "hlp\Map-N-Zap.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\Map-N-Zap.hlp" Debug
if exist Debug\nul copy "hlp\Map-N-Zap.cnt" Debug
if exist Release\nul copy "hlp\Map-N-Zap.hlp" Release
if exist Release\nul copy "hlp\Map-N-Zap.cnt" Release
copy "hlp\Map-N-Zap.hlp"
copy "hlp\Map-N-Zap.cnt"
echo.
goto :done

:Error
echo hlp\RobotWorld.hpj(1) : error: Problem encountered creating help file

:done
echo.
