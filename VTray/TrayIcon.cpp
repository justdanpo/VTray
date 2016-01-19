#include "TrayIcon.h"
#include "../VTK/apis.h"

#include <tchar.h>
extern LPCSTR szLogName;


UINT TrayIcon::uTooltipIDCounter = 0;
HWND TrayIcon::hTooltip = NULL;

TrayIcon::TrayIcon(PLSNOTIFYICONDATA pnid) :
	hOwnerWnd(NULL),
	uID(0),
	uCallbackMessage(0),
	szTip(NULL),
	uToolTip(0)
{
	// transfer properties
	hOwnerWnd = pnid->hWnd;
	uID = pnid->uID;
	uCallbackMessage = ((pnid->uFlags & NIF_MESSAGE) ? pnid->uCallbackMessage : NULL);

	// no flash on start
	state.isFlashing = false;
	state.FlashCount = 0;
	state.VisibilityStatus = ICON_SHOWN;

	// set icon and region properties
	SetIcon((pnid->uFlags & NIF_ICON) ? pnid->hIcon : NULL);

	// add tooltip (uses rect)
	SetTooltip((pnid->uFlags & NIF_TIP) ? pnid->szTip : NULL);

	if (uIconCounter == 1)
	{
		// create a tooltip window, no parent from Jugg (hParentWindow)
		hTooltip = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, TTS_ALWAYSTIP | TTS_NOPREFIX | WS_POPUP, 0, 0, 0, 0, NULL, NULL, NULL, NULL);

		// allow multiline tooltip for DUN icon
		SendMessage( hTooltip, TTM_SETMAXTIPWIDTH, 0, 300 );
	}
}

TrayIcon::~TrayIcon()
{
#ifdef _DEBUG
	_LSLog(LOG_DEBUG, szLogName, "TrayIcon Desctructor");
#endif

	// remove the tooltip
	SetTooltip(NULL);

	// destroy the tooltip window
	if (uIconCounter <= 1 && hTooltip)
	{
		DestroyWindow( hTooltip );
		hTooltip = NULL;
	}
}

void TrayIcon::SetFlashing(bool flashing)
{
	if (state.isFlashing != flashing)
	{
		state.isFlashing = flashing;
		TrayIcon::SetIcon(hIcon);
		InvalidateRect(hParentWindow, &rc, FALSE);
		_LSLogPrintf(LOG_DEBUG, szLogName, "(%s) flash update to: %d", szTip, state.isFlashing);
	}
}

void TrayIcon::SetIcon(HICON icon)
{
	COLORREF useClrHue;
	UCHAR useHueIntensity;
	UCHAR useSaturation;

	// find default colors
	ColorSettings::iterator find = ColorTintings.find(TINT_STANDARD);
	if (find != ColorTintings.end())
	{
		useClrHue = (*find).second.clrHue;
		useHueIntensity = (*find).second.cHueIntensity;
		useSaturation = (*find).second.cSaturation;
	}

	if (state.isFlashing) // hue effect depending on flashing
	{
		find = ColorTintings.find(TINT_FLASHING);
		if (find != ColorTintings.end())
		{
			useClrHue = (*find).second.clrHue;
			useHueIntensity = (*find).second.cHueIntensity;
			useSaturation = (*find).second.cSaturation;
			_LSLogPrintf(LOG_DEBUG, szLogName, "(%s) flashing icon preset found", szTip);
		}
		else
			_LSLogPrintf(LOG_DEBUG, szLogName, "(%s) flashing icon preset not found!", szTip);
	}

	Icon::SetIcon(icon, useClrHue, useHueIntensity, useSaturation);
}

