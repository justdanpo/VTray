/*

	Hover

	Author:	Vendicator

	Description:
	Simple class that handles hovering over window

	Requires:
	- GetModuleWindow() implemented in child class

	Changelog:
	[2003-07-21 - Vendicator]
	- Switched to TCHAR and SafeString.
	- Renamed HWND getting function to GetModuleWindow().

	[2003-03-15 - Vendicator]
	- First work?

*/

#include "Hover.h"
#include "LSUtils.h"
#include "lsapi_location.h"
#include "apis.h"
using namespace LSUtils;

extern LPCSTR szLogName;

Hover::Hover(LPCTSTR prefix) :
	Prefix(prefix),
	bHover(false)
{
}

Hover::~Hover()
{
}

void Hover::ReadHoverSettings()
{
	TCHAR szTemp[MAX_LINE_LENGTH];
	memset(szTemp, 0, MAX_LINE_LENGTH);

	LSUtils::PrefixedGetRCLine(Prefix, "OnMouseEnter", szTemp, MAX_LINE_LENGTH, TEXT("!NONE"));
	OnMouseEnterCommand.assign(szTemp);

	LSUtils::PrefixedGetRCLine(Prefix, "OnMouseLeave", szTemp, MAX_LINE_LENGTH, TEXT("!NONE"));
	OnMouseLeaveCommand.assign(szTemp);
}

void Hover::SetMouseEnterCommand(SafeString enter)
{
	OnMouseEnterCommand = enter;
}

void Hover::SetMouseLeaveCommand(SafeString leave)
{
	OnMouseLeaveCommand = leave;
}

void Hover::PurgeHoverSettings()
{
}

void Hover::HoverCheck()
{
	RECT rc;
	POINT pt;
	GetCursorPos(&pt);
	GetWindowRect(GetModuleWindow(), &rc);

	if (PtInRect(&rc, pt))
		HoverExecute(HOVER_IN);
	else
		HoverExecute(HOVER_OUT);
}

void Hover::HoverCheck(POINT* point)
{
	RECT rc;
	GetWindowRect(GetModuleWindow(), &rc);

	if (PtInRect(&rc, *point))
		HoverExecute(HOVER_IN);
	else
		HoverExecute(HOVER_OUT);
}

void Hover::HoverExecute(const int ID)
{
	HWND hWnd = GetModuleWindow();
	switch(ID)
	{
		case HOVER_IN:
			if(GetCapture() != hWnd)
				SetCapture(hWnd);

			if (!bHover)
			{
				bHover = TRUE;
#ifdef _DEBUG
				LoggedLSExecute(szLogName, hWnd, OnMouseEnterCommand.char_str(), 0);
#else
				LSExecute(hWnd, OnMouseEnterCommand.char_str(), 0);
#endif
			}

		break;
		case HOVER_OUT:
			if (bHover)
			{
				bHover = FALSE;
				ReleaseCapture();

#ifdef _DEBUG
				LoggedLSExecute(szLogName, hWnd, OnMouseLeaveCommand.char_str(), 0);
#else
				LSExecute(hWnd, OnMouseLeaveCommand.char_str(), 0);
#endif
			}
		break;
	}
}

#ifdef FAKE_HWND
HWND Hover::GetModuleWindow()
{
	return NULL;
};
#endif