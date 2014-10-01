# Microsoft Developer Studio Project File - Name="RRRSrv" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=RRRSrv - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RRRSrv.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RRRSrv.mak" CFG="RRRSrv - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RRRSrv - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "RRRSrv - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/RRRSrv", FFAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RRRSrv - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\RobotWorld" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /machine:I386 /out:".\Release/CyeSrv.exe"

!ELSEIF  "$(CFG)" == "RRRSrv - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\RobotWorld" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:".\Debug/CyeSrv.exe"

!ENDIF 

# Begin Target

# Name "RRRSrv - Win32 Release"
# Name "RRRSrv - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\cyecom.cpp
# End Source File
# Begin Source File

SOURCE=.\CyeSrv.odl
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ProtocolDiagnostics.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=".\robot communication.cpp"
# End Source File
# Begin Source File

SOURCE=..\RobotWorld\rothmath.cpp
# End Source File
# Begin Source File

SOURCE=.\RRRClist.cpp
# End Source File
# Begin Source File

SOURCE=.\RRRComEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\RRRComm.cpp
# End Source File
# Begin Source File

SOURCE=.\RRRMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\RRRSrv.cpp
# End Source File
# Begin Source File

SOURCE=.\RRRSrv.rc
# End Source File
# Begin Source File

SOURCE=.\RRRSrvCom.cpp
# End Source File
# Begin Source File

SOURCE=.\RRRSrvDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\RRRSrvView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Trayicon.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\cyecom.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\ProtocolDiagnostics.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=".\robot communication.h"
# End Source File
# Begin Source File

SOURCE=..\RobotWorld\rothmath.h
# End Source File
# Begin Source File

SOURCE=..\RobotWorld\rrdim.h
# End Source File
# Begin Source File

SOURCE=.\RRRClist.h
# End Source File
# Begin Source File

SOURCE=.\RRRComEngine.h
# End Source File
# Begin Source File

SOURCE=.\RRRComm.h
# End Source File
# Begin Source File

SOURCE=.\RRRConst.h
# End Source File
# Begin Source File

SOURCE=.\RRRMsg.h
# End Source File
# Begin Source File

SOURCE=.\RRRSrv.h
# End Source File
# Begin Source File

SOURCE=.\RRRSrvCom.h
# End Source File
# Begin Source File

SOURCE=.\RRRSrvDoc.h
# End Source File
# Begin Source File

SOURCE=.\RRRSrvView.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Trayicon.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\dead.ico
# End Source File
# Begin Source File

SOURCE=.\res\receive.ico
# End Source File
# Begin Source File

SOURCE=.\res\RRRSrv.ico
# End Source File
# Begin Source File

SOURCE=.\res\RRRSrv.rc2
# End Source File
# Begin Source File

SOURCE=.\res\RRRSrvDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\send.ico
# End Source File
# Begin Source File

SOURCE=.\res\sendrecv.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=".\res\up+alive.ico"
# End Source File
# End Group
# End Target
# End Project
