/*

	GuiWindow

	Author:	Vendicator
	
	Description:
	This class encapsulates common gui functions that you
	otherwise just copy anyway. Note that colors/bitmaps isn't
	handled in here (yet).

	Requires:
	- WinUtils class.
	- apis.h, LSApi loader

	Changelog:
	[2004-03-06 - Vendicator]
	- Added move by.

	[2003-12-26 - Vendicator]
	- Added dragging.

	[2003-07-30 - Vendicator]
	- Sizing with 0 as a parameter keeps old dimension.
	- When hooking to lsbox the SetLsBoxed(true) has been added.

	[2003-07-25 - Vendicator]
	- Fixed SetPosition when wharf loading.

	[2003-07-09 - Vendicator]
	- Added some handling to move functions if the window has a parent (ie. lsbox),
	  still untested tho.

	[2003-07-09 - Vendicator]
	- Made Move() and Size() go through SetPosition() etc so the same code is used.
	- Made some changes to align code to prevent it running multiple times.

	[2003-06-27 - Vendicator]
	- Added FixVisibility() to be run after window has been created so start hidden works.

	[2003-06-12 - Vendicator]
	- Added CreateWnd() to set up the window after the class has been configured.

	[2002-12-02 - Vendicator]
	- First work?
	
*/

#include "GuiWindow.h"
#include "WinUtils.h"
#include "apis.h"

extern LPCSTR szLogName;


GuiWindow::GuiWindow(LPCSTR szAppName, HWND parentWnd, bool Wharfed) :
	Window(szAppName),
	nXPos(0),
	nYPos(0),
	nWidth(0),
	nHeight(0),
	bVisible(false),
	bLsBoxed(false),
	bInWharf(Wharfed),
	bOnTop(false),
	hwndParent(parentWnd),
	nScreenWidth(0),
	nScreenHeight(0),
	firstAlignDone(false)
{
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
}

GuiWindow::~GuiWindow()
{
}

HWND GuiWindow::GetParentHWND(void)
{
	return hwndParent;
}

HWND GuiWindow::GetModuleWindow(void)
{
	return hWnd;
}

void GuiWindow::SetTop(bool ontop)
{
	bOnTop = ontop;
}
bool GuiWindow::IsOnTop(void)
{
	return bOnTop;
}

void GuiWindow::SetDraggable(bool draggable)
{
	bDraggable = draggable;
}
bool GuiWindow::IsDraggable(void)
{
	return bDraggable;
}

void GuiWindow::SetVisible(bool visible)
{
	_LSLogPrintf(LOG_DEBUG, "GuiWindow", "set visible: %d", visible);
	bVisible = visible;
}
bool GuiWindow::IsVisible(void)
{
	return bVisible;
	//return (bool) IsWindowVisible(hWnd);
}

void GuiWindow::SetInWharf(bool wharfed)
{
	bInWharf = wharfed;
}
bool GuiWindow::IsInWharf(void)
{
	return bInWharf;
}

void GuiWindow::SetLsBoxed(bool lsboxed)
{
	bLsBoxed = lsboxed;
}
bool GuiWindow::IsLsBoxed(void)
{
	return bLsBoxed;
}

int GuiWindow::GetX(void)
{
	return nXPos;
}

int GuiWindow::GetY(void)
{
	return nYPos;
}

int GuiWindow::GetWidth(void)
{
	return nWidth;
}

int GuiWindow::GetHeight(void)
{
	return nHeight;
}

void GuiWindow::SetSize(int x_size, int y_size)
{
	nWidth = x_size;
	nHeight = y_size;
}

void GuiWindow::SetPosition(int x_pos, int y_pos)
{
	int maxX = 0, maxY = 0;

#ifdef _DEBUG
	int x_org = x_pos;
	int y_org = y_pos;
#endif

	// special handling if wharfed/lsboxed?
	/*if (bLsBoxed || bInWharf)
	{
		RECT rc;
		GetClientRect( GetParent(hWnd), &rc );
		maxX = rc.right - rc.left;
		maxY = rc.bottom - rc.top;
	}
	else
	{*/
		maxX = GetSystemMetrics(SM_CXSCREEN);
		maxY = GetSystemMetrics(SM_CYSCREEN);
	//}

	// supports lsbox?
	if (x_pos < 0)
		x_pos += maxX;
	if (y_pos < 0)
		y_pos += maxY;

	// aligning
	if (!firstAlignDone && !bInWharf)
	{
		firstAlignDone = true;
		if (nAlign & ALIGN_RIGHT)
			x_pos -= (nWidth);
		if (nAlign & ALIGN_DOWN)
			y_pos -= (nHeight);
	}

	if (bLsBoxed) // bInWharf
	{
		// compensate for position of the lsbox, works when placed on desktop too..
		RECT rcOrg;
		HWND boxwnd = GetParent(hWnd);
		GetWindowRect(boxwnd, &rcOrg);
		x_pos -= rcOrg.left;
		y_pos -= rcOrg.top;
	}

	// save class variables
	nXPos = x_pos;
	nYPos = y_pos;
	
#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "Position set to %d,%d originally: %d,%d", nXPos, nYPos, x_org, y_org);
#endif

}

bool GuiWindow::IsDragPoint(int x, int y)
{
	return true;
}

