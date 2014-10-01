# Microsoft Developer Studio Project File - Name="RobotWorld" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=RobotWorld - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RobotWorld.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RobotWorld.mak" CFG="RobotWorld - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RobotWorld - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "RobotWorld - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Personal Robot/RobotWorld/RobotWorld", QEAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RobotWorld - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\FlowZap" /I ".\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "ROBOT_WORLD_PROJECT" /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i ".\FlowZap" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 "ateam.lib" "dxguid.lib" "ddraw.lib" "base.lib" "list.lib" /nologo /subsystem:windows /profile /machine:I386 /out:".\Release/Map-N-Zap.exe" /libpath:"..\Jim Hemmerle Stuff\Grid\lib"

!ELSEIF  "$(CFG)" == "RobotWorld - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\\" /I ".\FlowZap" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "ROBOT_WORLD_PROJECT" /FR /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i ".\FlowZap" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 "ddraw.lib" "dxguid.lib" "vfw32.lib" /nologo /subsystem:windows /debug /machine:I386 /out:".\Debug/Map-N-Zap.exe"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "RobotWorld - Win32 Release"
# Name "RobotWorld - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\FlowZap\Arrow.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Beep.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Branch.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\BranchLoop.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Calibrate.cpp
# End Source File
# Begin Source File

SOURCE=".\Calibration Zone.cpp"
# End Source File
# Begin Source File

SOURCE=.\canibuton.cpp
# End Source File
# Begin Source File

SOURCE=.\CharMatrix.cpp

!IF  "$(CFG)" == "RobotWorld - Win32 Release"

!ELSEIF  "$(CFG)" == "RobotWorld - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\check.cpp

!IF  "$(CFG)" == "RobotWorld - Win32 Release"

!ELSEIF  "$(CFG)" == "RobotWorld - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CTDPath.cpp

!IF  "$(CFG)" == "RobotWorld - Win32 Release"

!ELSEIF  "$(CFG)" == "RobotWorld - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FlowZap\Cue.cpp
# End Source File
# Begin Source File

SOURCE=.\cyecom.cpp
# End Source File
# Begin Source File

SOURCE=.\CyePersona.cpp
# End Source File
# Begin Source File

SOURCE=.\DbrFlowZapControl.cpp
# End Source File
# Begin Source File

SOURCE=.\DbrRobotExternals.cpp
# End Source File
# Begin Source File

SOURCE=.\ddutil2.cpp

!IF  "$(CFG)" == "RobotWorld - Win32 Release"

!ELSEIF  "$(CFG)" == "RobotWorld - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgAddHomeBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAddRobot.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCalibrateSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCalibrationSuccessful.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCreateCalibrationWall1.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCreateCheckPointSlipperySurface.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCreateHomeOnWood.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCreateVacuumPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCyeSample.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEditCalibrationZone.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgErrorCheckingWoodWithVacuum.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEStop.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgExecutableObjectGeneralProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFindHomebaseCorner.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHomeBaseFloorType.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgJoystickControl.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLabelName.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLinePathProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMotorControls.cpp
# End Source File
# Begin Source File

SOURCE=.\dlgmousecontrols.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOrientHomebase.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPropFloor.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRelocateRobot.cpp
# End Source File
# Begin Source File

SOURCE=.\dlgroboterror.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSetDisplayResolution.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTestRobot.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVelocityTorqueDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVideo.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\DropOff.cpp
# End Source File
# Begin Source File

SOURCE=.\ExecutableRWGraphicObject.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Execute.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\ExternalOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\FindPathThread.cpp
# End Source File
# Begin Source File

SOURCE=.\FinishLine.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\FlowZap.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\FlowZap.rc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\gauge.cpp
# End Source File
# Begin Source File

SOURCE=.\GoalSet.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Goto.cpp
# End Source File
# Begin Source File

SOURCE=.\Grid.cpp

!IF  "$(CFG)" == "RobotWorld - Win32 Release"

