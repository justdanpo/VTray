#ifndef __TRAYICON_H
#define __TRAYICON_H

#include <windows.h>
#include <shlobj.h>
#include <time.h> // for timers
#include "../VTK/lsapi_location.h"
#include "../VTK/Icon.h"

typedef struct LSNOTIFYICONDATA
{
	DWORD cbSize;
	HWND hWnd;
	UINT uID;
	UINT uFlags;
	UINT uCallbackMessage;
	HICON hIcon;
	CHAR szTip[256];
	DWORD dwState;
} LSNOTIFYICONDATA, *PLSNOTIFYICONDATA;


// icon colorsettings
enum
{
	TINT_STANDARD,
	TINT_FLASHING
};

// icon visibility status
enum
{
	ICON_HIDDEN = 40,
	ICON_SHOWN,
	ICON_SCROLLED
};

enum // matches starting at 20
{
	MATCH_NONE = -1,
	MATCH_HIDE = 20,
	MATCH_FLASH
};

typedef struct ICON_SETTING
{
	int MatchHidePos; // position in config struct, also set if this icon shouldn't hide
	int MatchFlashPos; // -""-
	bool canHide;	// set when title is updated & matches defined one in config.
					// checked first in HideIconTimeout, which then checks update time.
} ICON_SETTING;

typedef struct ICON_STATE
{
	time_t updated;	// update time
	int FlashCount; // how many times icon has flashed
	int VisibilityStatus; // different status of visibility
	bool isFlashing; // if icon has been updated&matches flash config, is checked for in timer
} ICON_STATE;

class TrayIcon : public Icon
{
public:
	TrayIcon(PLSNOTIFYICONDATA pnid);
	~TrayIcon();

	void SetIcon(HICON icon);

	void SetTooltip(LPTSTR szNewTooltip);
	void UpdateTooltip(bool bCreate = false);
	void RemoveTooltip();
	bool Update(PLSNOTIFYICONDATA pnid);
	void SetFlashing(bool flashing);
	void LogInfo();

public:
	// tooltip created using singleton pattern (ie. with first icon)
	static HWND hTooltip;
	static UINT uTooltipIDCounter;

	// owner window
	HWND hOwnerWnd;
	// id
	UINT uID;
	// callback message ( to send to the owner window )
	UINT uCallbackMessage;
	// tooltip text
	LPTSTR szTip;
	// tooltip id
	UINT uToolTip;

	// for more advanced features
	struct ICON_STATE state;
	struct ICON_SETTING setting;
};

typedef TrayIcon SYSTRAYICON;
typedef TrayIcon* PSYSTRAYICON;


#endif