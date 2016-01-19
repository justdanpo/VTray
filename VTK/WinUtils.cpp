/*

	WinUtils

	Author:	Vendicator
	
	Description:
	Class that encapsulates some common Windows functions.

	Requires:

	Changelog:
	[2002-11-07 - Vendicator]
	- First work?
	
*/

#include "apis.h"
#include "WinUtils.h"

extern LPCSTR szLogName;

void WinUtils::LogWindow(HWND window)
{
	TCHAR name[128];
	memset(name, 0, sizeof(name)/sizeof(name[0]) );

	if (window)
	{
		GetClassName(window, name, sizeof(name)/sizeof(name[0]) );
		//GetWindowModuleFileName(window, path, sizeof(path)/sizeof(path[0]) );
		_LSLogPrintf(LOG_NOTICE, szLogName, "Window Info: %d - classname: %s", window, name);
	}
}

HWND WinUtils::GetDesktopHWND()
{
	HWND desktop = FindWindow(WC_SHELLDESKTOP, NULL);
	if (!desktop)
	{
#ifdef _DEBUG
		_LSLog(LOG_WARNING, szLogName, "Didn't find desktop window class, trying function");
#endif
		desktop = GetDesktopWindow();
	}
	if (desktop)
	{
		return desktop;
	}
	else
	{
#ifdef _DEBUG
		_LSLog(LOG_ERROR, szLogName, "Could not find desktop window");
#endif
		return NULL;
	}
}

bool WinUtils::IsScreenSaverRunning()
{
	BOOL Running = FALSE;
	BOOL res = SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &Running, 0);
	if (res)
		return (Running == TRUE);

	// Works under 95, 98 and NT>=5. For older OS:s below
	HDESK hDesk = OpenDesktop(TEXT("screen-saver"), 0, FALSE, MAXIMUM_ALLOWED);
	if (hDesk != NULL)
	{
		CloseDesktop(hDesk);
		return true;
	}
	if (GetLastError() == ERROR_ACCESS_DENIED)
		return true;
	else
		return false;
}

bool WinUtils::IsWinNT5(void)
{
	OSVERSIONINFO OSVersionInfo;
	OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVersionInfo);
	GetVersionEx(&OSVersionInfo);
	bool bWinNT5 = OSVersionInfo.dwMajorVersion >= 5 && OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT;
	return bWinNT5;
}

DWORD WinUtils::ModifyStyle(HWND hWnd, DWORD dwRemove, DWORD dwAdd)
{
	DWORD dwStyle = (DWORD) GetWindowLong(hWnd, GWL_STYLE);
	SetWindowLong(hWnd, GWL_STYLE, (dwStyle & ~dwRemove) | dwAdd);
	return dwStyle;
}