!ELSEIF  "$(CFG)" == "RobotWorld - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\HomeBase.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Instruction.cpp
# End Source File
# Begin Source File

SOURCE=.\IPathPlanner.cpp
# End Source File
# Begin Source File

SOURCE=.\Laygo.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\LeftLoopPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\LinePath.cpp
# End Source File
# Begin Source File

SOURCE=.\LineRectTracker.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Loop.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\LoopPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MakeHelp.bat
# End Source File
# Begin Source File

SOURCE=".\hlp\Map-N-Zap.hpj"

!IF  "$(CFG)" == "RobotWorld - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=".\hlp\Map-N-Zap.hpj"

"hlp\RobotWorld.hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	del hlp\RobotWorld.hm 
	REM -- First make map file from Microsoft Visual C++ generated resource.h 
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
	REM -- Run rtffix.exe on rtf files to fix autonumbering problem 
	rem echo hlp\rtffix.exe hlp\*.rtf 
	rem hlp\rtffix.exe hlp\*.rtf 
	REM -- Make help for Project RobotWorld 
	rem echo Building Win32 Help files 
	rem start /wait hcw /C /E /M "hlp\Map-N-Zap.hpj" 
	rem if errorlevel 1 goto :Error 
	rem if not exist "hlp\Map-N-Zap.hlp" goto :Error 
	rem if not exist "hlp\Map-N-Zap.cnt" goto :Error 
	rem echo. 
	rem copy "hlp\Map-N-Zap.hlp" $(OutDir) 
	rem copy "hlp\Map-N-Zap.cnt" $(OutDir) 
	rem copy "hlp\Map-N-Zap.hlp" 
	rem copy "hlp\Map-N-Zap.cnt" 
	rem echo. 
	rem goto :done 
	rem :Error 
	rem echo hlp\RobotWorld.hpj(1) : error: Problem encountered creating help file 
	rem :done 
	rem echo. 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "RobotWorld - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=".\hlp\Map-N-Zap.hpj"

"hlp\RobotWorld.hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	del hlp\RobotWorld.hm 
	REM -- First make map file from Microsoft Visual C++ generated resource.h 
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
	REM -- Run rtffix.exe on rtf files to fix autonumbering problem 
	rem echo hlp\rtffix.exe hlp\*.rtf 
	rem hlp\rtffix.exe hlp\*.rtf 
	REM -- Make help for Project RobotWorld 
	rem echo Building Win32 Help files 
	rem start /wait hcw /C /E /M "hlp\Map-N-Zap.hpj" 
	rem if errorlevel 1 goto :Error 
	rem if not exist "hlp\Map-N-Zap.hlp" goto :Error 
	rem if not exist "hlp\Map-N-Zap.cnt" goto :Error 
	rem echo. 
	rem copy "hlp\Map-N-Zap.hlp" $(OutDir) 
	rem copy "hlp\Map-N-Zap.cnt" $(OutDir) 
	rem copy "hlp\Map-N-Zap.hlp" 
	rem copy "hlp\Map-N-Zap.cnt" 
	rem echo. 
	rem goto :done 
	rem :Error 
	rem echo hlp\RobotWorld.hpj(1) : error: Problem encountered creating help file 
	rem :done 
	rem echo. 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FlowZap\Motion.cpp
# End Source File
# Begin Source File

SOURCE=.\oleobject.cpp
# End Source File
# Begin Source File

SOURCE=.\oleobjects.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Pallette.cpp
# End Source File
# Begin Source File

SOURCE=.\flowzap\PalletteItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Path.cpp
# End Source File
# Begin Source File

SOURCE=.\PathDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\PathFollow.cpp
# End Source File
# Begin Source File

SOURCE=.\PathList.cpp
# End Source File
# Begin Source File

SOURCE=.\PBehavior.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\PMotion.cpp
# End Source File
# Begin Source File

SOURCE=.\PositionLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Preferences.cpp
# End Source File
# Begin Source File