void GuiWindow::SetAlignment(LPCSTR align)
{
	nAlign = ALIGN_OFF;

	if ( strstr(align, "left") )
	{
		nAlign |= ALIGN_LEFT;
	}
	else if ( strstr(align, "right") )
	{
		nAlign |= ALIGN_RIGHT;
	}

	if ( strstr(align, "top") )
	{
		nAlign |= ALIGN_UP;
	}
	else if ( strstr(align, "bottom") )
	{
		nAlign |= ALIGN_DOWN;
	}
}

void GuiWindow::Size(int width, int height)
{
	SetSize( (width > 0 ? width : nWidth), (height > 0 ? height : nHeight));
	SetWindowPos(hWnd, NULL, 0, 0, nWidth, nHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void GuiWindow::Move(int x_pos, int y_pos)
{
	SetPosition(x_pos, y_pos);

#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "new pos: x:%d,y:%d", x_pos, y_pos);
#endif
	SetWindowPos(hWnd, NULL, nXPos, nYPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void GuiWindow::MoveBy(int x_pos, int y_pos)
{
	RECT orgRc;
	GetWindowRect(hWnd, &orgRc);

	Move(orgRc.left+x_pos, orgRc.top+y_pos);
}

void GuiWindow::MoveAndSize(int x_pos, int y_pos, int width, int height)
{
	SetPosition(x_pos, y_pos);
	SetSize( (width > 0 ? width : nWidth), (height > 0 ? height : nHeight));
	SetWindowPos(hWnd, NULL, nXPos, nYPos, nWidth, nHeight, SWP_NOZORDER | SWP_NOACTIVATE);
}

void GuiWindow::FixVisibility()
{
	if (bVisible)
		Show();
	else
		Hide();
}

void GuiWindow::Show(void)
{
	bVisible = true;
	ShowWindow(hWnd, SW_SHOWNOACTIVATE);
}

void GuiWindow::Hide(void)
{
	bVisible = false;
	ShowWindow(hWnd, SW_HIDE);
}

void GuiWindow::ToggleVisibility(void)
{
	if (bVisible)
		Hide();
	else
		Show();
}

void GuiWindow::BoxHook(LPCSTR szArgs)
{
#ifdef _DEBUG
	_LSLog(LOG_DEBUG, szLogName, "Attempting to hook to LSBox");
#endif

	char* handle = strrchr(szArgs,' ');
	if (handle)
	{
		HWND boxwnd = (HWND)atoi(handle+1);
		if (boxwnd)
		{
			bLsBoxed = true;
			if (boxwnd != GetParent(hWnd))
			{
				SetWindowLong(hWnd, GWL_STYLE, (GetWindowLong(hWnd, GWL_STYLE) &~ WS_POPUP)|WS_CHILD);
				::SetParent(hWnd, boxwnd);
				hwndParent = boxwnd;
				SetLsBoxed(true);
				// set new pos using getrccordinate again with boxwnd size?
			}

#ifdef _DEBUG
			_LSLog(LOG_DEBUG, szLogName, "Hooking to LSBox successful");
#endif

		}
		else
		{
			_LSLog(LOG_WARNING, szLogName, "Hooking to LSBox failed");
			SetLsBoxed(false);
		}
	}
}

void GuiWindow::FixPosition()
{
	// tries to fix position after ie a display change (game)
	Move(nXPos, nYPos);
	Size(nWidth, nHeight);
	SetOnTop(bOnTop);

	if (bVisible)
		ShowWindow(hWnd, SW_SHOWNOACTIVATE);
	else
		ShowWindow(hWnd, SW_HIDE);
}

void GuiWindow::SetOnTop(bool fAlwaysOnTop)
{
    if (!bLsBoxed && !bInWharf)
	{
		WinUtils::ModifyStyle(hWnd, WS_POPUP, WS_CHILD);
		SetParent(hWnd, fAlwaysOnTop ? NULL : WinUtils::GetDesktopHWND());
		WinUtils::ModifyStyle(hWnd, WS_CHILD, WS_POPUP);
	}

    SetWindowPos(hWnd, fAlwaysOnTop ? HWND_TOPMOST : HWND_NOTOPMOST,
        0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

	bOnTop = fAlwaysOnTop;
}

void GuiWindow::ToggleOnTop(void)
{
	bOnTop ? SetOnTop(false) : SetOnTop(true);
}

/*
	Wrapper around the standard ls createWindow to get the parent
	and style right for ontop/desktop etc.
*/
BOOL GuiWindow::CreateWnd(LPCSTR szWCName)
{
	HWND desktop = WinUtils::GetDesktopHWND();
	HWND liteStep = GetLitestepWnd();
	BOOL ok;

	_LSLogPrintf(LOG_DEBUG, szLogName, "ontop: %d, visible: %d, wharf: %d, boxed: %d", bOnTop, bVisible, bInWharf, bLsBoxed);

	ok = createWindow(WS_EX_TOOLWINDOW, szWCName, bInWharf ? WS_CHILD : WS_POPUP,
		nXPos, nYPos, nWidth, nHeight, bInWharf ? hwndParent : (bOnTop ? NULL : desktop));

	FixVisibility();

	return ok;
}