void TrayIcon::SetTooltip(LPTSTR szNewTooltip)
{
	BOOL existed = TRUE;
	TOOLINFO ti;

	ti.cbSize = sizeof(TOOLINFO);
	ti.hwnd	= hParentWindow;
	ti.hinst = NULL;

	if (!szTip)
	{
		existed = FALSE;
		uToolTip = ++TrayIcon::uTooltipIDCounter;
	}

	ti.uId = uToolTip;

	// delete old tip
	if (existed) {
		delete [] szTip;
		szTip = NULL;
	}

	// set the new tip
	if (szNewTooltip)
	{
		szTip = new TCHAR[ _tcslen(szNewTooltip) + 1];
		_tcscpy(szTip, szNewTooltip);
	}

	ti.lpszText = szTip;

	if ( existed )
	{
		if ( IsWindow(TrayIcon::hTooltip) )
		{
			SendMessage(TrayIcon::hTooltip, (szTip) ? TTM_UPDATETIPTEXT : TTM_DELTOOL, 0, (LPARAM)&ti);
		}
	}
	else if ( szTip && IsWindow(TrayIcon::hTooltip) )
	{
		ti.uFlags = 0;
		ti.rect = rc;
		SendMessage(TrayIcon::hTooltip, TTM_ADDTOOL, 0, (LPARAM)&ti);
	}
}

void TrayIcon::UpdateTooltip(bool bCreate)
{
	TOOLINFO ti;
	if (szTip)
	{
		ti.cbSize = sizeof(TOOLINFO);
		ti.hwnd = hParentWindow;
		ti.uId = uToolTip;
		ti.rect = rc;
		ti.uFlags = 0;
		if (IsWindow(hTooltip))
		{
			if (bCreate)
			{
				ti.lpszText = szTip;
				SendMessage(TrayIcon::hTooltip, TTM_ADDTOOL, 0, (LPARAM)&ti);
			}
			else
				SendMessage(hTooltip, TTM_NEWTOOLRECT, 0, (LPARAM)&ti);
			//SendMessage(tooltip, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
		}
	}
}

void TrayIcon::RemoveTooltip()
{
	TOOLINFO ti;
	ti.cbSize = sizeof(TOOLINFO);
	ti.uId = uToolTip;
	ti.hwnd	= hParentWindow;
	ti.hinst = NULL;
	if (IsWindow(TrayIcon::hTooltip))
	{
		SendMessage(TrayIcon::hTooltip, TTM_DELTOOL, 0, (LPARAM)&ti);
	}
}

bool TrayIcon::Update(PLSNOTIFYICONDATA pnid)
{
	bool bTitleUpdated = false;
	uID = pnid->uID;

	// only if this icon has a setting flash or hide, and the tip should be updated
	if (setting.MatchFlashPos != MATCH_NONE || setting.MatchHidePos != MATCH_NONE && pnid->uFlags & NIF_TIP)
		// icon considered updated if the tooltip has changed
		if ( (szTip == NULL && pnid->szTip != NULL) || strncmp(szTip, pnid->szTip, max(strlen(szTip), strlen(pnid->szTip))) != 0)
			bTitleUpdated = true;

	if (pnid->uFlags & NIF_MESSAGE)
		uCallbackMessage = pnid->uCallbackMessage;

	if (pnid->uFlags & NIF_TIP)
		SetTooltip(pnid->szTip);

	if (pnid->uFlags & NIF_ICON)
		SetIcon(pnid->hIcon);

	if (bTitleUpdated)
		state.updated = time(NULL); // update timepSysTrayIcon->state.updated = time(NULL); // update time

	return bTitleUpdated;
}

void TrayIcon::LogInfo()
{
	TCHAR classname[MAX_LINE_LENGTH] = TEXT("");

	// output window classnames
	GetClassName(hOwnerWnd, classname, MAX_LINE_LENGTH-1 ); // sizeof(classname)/sizeof(classname[0])-1
	_LSLogPrintf(LOG_NOTICE, szLogName, "Icon: \"%s\" (0x%X, %d), shown:%d, class: \"%s\"", szTip, hOwnerWnd, uID, state.VisibilityStatus, classname);
}