SOURCE=".\Probability Grid.cpp"
# End Source File
# Begin Source File

SOURCE=.\PropGeneral.cpp
# End Source File
# Begin Source File

SOURCE=.\PropRobotColor.cpp
# End Source File
# Begin Source File

SOURCE=.\PSong.cpp
# End Source File
# Begin Source File

SOURCE=".\Red Zone.cpp"
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Repeat.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\RepeatLoop.cpp
# End Source File
# Begin Source File

SOURCE=.\richtext.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\RightLoopPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\Robot.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotCom.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotInstruction.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotWorld.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotWorld.odl
# End Source File
# Begin Source File

SOURCE=.\RobotWorldDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotWorldView.cpp
# End Source File
# Begin Source File

SOURCE=.\RothMath.cpp
# End Source File
# Begin Source File

SOURCE=.\RothString.cpp
# End Source File
# Begin Source File

SOURCE=.\RothTime.cpp
# End Source File
# Begin Source File

SOURCE=.\RothUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\Rover.cpp

!IF  "$(CFG)" == "RobotWorld - Win32 Release"

!ELSEIF  "$(CFG)" == "RobotWorld - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RulerGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\RWGraphicObject.cpp
# End Source File
# Begin Source File

SOURCE=.\SplashWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Start.cpp
# End Source File
# Begin Source File

SOURCE=.\StartLine.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Stop.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Sub.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Tool.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Util.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Vacuum.cpp
# End Source File
# Begin Source File

SOURCE=.\VacuumPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Wait.cpp
# End Source File
# Begin Source File

SOURCE=.\World.cpp

!IF  "$(CFG)" == "RobotWorld - Win32 Release"

!ELSEIF  "$(CFG)" == "RobotWorld - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FlowZap\Zone.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\FlowZap\Arrow.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Beep.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Branch.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\BranchLoop.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Calibrate.h
# End Source File
# Begin Source File

SOURCE=".\Calibration Zone.h"
# End Source File
# Begin Source File

SOURCE=.\canibuton.h
# End Source File
# Begin Source File

SOURCE=.\CharMatrix.h
# End Source File
# Begin Source File

SOURCE=.\CTDPath.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Cue.h
# End Source File
# Begin Source File

SOURCE=.\cyecom.h
# End Source File
# Begin Source File

SOURCE=.\CyePersona.h
# End Source File
# Begin Source File

SOURCE=.\DbrFlowZapControl.h
# End Source File
# Begin Source File

SOURCE=.\DbrRobotExternals.h
# End Source File
# Begin Source File

SOURCE=.\ddutil.h
# End Source File
# Begin Source File

SOURCE=.\ddutil2.h
# End Source File
# Begin Source File

SOURCE=.\DlgAddHomebase.h
# End Source File
# Begin Source File

SOURCE=.\DlgAddRobot.h
# End Source File
# Begin Source File

SOURCE=.\DlgCalibrateSurface.h
# End Source File
# Begin Source File

SOURCE=.\DlgCalibrationSuccessful.h
# End Source File
# Begin Source File

SOURCE=.\DlgCreateCalibrationWall1.h
# End Source File
# Begin Source File

SOURCE=.\DlgCreateCheckPointSlipperySurface.h
# End Source File
# Begin Source File

SOURCE=.\DlgCreateHomeOnWood.h
# End Source File
# Begin Source File

SOURCE=.\DlgCreateVacuumPatch.h
# End Source File
# Begin Source File

SOURCE=.\DlgCyeSample.h
# End Source File
# Begin Source File

SOURCE=.\DlgEditCalibrationZone.h
# End Source File
# Begin Source File

SOURCE=.\DlgErrorCheckingWoodWithVacuum.h
# End Source File
# Begin Source File

SOURCE=.\DlgEStop.h
# End Source File
# Begin Source File

SOURCE=.\DlgExecutableObjectGeneralProperties.h
# End Source File
# Begin Source File

