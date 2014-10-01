# Microsoft Developer Studio Project File - Name="FlowChart" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FlowChart - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FlowChart.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FlowChart.mak" CFG="FlowChart - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FlowChart - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FlowChart - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Personal Robot/PC Code/FlowChart", ITAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FlowChart - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "FlowChart - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "FlowChart - Win32 Release"
# Name "FlowChart - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Arrow.cpp
# End Source File
# Begin Source File

SOURCE=.\Beep.cpp
# End Source File
# Begin Source File

SOURCE=.\Branch.cpp
# End Source File
# Begin Source File

SOURCE=.\BranchLoop.cpp
# End Source File
# Begin Source File

SOURCE=.\Calibrate.cpp
# End Source File
# Begin Source File

SOURCE=.\Cue.cpp
# End Source File
# Begin Source File

SOURCE=.\DropOff.cpp
# End Source File
# Begin Source File

SOURCE=.\Execute.cpp
# End Source File
# Begin Source File

SOURCE=.\ExternalOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowChart.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowChart.rc
# End Source File
# Begin Source File

SOURCE=.\FlowChartDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowZap.rc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Goto.cpp
# End Source File
# Begin Source File

SOURCE=.\Instruction.cpp
# End Source File
# Begin Source File

SOURCE=.\LeftLoopPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\Loop.cpp
# End Source File
# Begin Source File

SOURCE=.\LoopPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\Motion.cpp
# End Source File
# Begin Source File

SOURCE=.\Pallette.cpp
# End Source File
# Begin Source File

SOURCE=.\PalletteItem.cpp
# End Source File
# Begin Source File

SOURCE=.\PathFollow.cpp
# End Source File
# Begin Source File

SOURCE=.\Preferences.cpp
# End Source File
# Begin Source File

SOURCE=.\Repeat.cpp
# End Source File
# Begin Source File

SOURCE=.\RepeatLoop.cpp
# End Source File
# Begin Source File

SOURCE=.\RightLoopPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\Start.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Stop.cpp
# End Source File
# Begin Source File

SOURCE=.\Sub.cpp
# End Source File
# Begin Source File

SOURCE=.\Tool.cpp
# End Source File
# Begin Source File

SOURCE=.\Util.cpp

!IF  "$(CFG)" == "FlowChart - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "FlowChart - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Vacuum.cpp
# End Source File
# Begin Source File

SOURCE=.\Wait.cpp
# End Source File
# Begin Source File

SOURCE=.\Zone.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Arrow.h
# End Source File
# Begin Source File

SOURCE=.\Beep.h
# End Source File
# Begin Source File

SOURCE=.\Branch.h
# End Source File
# Begin Source File

SOURCE=.\BranchLoop.h
# End Source File
# Begin Source File

SOURCE=.\Calibrate.h
# End Source File
# Begin Source File

SOURCE=.\Cue.h
# End Source File
# Begin Source File

SOURCE=.\DropOff.h
# End Source File
# Begin Source File

SOURCE=.\Execute.h
# End Source File
# Begin Source File

SOURCE=.\ExternalOutput.h
# End Source File
# Begin Source File

SOURCE=.\FlowChart.h
# End Source File
# Begin Source File

SOURCE=.\FlowChartDlg.h
# End Source File
# Begin Source File

SOURCE=.\FlowChartResource.h
# End Source File
# Begin Source File

SOURCE=.\FlowZap.h
# End Source File
# Begin Source File

SOURCE=.\FlowZapResource.h
# End Source File
# Begin Source File

SOURCE=.\Goto.h
# End Source File
# Begin Source File

SOURCE=.\Instruction.h
# End Source File
# Begin Source File

SOURCE=.\LeftLoopPanel.h
# End Source File
# Begin Source File

SOURCE=.\Loop.h
# End Source File
# Begin Source File

SOURCE=.\LoopPanel.h
# End Source File
# Begin Source File

SOURCE=.\Motion.h
# End Source File
# Begin Source File

SOURCE=.\Pallette.h
# End Source File
# Begin Source File

SOURCE=.\PalletteItem.h
# End Source File
# Begin Source File

SOURCE=.\PathFollow.h
# End Source File
# Begin Source File

SOURCE=.\Preferences.h
# End Source File
# Begin Source File

SOURCE=.\Repeat.h
# End Source File
# Begin Source File

SOURCE=.\RepeatLoop.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RightLoopPanel.h
# End Source File
# Begin Source File

SOURCE=.\Start.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Stop.h
# End Source File
# Begin Source File

SOURCE=.\Sub.h
# End Source File
# Begin Source File

SOURCE=.\Tool.h
# End Source File
# Begin Source File

SOURCE=.\Util.h
# End Source File
# Begin Source File

SOURCE=.\Vacuum.h
# End Source File
# Begin Source File

SOURCE=.\Wait.h
# End Source File
# Begin Source File

SOURCE=.\Zone.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\beep_bw.ico
# End Source File
# Begin Source File

