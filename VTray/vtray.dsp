# Microsoft Developer Studio Project File - Name="vtray" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=vtray - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vtray.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vtray.mak" CFG="vtray - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vtray - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vtray - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "vtray - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GRDTRAY2_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /O1 /Op /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /o "NUL" /win32
# SUBTRACT MTL /mktyplib203 /Oicf
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 gdi32.lib advapi32.lib shell32.lib ole32.lib ../current/lsapi/Release/lsapi.lib kernel32.lib user32.lib /nologo /base:"0x15000000" /version:3.0 /subsystem:windows /dll /pdb:none /machine:I386 /opt:NOWIN98
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "vtray - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 gdi32.lib advapi32.lib shell32.lib ole32.lib ../current/lsapi/Debug/lsapi.lib kernel32.lib user32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "vtray - Win32 Release"
# Name "vtray - Win32 Debug"
# Begin Group "Source"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\FlashSettingsList.cpp
# End Source File
# Begin Source File

SOURCE=.\HideSettingsList.cpp
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\TrayIconSettingsList.cpp
# End Source File
# Begin Source File

SOURCE=.\vtray.cpp
# End Source File
# End Group
# Begin Group "VTK"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\VTK\apis.cpp
# End Source File
# Begin Source File

SOURCE=..\VTK\BoxPainter.cpp
# End Source File
# Begin Source File

SOURCE=..\VTK\Clickable.cpp
# End Source File
# Begin Source File

SOURCE=..\VTK\CountingBangRunner.cpp
# End Source File
# Begin Source File

SOURCE=..\VTK\GuiWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\VTK\Hover.cpp
# End Source File
# Begin Source File

SOURCE=..\VTK\Icon.cpp
# End Source File
# Begin Source File

SOURCE=..\VTK\LSUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\VTK\MultipleConfigReader.cpp
# End Source File
# Begin Source File

SOURCE=..\VTK\window.cpp
# End Source File
# Begin Source File

SOURCE=..\VTK\WinUtils.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\Defines.h
# End Source File
# Begin Source File

SOURCE=.\FlashSettingsList.h
# End Source File
# Begin Source File

SOURCE=.\HideSettingsList.h
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.h
# End Source File
# Begin Source File

SOURCE=.\TrayIconSettingsList.h
# End Source File
# Begin Source File

SOURCE=.\vtray.h
# End Source File
# End Group
# Begin Source File

SOURCE=".\VTray-changelog.txt"
# End Source File
# Begin Source File

SOURCE=".\VTray-documentation.txt"
# End Source File
# End Target
# End Project