SOURCE=.\DlgFindHomebaseCorner.h
# End Source File
# Begin Source File

SOURCE=.\DlgHomeBaseFloorType.h
# End Source File
# Begin Source File

SOURCE=.\DlgJoystickControl.h
# End Source File
# Begin Source File

SOURCE=.\DlgLabelName.h
# End Source File
# Begin Source File

SOURCE=.\DlgLinePathProperties.h
# End Source File
# Begin Source File

SOURCE=.\DlgMotorControls.h
# End Source File
# Begin Source File

SOURCE=.\dlgmousecontrols.h
# End Source File
# Begin Source File

SOURCE=.\DlgOrientHomebase.h
# End Source File
# Begin Source File

SOURCE=.\DlgPropFloor.h
# End Source File
# Begin Source File

SOURCE=.\DlgRelocateRobot.h
# End Source File
# Begin Source File

SOURCE=.\dlgroboterror.h
# End Source File
# Begin Source File

SOURCE=.\DlgSetDisplayResolution.h
# End Source File
# Begin Source File

SOURCE=.\DlgTestRobot.h
# End Source File
# Begin Source File

SOURCE=.\DlgVelocityTorqueDisplay.h
# End Source File
# Begin Source File

SOURCE=.\DlgVideo.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\DropOff.h
# End Source File
# Begin Source File

SOURCE=.\ExecutableRWGraphicObject.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Execute.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\ExternalOutput.h
# End Source File
# Begin Source File

SOURCE=.\FindPathThread.h
# End Source File
# Begin Source File

SOURCE=.\FinishLine.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\FlowZap.h
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\gauge.h
# End Source File
# Begin Source File

SOURCE=.\global.h
# End Source File
# Begin Source File

SOURCE=.\GoalSet.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Goto.h
# End Source File
# Begin Source File

SOURCE=.\Grid.h
# End Source File
# Begin Source File

SOURCE=.\HomeBase.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Instruction.h
# End Source File
# Begin Source File

SOURCE=.\IPathPlanner.h
# End Source File
# Begin Source File

SOURCE=.\Laygo.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\LeftLoopPanel.h
# End Source File
# Begin Source File

SOURCE=.\LinePath.h
# End Source File
# Begin Source File

SOURCE=.\LineRectTracker.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Loop.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\LoopPanel.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\mhcomm.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Motion.h
# End Source File
# Begin Source File

SOURCE=.\oleobject.h
# End Source File
# Begin Source File

SOURCE=.\oleobjects.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Pallette.h
# End Source File
# Begin Source File

SOURCE=.\Path.h
# End Source File
# Begin Source File

SOURCE=.\PathDisplay.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\PathFollow.h
# End Source File
# Begin Source File

SOURCE=.\PathList.h
# End Source File
# Begin Source File

SOURCE=.\PBehavior.h
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\PMotion.h
# End Source File
# Begin Source File

SOURCE=.\PositionLabel.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Preferences.h
# End Source File
# Begin Source File

SOURCE=".\Probability Grid.h"
# End Source File
# Begin Source File

SOURCE=.\PropGeneral.h
# End Source File
# Begin Source File

SOURCE=.\PropRobotColor.h
# End Source File
# Begin Source File

SOURCE=.\PSong.h
# End Source File
# Begin Source File

SOURCE=".\Red Zone.h"
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Repeat.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\RepeatLoop.h
# End Source File
# Begin Source File

SOURCE=.\richtext.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\RightLoopPanel.h
# End Source File
# Begin Source File

SOURCE=.\Robot.h
# End Source File
# Begin Source File

SOURCE=.\RobotCom.h
# End Source File
# Begin Source File

SOURCE=.\RobotInstruction.h
# End Source File
# Begin Source File

SOURCE=.\RobotStatus.h
# End Source File
# Begin Source File

SOURCE=".\RobotWorld Resource.h"
# End Source File
# Begin Source File

SOURCE=.\RobotWorld.h
# End Source File
# Begin Source File