SOURCE=.\res\beep_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BeepBW.ico
# End Source File
# Begin Source File

SOURCE=.\res\BeepDisabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\BeepEnabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\beepu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\branch_b.ico
# End Source File
# Begin Source File

SOURCE=.\res\branch_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BranchBW.ico
# End Source File
# Begin Source File

SOURCE=.\res\BranchDisabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\BranchEnabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\branchu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cal_bw.ico
# End Source File
# Begin Source File

SOURCE=.\res\CalDisabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\CalEnabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\calibrateu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cBeep.cur
# End Source File
# Begin Source File

SOURCE=.\res\cBranch.cur
# End Source File
# Begin Source File

SOURCE=.\res\cCal.cur
# End Source File
# Begin Source File

SOURCE=.\res\cGoto.cur
# End Source File
# Begin Source File

SOURCE=.\res\checkpoi.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cMotion.cur
# End Source File
# Begin Source File

SOURCE=.\res\cMove.cur
# End Source File
# Begin Source File

SOURCE=.\res\cRepeat.cur
# End Source File
# Begin Source File

SOURCE=.\res\cStop.cur
# End Source File
# Begin Source File

SOURCE=.\res\cSub.cur
# End Source File
# Begin Source File

SOURCE=.\res\cVac.cur
# End Source File
# Begin Source File

SOURCE=.\res\cWait.cur
# End Source File
# Begin Source File

SOURCE=".\res\external output disabled.ico"
# End Source File
# Begin Source File

SOURCE=".\res\external output.cur"
# End Source File
# Begin Source File

SOURCE=.\res\external_output.ico
# End Source File
# Begin Source File

SOURCE=.\res\externalu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FlowChart.ico
# End Source File
# Begin Source File

SOURCE=.\res\FlowChart.rc2
# End Source File
# Begin Source File

SOURCE=.\res\FZ_start_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FZbracket.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FZcheck.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FZclock.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FZhotpoint.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FZhotspot.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FZmusic.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FZnudge.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FZon_off.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FZpath.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FZrepeat.bmp
# End Source File
# Begin Source File

SOURCE=".\res\FZrot+go.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\FZstop.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FZvac.bmp
# End Source File
# Begin Source File

SOURCE=.\res\goto_bw.ico
# End Source File
# Begin Source File

SOURCE=.\res\GotoDisabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\GotoEnabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\grab.cur
# End Source File
# Begin Source File

SOURCE=.\res\hotpoint.bmp
# End Source File
# Begin Source File

SOURCE=.\res\iplay_st.ico
# End Source File
# Begin Source File

SOURCE=.\res\motion.ico
# End Source File
# Begin Source File

SOURCE=.\res\motion_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MotionBW.ico
# End Source File
# Begin Source File

SOURCE=.\res\MotionDisabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\motionu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\onoff_di.bmp
# End Source File
# Begin Source File

SOURCE=.\res\path.cur
# End Source File
# Begin Source File

SOURCE=.\res\path_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\path_dis.ico
# End Source File
# Begin Source File

SOURCE=.\res\PathEnabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\pathu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PlayPause.ico
# End Source File
# Begin Source File

SOURCE=.\res\repeat_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RepeatBW.ico
# End Source File
# Begin Source File

SOURCE=.\res\RepeatDisabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\RepeatEnabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\repeatu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Start.ico
# End Source File
# Begin Source File

SOURCE=.\res\Stop.ico
# End Source File
# Begin Source File

SOURCE=.\res\stop_bw.ico
# End Source File
# Begin Source File

SOURCE=.\res\stop_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\stop_pla.ico
# End Source File
# Begin Source File

SOURCE=.\res\StopDisabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\stopu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sub_disa.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SubBW.ico
# End Source File
# Begin Source File

SOURCE=.\res\SubDisabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\SubEnabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\subu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\vacuum_b.ico
# End Source File
# Begin Source File

SOURCE=.\res\vacuum_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VacuumDisabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\VacuumEnabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\vacuumu.bmp
# End Source File
# Begin Source File

SOURCE=".\res\wait bw.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\wait_bw.ico
# End Source File
# Begin Source File

SOURCE=.\res\wait_col.bmp
# End Source File
# Begin Source File

SOURCE=.\res\wait_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WaitDisabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\WaitEnabled.ico
# End Source File
# Begin Source File

SOURCE=.\res\waitu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\x.cur
# End Source File
# Begin Source File

SOURCE=.\res\zapIDbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\sounds\addInstruction.wav
# End Source File
# Begin Source File

SOURCE=.\sounds\choice.wav
# End Source File
# Begin Source File

SOURCE=.\sounds\condense.wav
# End Source File
# Begin Source File

SOURCE=.\sounds\expand.wav
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\sounds\removeInstruction.wav
# End Source File
# Begin Source File

SOURCE=.\sounds\select.wav
# End Source File
# End Target
# End Project
# Section FlowChart : {0000FF00-FF00-00FF-FF00-0000FF00FF00}
# 	1:10:IDR_POPUP1:103
# End Section
