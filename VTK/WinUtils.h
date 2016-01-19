#ifndef __WINUTILS_H
#define __WINUTILS_H

#include <windows.h>

namespace WinUtils
{
	// name of shelldesktop class
	#define WC_SHELLDESKTOP    "DesktopBackgroundClass"

	// defines for strange windows version
	#ifndef SPI_GETSCREENSAVERRUNNING
	#define SPI_GETSCREENSAVERRUNNING 114
	#endif

	void LogWindow(HWND window);
	HWND GetDesktopHWND(void);
	bool IsScreenSaverRunning(void);
	bool IsWinNT5(void);
	DWORD ModifyStyle(HWND hWnd, DWORD dwRemove, DWORD dwAdd);
}

#endif