SOURCE=.\RobotWorldDoc.h
# End Source File
# Begin Source File

SOURCE=.\RobotWorldView.h
# End Source File
# Begin Source File

SOURCE=.\RothMath.h
# End Source File
# Begin Source File

SOURCE=.\RothString.h
# End Source File
# Begin Source File

SOURCE=.\RothTime.h
# End Source File
# Begin Source File

SOURCE=.\RothUtil.h
# End Source File
# Begin Source File

SOURCE=.\Rover.h
# End Source File
# Begin Source File

SOURCE=.\RRDim.h
# End Source File
# Begin Source File

SOURCE=.\RulerGrid.h
# End Source File
# Begin Source File

SOURCE=.\RWGraphicObject.h
# End Source File
# Begin Source File

SOURCE=.\SerialCommEvents.h
# End Source File
# Begin Source File

SOURCE=.\SplashWnd.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Start.h
# End Source File
# Begin Source File

SOURCE=.\StartLine.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Stop.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Sub.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Tool.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Vacuum.h
# End Source File
# Begin Source File

SOURCE=.\VacuumPatch.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Wait.h
# End Source File
# Begin Source File

SOURCE=.\World.h
# End Source File
# Begin Source File

SOURCE=.\WorldGridConsts.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap\Zone.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\000tn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\011tn.BMP
# End Source File
# Begin Source File

SOURCE=.\res\022tn.BMP
# End Source File
# Begin Source File

SOURCE=.\res\033tn.BMP
# End Source File
# Begin Source File

SOURCE=.\res\044tn.BMP
# End Source File
# Begin Source File

SOURCE=.\res\200tn.BMP
# End Source File
# Begin Source File

SOURCE=.\res\300tn.BMP
# End Source File
# Begin Source File

SOURCE=.\res\arrow_l.bmp
# End Source File
# Begin Source File

SOURCE=.\res\arrow_m.bmp
# End Source File
# Begin Source File

SOURCE=.\res\arrow_s.bmp
# End Source File
# Begin Source File

SOURCE=".\res\battery charged.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\battery charging.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\battery not charging.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\battery_low.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\beep_bw.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\BeepBW.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\BeepDisabled.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\BeepEnabled.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\branch_b.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\BranchBW.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\BranchDisabled.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\BranchEnabled.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\cal_bw.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\CalDisabled.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\CalEnabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\CalZone.bmp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\cBeep.cur
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\cBranch.cur
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\cCal.cur
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\cGoto.cur
# End Source File
# Begin Source File

SOURCE=.\res\Chrometn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ClearPenActive.bmp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\cMotion.cur
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\cMove.cur
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\cRepeat.cur
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\cStop.cur
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\cSub.cur
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\cVac.cur
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\cWait.cur
# End Source File
# Begin Source File

SOURCE=".\res\draw freehand.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\draw line.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\draw rectgle.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\drawing_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\EStop.bmp
# End Source File
# Begin Source File

SOURCE=.\res\EStopD.bmp
# End Source File
# Begin Source File

SOURCE=".\res\finish line1.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line10.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line11.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line12.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line13.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line14.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line15.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line16.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line17.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line20.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line21.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line22.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line23.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line24.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line25.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line26.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line27.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line3.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line30.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line31.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line32.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line33.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line34.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line35.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line36.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\finish line37.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\fz_execute.bmp
# End Source File
# Begin Source File

SOURCE=".\res\fzPlay Disabled.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\fzShowFZ Disabled.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\fzShowFZ Enabled.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\fzShowFZ Pushed.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\fzStop Disabled.bmp"
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\goto_bw.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\GotoDisabled.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\GotoEnabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\inactivelines.bmp
# End Source File
# Begin Source File

SOURCE=.\res\inactivepens.bmp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\iplay_st.ico
# End Source File
# Begin Source File

SOURCE=.\res\joystick1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\joystick2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\joystick3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\joystick4.bmp
# End Source File
# Begin Source File

