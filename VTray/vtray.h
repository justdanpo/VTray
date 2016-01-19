#ifndef  __VTRAY_H
#define  __VTRAY_H

// if using bitmap with transparent blt or "real" icon functions
#define ICONS_BITMAPPED

// registry saving
#define ICONS_REGISTRY


#include <time.h> // for timers
#include "../VTK/guiwindow.h"
#include "../VTK/Hover.h"
#include "../VTK/CountingBangRunner.h"
#include "../VTK/BoxPainter.h"
#include "../VTK/Clickable.h"
#include "TrayIcon.h"
#include "FlashSettingsList.h"
#include "HideSettingsList.h"

#include <list>
using std::list;

typedef list<TrayIcon*> IconList;


class vtray : public GuiWindow, Hover, Clickable
{
protected:
	// background handler/painter
	BoxPainter painter;

	// booleans
	bool hideIfEmpty;
	bool bWinNT5;
	bool bCheckOnCommands;
	bool bRememberIcons;

	// autosize
	bool autoSize;

	// dragging
	bool borderDrag;
	int snapDistance;

	// tray wrapping
	int direction;
	int wrapCount;
	int wrapDirection;
	bool bPaintNext;
	int nScrollStep;

	// borders
	int borderLeft;
	int borderTop;
	int borderRight;
	int borderBottom;

	// icon placement
	UINT iconSpacingX;
	UINT iconSpacingY;
	int deltaX;
	int deltaY;

	int firstX;
	int lastX;
	int firstY;
	int lastY;

	HWND liteStep;
	HWND desktop;
	HWND parentWindow;

	IconList normalIcons;
	IconList hiddenIcons;

	// onadd, wrap, etc commands
	CountingBangRunner commands;

	// hiding lists
	bool ClearIconsOnTimer;
	FlashSettingsList flashconfig;
	HideSettingsList hideconfig;
	UINT nHideTimer;
	UINT nScrollTimer;

	bool bScreenSaverRunning;
	bool bHover;

	bool FlashInSS;
	bool TrayFlash;
	bool scrollWas;
	bool ScrollFlashing;
	bool AlwaysFlash;
	int timerScroll;

	LPTSTR szSettingsPrefix;

	typedef BOOL (WINAPI* ASFWPROC)(DWORD);
    ASFWPROC m_pAllowSetForegroundWindow;

public:
	vtray(HWND parentWnd, int& code, bool inWharf);
	~vtray();

	// bang handlers
	void AcceptFlash(bool flash);
	void IconListInfo(void);
	void IconListShowAll(void);
	void IconListScroll(int Type);
	void IconListCleanup();
	HWND GetModuleWindow();
	bool IsDragPoint(int x, int y);


protected:
	// registry saving
#ifdef ICONS_REGISTRY
	bool SaveNewIcon(PLSNOTIFYICONDATA pnid);
	bool RestoreIcons();
	bool RemoveIcons();
#endif

	void AdjustSize();
	void IconListSetFirstLast();
	void IconAdjustRect(PSYSTRAYICON pSysTrayIcon);

	void DeleteTimers();
	void ReadSetTimers();
	void ReadCommands();
	void ReadOnAddCommand();
	void ReadOnDelCommand();
	void ReadSizeAndPos();
	void ReadSizeProps();
	void ReadGUIProps();
	void ReadWindowLists();
	void ReadIconHide(void);
	void ReadIconFlash(void);
	void ReadAddCommands(void);
	void ReadMouseCommands(void);
	void SetMouseCommand(const int ID, LPCTSTR command);

	void RunCommandAdd(void);
	void RunCommandDel(void);

	void ExportEvars();

	/* Icon List Handling */
	TrayIcon* IconListFind(HWND hWnd, UINT uID);
	TrayIcon* IconListFind(POINT pt);
	BOOL IconListDeleteAll();
	void IconListPaint(HDC hdcDest, HRGN hrgnDest, LPRECT updateRC = NULL);
	int IconListCount(int Type);
	void IconListFixPositions();

	/* Icon Handling */
	BOOL IconShownRemove(TrayIcon* pSysTrayIcon);
	PSYSTRAYICON IconAdd(PLSNOTIFYICONDATA pnid);
	BOOL IconDelete(PLSNOTIFYICONDATA pnid);
	BOOL IconDelete(TrayIcon* pSysTrayIcon);
	TrayIcon* IconModify(PLSNOTIFYICONDATA pnid);
	void IconShow(TrayIcon* icon, bool bFront = false);
	void IconHide(TrayIcon* icon, int status = ICON_HIDDEN);

	virtual void windowProc(Message& message);
	void onRefresh(Message& message);
	void onCreate(Message& message);
	void onDestroy(Message& message);
	void onDisplayChange(Message& message);
	void onEndSession(Message& message);
	void onGetRevId(Message& message);
	void onKeyMessage(Message& message);
	void onMouse(Message& message);
	BOOL onMouseIcon(Message& message);
	void onPaint(Message& message);
	void onSysCommand(Message& message);
	void onWindowActivated(Message& message);
	void onSysTray(Message& message);
	void onTimer(Message& message);
	void onWindowPosChanging(Message& message);

	void AllowSetForeground(HWND hWnd) const;
};

void BangIconInfo( HWND sender, LPCSTR args );
void BangShowAllIcons( HWND sender, LPCSTR args );
void BangHook( HWND sender, LPCSTR szArgs);
void BangHide( HWND sender, LPCSTR args );
void BangShow( HWND sender, LPCSTR args );
void BangToggle( HWND sender, LPCSTR args );
void BangSetOnTop( HWND sender, LPCSTR args );
void BangMove( HWND sender, LPCSTR args );
void BangMoveBy( HWND sender, LPCSTR args );
void BangSize( HWND sender, LPCSTR args );
void BangDisableFlash( HWND sender, LPCSTR args );
void BangEnableFlash( HWND sender, LPCSTR args );
void BangScrollIcons( HWND sender, LPCSTR args );
void BangIconCleaning( HWND sender, LPCSTR args );

extern "C"
{
	__declspec( dllexport ) int initModuleEx(HWND parent, HINSTANCE dll, LPCSTR szPath);
	__declspec( dllexport ) void quitModule(HINSTANCE dllInst);
	__declspec( dllexport ) int initWharfModule(HWND parent, HINSTANCE dll, void *wharfData);
	__declspec( dllexport ) void quitWharfModule(HINSTANCE dllInst);
}

#endif// __SYSTRAY