SOURCE=".\res\large black arrow.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\large grey arrow.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\large.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LargeLineActive.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LaygoDefault.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MainFrmTbr.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Map And Zap.ico"
# End Source File
# Begin Source File

SOURCE=.\res\map_tools.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mapIDbar.bmp
# End Source File
# Begin Source File

SOURCE=".\res\medium black arrow.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\medium grey arrow.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\medium.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MediumLineActive.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mm_block.cur
# End Source File
# Begin Source File

SOURCE=.\res\mm_clear.cur
# End Source File
# Begin Source File

SOURCE=.\res\mm_draw_freehand.cur
# End Source File
# Begin Source File

SOURCE=.\res\mm_draw_line.cur
# End Source File
# Begin Source File

SOURCE=.\res\mm_draw_rectangle.cur
# End Source File
# Begin Source File

SOURCE=.\res\mm_hand.cur
# End Source File
# Begin Source File

SOURCE=.\res\mm_move_robot.cur
# End Source File
# Begin Source File

SOURCE=".\res\mm_place finish line.cur"
# End Source File
# Begin Source File

SOURCE=.\res\mm_place_start.cur
# End Source File
# Begin Source File

SOURCE=.\res\mm_search.cur
# End Source File
# Begin Source File

SOURCE=.\res\mm_unkno.cur
# End Source File
# Begin Source File

SOURCE=.\res\mmedit.cur
# End Source File
# Begin Source File

SOURCE=.\res\mmerase.cur
# End Source File
# Begin Source File

SOURCE=.\res\mmplace_.cur
# End Source File
# Begin Source File

SOURCE=.\res\mmplacelinepath.cur
# End Source File
# Begin Source File

SOURCE=.\res\mmPlacePositionLabel.cur
# End Source File
# Begin Source File

SOURCE=.\res\mmplacevacuum.cur
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\motion.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\MotionBW.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\MotionDisabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\ObstaclePenActive.bmp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\path.cur
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\path_dis.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\PathEnabled.ico
# End Source File
# Begin Source File

SOURCE=".\res\Pause Disabled.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Pause Enabled.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Pause Pushed.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Play Enabled.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Play Pushed.bmp"
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\PlayPause.ico
# End Source File
# Begin Source File

SOURCE=".\res\position label1.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\position label10.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\position label11.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\position label12.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\position label13.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\position label14.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\position label15.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\position label2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\position label20.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\position label21.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\position label22.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\position label23.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\position label24.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\position label25.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\position label3.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\position label30.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\position label31.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\position label32.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\position label33.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\position label34.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\position label35.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\practice toolbar.bmp"
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\RedZone.bmp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\RepeatBW.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\RepeatDisabled.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\RepeatEnabled.ico
# End Source File
# Begin Source File

SOURCE=".\res\robot tools.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\robot_status_charged.bmp
# End Source File
# Begin Source File

SOURCE=.\res\robot_status_charging.bmp
# End Source File
# Begin Source File

SOURCE=.\res\robot_status_not_charging.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RobotControlTbr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RobotExternalsOff.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RobotExternalsOn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RobotWorld.ico
# End Source File
# Begin Source File

SOURCE=.\RobotWorld.rc
# End Source File
# Begin Source File

SOURCE=.\res\RobotWorld.rc2
# End Source File
# Begin Source File

SOURCE=.\res\RobotWorldDoc.ico
# End Source File
# Begin Source File

SOURCE=".\res\small black arrow.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\small grey arrow.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\small.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SmallLineActive.bmp
# End Source File
# Begin Source File

SOURCE=.\res\speedball.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splash.bmp
# End Source File
# Begin Source File

SOURCE=".\res\start line1.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\start line2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\start line3.bmp"
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\Start.ico
# End Source File
# Begin Source File

SOURCE=".\res\Stop Enabled.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Stop Pushed.bmp"
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\Stop.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\stop_bw.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\stop_pla.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\StopDisabled.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\SubBW.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\SubDisabled.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\SubEnabled.ico
# End Source File
# Begin Source File

SOURCE=".\res\timer bottom.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\timer left.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\timer right.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\timer top.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\UnknownPenActive.bmp
# End Source File
# Begin Source File

SOURCE=.\res\untitled.bmp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\vacuum_b.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\VacuumDisabled.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\VacuumEnabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\velocity_torque.bmp
# End Source File
# Begin Source File

SOURCE=".\FlowZap\res\wait bw.bmp"
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\wait_bw.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\wait_col.bmp
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\WaitDisabled.ico
# End Source File
# Begin Source File

SOURCE=.\FlowZap\res\WaitEnabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\XLargeLineActive.bmp
# End Source File
# End Group
# Begin Group "Help Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\hlp\AfxCore.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\AfxPrint.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\FlowZap.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\howto.rtf
# End Source File
# Begin Source File

SOURCE=".\hlp\Map-N-Zap.cnt"
# End Source File
# Begin Source File

SOURCE=.\hlp\RobotWorld.rtf
# End Source File
# End Group
# Begin Group "Sounds"

# PROP Default_Filter "*.wav"
# Begin Source File

SOURCE=.\flowzap\sounds\addInstruction.wav
# End Source File
# Begin Source File

SOURCE=.\flowzap\sounds\choice.wav
# End Source File
# Begin Source File

SOURCE=.\flowzap\sounds\condense.wav
# End Source File
# Begin Source File

SOURCE=.\flowzap\sounds\expand.wav
# End Source File
# Begin Source File

SOURCE=.\flowzap\sounds\removeInstruction.wav
# End Source File
# Begin Source File

SOURCE=.\flowzap\sounds\select.wav
# End Source File
# End Group
# Begin Source File

SOURCE=.\RobotWorld.reg
# End Source File
# Begin Source File

SOURCE=.\rover.dat
# End Source File
# Begin Source File

SOURCE=.\ddraw.lib
# End Source File
# End Target
# End Project
# Section RobotWorld : {58086B00-1F5D-11CF-A484-00403333EC93}
# 	0:7:MDraw.h:E:\RugRat\Code\Visual C++ 4.0\RobotWorld\MDraw.h
# 	0:9:MDraw.cpp:E:\RugRat\Code\Visual C++ 4.0\RobotWorld\MDraw.cpp
# 	2:21:DefaultSinkHeaderFile:mdraw.h
# 	2:16:DefaultSinkClass:CMDraw
# End Section
# Section RobotWorld : {859321D0-3FD1-11CF-8981-00AA00688B10}
# 	2:5:Class:COLEObjects
# 	2:10:HeaderFile:oleobjects.h
# 	2:8:ImplFile:oleobjects.cpp
# End Section
# Section RobotWorld : {11CB8561-54B8-11CF-8F33-00AA00B46FE8}
# 	2:5:Class:CGauge
# 	2:10:HeaderFile:gauge.h
# 	2:8:ImplFile:gauge.cpp
# End Section
# Section RobotWorld : {E9A5593C-CAB0-11D1-8C0B-0000F8754DA1}
# 	2:5:Class:CRichText
# 	2:10:HeaderFile:richtext.h
# 	2:8:ImplFile:richtext.cpp
# End Section
# Section RobotWorld : {0000FF00-FF00-00FF-FF00-0000FF00FF00}
# 	1:29:CG_IDR_POPUP_ROBOT_WORLD_VIEW:109
# End Section
# Section OLE Controls
# 	{11CB8560-54B8-11CF-8F33-00AA00B46FE8}
# 	{8BC1991E-3889-11CE-8748-524153480004}
# 	{58086B00-1F5D-11CF-A484-00403333EC93}
# 	{F72CC885-5ADC-101B-A56C-00AA003668DC}
# End Section
# Section RobotWorld : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section RobotWorld : {11CB8560-54B8-11CF-8F33-00AA00B46FE8}
# 	0:9:Picture.h:E:\RugRat\Code\Visual C++ 4.0\RobotWorld\Picture.h
# 	0:9:Gauge.cpp:E:\RugRat\Code\Visual C++ 4.0\RobotWorld\Gauge.cpp
# 	0:11:Picture.cpp:E:\RugRat\Code\Visual C++ 4.0\RobotWorld\Picture.cpp
# 	0:7:Gauge.h:E:\RugRat\Code\Visual C++ 4.0\RobotWorld\Gauge.h
# 	2:21:DefaultSinkHeaderFile:gauge.h
# 	2:16:DefaultSinkClass:CGauge
# End Section
# Section RobotWorld : {3B7C8860-D78F-101B-B9B5-04021C009402}
# 	2:21:DefaultSinkHeaderFile:richtext.h
# 	2:16:DefaultSinkClass:CRichText
# End Section
# Section RobotWorld : {1BB9C511-657F-11D0-8465-CA281D000000}
# 	2:5:Class:CRRRCom
# 	2:10:HeaderFile:rrrcom.h
# 	2:8:ImplFile:rrrcom.cpp
# End Section
# Section RobotWorld : {D92641A0-DF79-11CF-8E74-00A0C90F26F8}
# 	2:5:Class:CRichText
# 	2:10:HeaderFile:richtext.h
# 	2:8:ImplFile:richtext.cpp
# End Section
# Section RobotWorld : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section RobotWorld : {F72CC886-5ADC-101B-A56C-00AA003668DC}
# 	2:5:Class:CCanibuton
# 	2:10:HeaderFile:canibuton.h
# 	2:8:ImplFile:canibuton.cpp
# End Section
# Section RobotWorld : {8BC1991F-3889-11CE-8748-524153480004}
# 	2:5:Class:CMhComm
# 	2:10:HeaderFile:mhcomm.h
# 	2:8:ImplFile:mhcomm.cpp
# End Section
# Section RobotWorld : {58086B01-1F5D-11CF-A484-00403333EC93}
# 	2:5:Class:CMDraw
# 	2:10:HeaderFile:mdraw.h
# 	2:8:ImplFile:mdraw.cpp
# End Section
# Section RobotWorld : {ED117630-4090-11CF-8981-00AA00688B10}
# 	2:5:Class:COLEObject
# 	2:10:HeaderFile:oleobject.h
# 	2:8:ImplFile:oleobject.cpp
# End Section
# Section RobotWorld : {F72CC885-5ADC-101B-A56C-00AA003668DC}
# 	0:11:Canibuton.h:E:\RugRat\Code\Visual C++ 4.0\RobotWorld\Canibuton.h
# 	0:13:Canibuton.cpp:E:\RugRat\Code\Visual C++ 4.0\RobotWorld\Canibuton.cpp
# 	2:21:DefaultSinkHeaderFile:canibuton.h
# 	2:16:DefaultSinkClass:CCanibuton
# End Section
# Section RobotWorld : {1BB9C513-657F-11D0-8465-CA281D000000}
# 	2:21:DefaultSinkHeaderFile:cyecom.h
# 	2:16:DefaultSinkClass:CCyeCom
# End Section
# Section RobotWorld : {8BC1991E-3889-11CE-8748-524153480004}
# 	0:8:Font.cpp:E:\RugRat\Code\Visual C++ 4.0\RobotWorld\Font.cpp
# 	0:10:MhComm.cpp:E:\RugRat\Code\Visual C++ 4.0\RobotWorld\MhComm.cpp
# 	0:6:Font.h:E:\RugRat\Code\Visual C++ 4.0\RobotWorld\Font.h
# 	0:8:MhComm.h:E:\RugRat\Code\Visual C++ 4.0\RobotWorld\MhComm.h
# 	2:21:DefaultSinkHeaderFile:mhcomm.h
# 	2:16:DefaultSinkClass:CMhComm
# End Section
