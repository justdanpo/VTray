/*

- To do:
  - Protection of weird char in *On command (ex: <7)
  - Check per pixel alpha support
  - Use getrccoordinate in lsbox hook and use lsbox size for maxsize positioning?
  - Use real icon routines to avoid magic pink with smooth icons, test:
	why is creating icons from hbmBuffer making the icons display in half size (16->8), while outline is correct size?

Changelog:
- Added [Vtray / Vendicator / 2004-03-06, release 1.10]
  - Added ![Tray]MoveBy bang.
  - Renamed [Tray]CheckOnCommands to [Tray]DoNotCheckOnCommands so it can be on by default,
    makes scripting easier.

- Added [VTray / Vendicator / 2003-12-26]
  - Added configurable mouse commands, including the special command .drag, which starts to
    drag the tray window in to another position, only to be used with middle/left/right button down.

- Added [VTray / Vendicator / 2003-12-14]
  - Added an experimental session based icon saving to registry to prevent loosing icons
    on ls crash/quit, graphics can not be saved though so only blanks will be restored,
	disabled by default, check [Tray]RememberIcons.
  - Moved adding of bang commands until window is created.

- Fixed [VTray / Vendicator / 2003-10-19]
  - Fixed *On commands crash (misnumbered arrays)
  - "Stole" some changes from systray2 (thanks ilmcuts) including the foreground issue of winamp and wharfborder def. value

- Fixed [VTray / Vendicator / 2003-08-09, release 1.06]
  - Fixed MultipleConfigReader, which I had previously broken causing hide/flash/on to not work...

- Changed [VTray / Vendicator / 2003-08-09, release 1.05]
  - Creating real tinted icon and using windows icon drawing functions.

- Fixed [VTray / Vendicator / 2003-08-01]
  - Fixed a scrolling issue when the size of the tray was enough to fit all icons.

- Fixed [VTray / Vendicator / 2003-08-01]
  - Fixed error in detecting "prev" argument.
  - Debug builds autosets LSLogLevel to 4.

- Added [VTray / Vendicator / 2003-07-30, release 1.04]
  - Added [Tray]IconScrollStep, to define how many icons to move per scroll.
  - Fixed reverse order of icons after a scroll "next" then "back".
  - Fixed possible GDI leak and a tooltip problem.
  - Fixed the icon counting/tooltip deletion, fixing last gdi leak.

- Fixed [VTray / Vendicator / 2003-07-28, release 1.03]
  - Jugg sent in a fix to get tooltips above all windows.

- Fixed [VTray / Vendicator / 2003-07-26, release 1.02]
  - Fixed starting auto scroll value.
  - Scrolling to next cannot hide all icons, nothing will happen on last line.
  - Using strstr to do a substring search on scroll icon arguments, matching "next" as well as next.

- Fixed [VTray / Vendicator / 2003-07-25, release 1.01]
  - Fixed wharf loading.
  - Default wrap count is auto calculated from window size when wharfed.

- Fixed [VTray / Vendicator / 2003-07-24]
  - Fixed !VTrayScrollIcons, use with small fixed tray windows.
  - Fixed special flash tinting, default flash tint to normal tint.
  - Added !VTrayIconCleaning

- Changed [VTray / Vendicator / 2003-07-21]
  - Using Hover class for mouse overs.
  - Updated docs.

- Changed [VTray / Vendicator / 2003-07-19]
  - Moved bitmap/color skinning out to separate class.

- Changed [VTray / Vendicator / 2003-07-16]
  - Reading settings through prefixed settings reader to allow for settings compatibility.
  - Broke TrayIcon class down in to a general, reusable Icon class and built TrayIcon on top.

- Changed [VTray / Vendicator / 2003-07-15]
  - Finished TrayIcon class.
  - Renamed some functions in vtray to have more consistent naming.
  - Added #pragma's to get rid of all those annoying C4786 warnings from STL.
  - Trying to reduce painting by checking update rect when painting icons, only invalidating modified icons.
  - Increased icon hiding resolution to seconds.

- Changed [VTray / Vendicator / 2003-07-14]
  - Moved icon handling functions to make TrayIcon a self contained class.
  - Made the tooltip work as a singleton
  - Deleted when hiding icon to prevent it from accidentally getting passed to another icon.

- Changed [VTray / Vendicator / 2003-07-12]
  - Fixed some copy pasate errors regarding hidden/shown list.
  - Changed show/hide to only include status when scrolling.
  - Deleting/Recreating tooltips when hiding to try to remove bug with hidden
    icons tooltip carrying over to another icon.

- Changed [VTray / Vendicator / 2003-07-11]
  - Switched to stl::list for icon storage

- Changed [VTray / Vendicator / 2003-07-10]
  - Using CountingBangRunner to store and use on add/del commands

- Changed [VTray / Vendicator / 2003-07-09]
  - Using GuiWindow class for handling of common functions.
  - Using MultipleConfigReader for hide/flash reading, separate storage class for each.

[VTray branch started / 2003-07-09]

- Fixed [systray2 / Vendicator / 2003-06-09]
  - Removed an unnecessary pointer from flashing/hiding icons configs

- Fixed [systray2 / ilmcuts / 2003-05-13]
  - Fixed alwaysontop state.

- Fixed [systray2 / Vendicator / 2003-04-05]
  - Added extra checks that NIF_TIP is set on icons in hide/flash checks

- Fixed [systray2 / Vendicator / 2003-02-03]
  - Made LS the parent window on ontop configs to remove it from tasklists

- Fixed [systray2 / Vendicator / 2003-02-01]
  - Fixed accidentally running the add command twice when an icon is added (thanks psynapse)

- Fixed [systray2 / Vendicator / 2003-01-31]
  - Invalidating whole window on icon update again, forgot that the bg needed painting ;)
  - Fix a missing endpaint() in the iconbuffer creation

- Changed [systray2 / Vendicator / 2003-01-22]
  - Added a bitmap buffer for each icon that is used when tinting is on
  - Moved tinting to set icon, so the tinted icon is only updated when icon is,
    not on all redraws like before, reduces cpu usage with tinting on quite a lot.

- Added [systray2 / Vendicator / 2003-01-18]
  - Added setting SystrayCheckOnCommands to check the oncommands if using dynamic vars
  - Fixed so that the evars are exported last now (found that it did the previous size before)
  - Changed so that systray2 isn't created as child to desktop when ontop

- Fixed [systray2 / Vendicator / 2002-12-20]
  - Added check for autoadjust with timer cleaning

- Fixed [systray2 / Vendicator / 2002-11-14]
  - Found a gdi leak from not removing a test region when checking transp.

- Fixed [systray2 / Vendicator / 2002-09-15]
  - Fixed autosizing always happening in !refresh

- Fixed [systray2 / Ilmcuts / 2002-09-08]
  - OnNumList2, ignoreTag, and flashTag weren't initialized to NULL in the constructor.

- Changed [systray2 / Vendicator / 2002-08-26]
  - Removed internal icons counter and using a function check instead for !bang commands
  - Exported the following evars:
  $SystrayCurrentIcons$, $SystrayCurrentX$, $SystrayCurrentY$, $SystrayCurrentHeight$, $SystrayCurrentWidth$

- Fixed [systray2 / Vendicator / 2002-08-17]
  - Fixed update check to only use the title and not icon change, will allow for
    hiding of zonealarm for instance
  - Jesus_mjjg's fix temporarily disabled until the reason why systray also sets
    it self above all windows is determined

- Fixed [systray2 / Vendicator / 2002-08-12]
  - Fixed a small mem leak (2bytes per flash config read)
  - Merged icon removal between hidden&shown icons
  - Tweaked more code to save some cycles here and there

- Fixed [systray2 / Jesus_mjjg / 2002-07-26]
  - Shouldn't tooltip windows be always on top of any other window ?
    now they are (but maybe i could make that cleaner)

- Added [systray2 / Vendicator / 2002-07-18]
  - Added setting "SystrayAlwaysFlashOnMatch", which continues to flash an icon even when
    icon is activated, this means that it will only stop when the match of the icon stops
	(ie icon removed, or tooltip doesn't match no more). If used with no tooltip and a
	flash limit, it the icon will only be able to flash once (since that is always matched).

- Fixed [systray2 / Vendicator / 2002-07-09]
  - Added an extra #define so that whichever strange os/msvc version can compile this again.

- Changed [systray2 / Vendicator / 2002-07-08]
  - Changed flash format to:
	*systrayflash "classname" "title (optional)" flashes/update
	to be able to specify max flashes, 0 to flash until you click the icon.
	This means that the icon won't flash anymore until you activate it (even if it's updated again).
  - Added setting "SystrayFlashInScreensaver", to specify if flashing should
    occur even when a screensaver is running, the flashing will most likely turn off the screensaver.

- Fixed [systray2 / Vendicator / 2002-07-02]
  - Rewrote the adjustsize algorithm, previous seemed to fail sometimes when hiding icons
  - Moved more code around, most settings read through functions, makes editing startup behaviour easier (!refresh, !recycle, startup)
  - Fixed !refresh some more... sometime tray isn't visible in lsbox, when does this happen?
  - Fixed a bug in the move code
  - Added systrayAlign which accepts a string like "right bottom" to define from which corner
    systray should be placed, defined size might give strange results.

- Added [systray2 / Vendicator / 2002-07-01]
  - Added output of window classes to log file (lv3) on !systrayinfo for easier config of hiding/flashing
  - Added scroll flashing on icon update through code from taskbar3,
    needs window classname, optional tooltip text (case sensitive, check log file).
  - Added mouse enter / leave !bang running.
  - Restructured how checks flash&hide checks are made, saves some cycles.
  - Fixed *Wharfmodule from LSBox, thanks to someone who I lost the mail from for pointing it out.
  - Started implementation of !refresh... still a little flaky
  - Ilmcuts noticed that the previous hue code used BGR format, changed to RGB even though a warning
    from grd was present in the code, but it's checked and doesn't appear to have any problems.

- Fixed [systray2 / Vendicator / 2002-06-17]
  - On recycle the remove commands were removed before run, fixed.

- Fixed [systray2 / Sci / 2002-06-16]
  - Stole a one-liner from the geOShell tray service, the right click menu is
    now closed when it loses focus!

- Updated [systray2 / Vendicator / 2002-06-10]
  - Did some code cleanup & added some more logging with #ifdef
  - Fixed saturation misspelling =)

- Updated [systray2 / ilmcuts / 2002-06-09]
  - the minimum value for systrayIconSize is now 1 instead of 0
  - fixed warnings about signed/unsigned int conflicts
  - tweaked wharf/lsbox compatibility (hopefully positive)
  - misc tweaks (thanks to Maduin for the new ontop code)

- Fixed [systray2 / Vendicator / 2002-06-08]
  - Fixed wierd systray placement bug that affected some ppl.

- Added [systray2 / Vendicator 2002-06-06]
  - Added grdTrays icon effects, hue & saturation (grdtray never wanted to work on my comp)
    Settings:systrayIconHueIntensity systrayIconHueColor systrayIconSaturation.

- Fixed [systray2 / Vendicator / MickeM / 2002-05-31]
  - MickeM started fixing some memory leaks and I continued..
  - Icon was set to NULL on update before it tried to delete it... fixed

- Added [systray2 / Vendicator / 2002-05-30]
  - Checks systray size in timer when "SystrayTimerCleaning" is used.
  - Killed timer too
  - Added !SystrayIconInfo, mainly for debugging purposes.
  - Moved and grouped functions better in the source =)

- Fixed [systray2 / Vendicator / 2002-05-29]
  - Fixed b0rked lsbox support, thanks to Maestr0 for pointing it out.

- Added [systray2 / Vendicator / 2002-05-28]
  - Added cleaning of crashed programs on timer, use "SystrayTimerCleaning".
  - Fixed a routine which didn't handle hidden icons before.
  - Added log of deleted icons

- Fixed [systray2 / Sci / 2002-05-22]
  - Multiline tooltips now works (DUN icon)

- Added [systray2 / Vendicator / 2002-05-17]
  - Implemented icon hiding base on window class name.
    Format: *systrayhide Window_Class_Name min_inactive_before_hiding
  - Added SystrayIconTimer, which set how often to do the check for inactive icons
    defaults to every minute, setting 0 disables check.
  - Added !SystrayShowIcons to display any hidden icons.
  - Added some logging, watch out on loglevel 4 =)
  - Rewrote the systray2kad stuff to skip vector notation...
  - Todo: Testing

- Problem [systray2 / Vendicator / 2002-05-06]
  - Works in lsbox and on desktop, but I'm guessing not in a wharf...

- Fixed [systray2 / Vendicator / 2002-05-05]
  - Added compensation for lsbox position, needed when using autoresize in lsbox.
  - Entered the systray2kad hacks for command execution on add/del of icons.

- Fixed [systray2 / Vendicator / 2002-05-02]
  - Added lsbox support, seems to hide if the tray resizes, watch of with autoresize...

- Fixed [systray2 / Vendicator / 2002-04-15]
  - Now using GetRCCoordinates to get X,Y position

- Fixed [systray2 / Message / 2001-11-26]
  - Stripped everything out of it except for basic gui, this modulewill not run
    without a new build

- Fixed [systray2 / Message / 2001-11-21]
  - Added Windows2000 and WindowxXP support. Seems to work fine when first loaded,
    but after a recycle w2k loses its volume icon and xp loses all icons (?). A simple fix
    for w2k is to run trayfix.exe after a recycle, and that will put the volume icon back.

    NOTES:
    The DUN icon uses 4 icons to display status, when NIM_ADD the icons, they all have the
    same HWND, but differ in NOTIFYICONDATA.uID. To get around this, we only check to see
    if an icon already exists by HWND only, and not uID. On NIM_MODIFY, we set the uID
    of any found icon to the modifying icon's uID or problems occur with the mouse handling.

    UnloadShellServiceObjects needs to be called before LM_SAVESYSTRAY is handled or
    the DUN/Volume icons get saved also and won't be handled properly when the tray is
    restarted after a recycle.

    (The tooltip on the DUN icon still doesn't handle well. I think this is our implementation
    and not MS related.) - Fixed by Sci
****************************************************************************/

#include "vtray.h"
#include "../VTK/apis.h"
#include "../VTK/BangManager.h"
#include "../VTK/MultipleConfigReader.h"
#include "../VTK/TokenSafeString.h"
#include "../VTK/WinUtils.h"
#include "../VTK/DragAction.h"
#include "Defines.h"

#include "../VTK/LSUtils.h"
using namespace LSUtils;

// defined directions
#define TD_VERTICAL		0x2
#define TD_TOTOPLEFT	0x4
// for alignment, default value:
#define TD_OFF			0x0

#define TD_RIGHT		0x1
#define TD_LEFT			(TD_TOTOPLEFT)
#define TD_UP			(TD_TOTOPLEFT|TD_VERTICAL)
#define TD_DOWN			(TD_VERTICAL)

enum // timers starting at 1
{
	TIMER_AUTOHIDE = 1,
	TIMER_SCROLL
};

enum // icons starting at 10
{
	COUNT_VISIBLE = 10,
	COUNT_NOTVISIBLE, // = scrolled + hidden
	COUNT_SCROLLED,
	COUNT_HIDDEN,
	COUNT_ALL,
	COUNT_FLASHING,
	COUNT_ROOMFOR,
	COUNT_PERWRAP
};

enum // icon scrolling
{
	SCROLL_BACK = 30,
	SCROLL_NEXT,
	SCROLL_AUTO
};

LPCTSTR szAppName = "VTray"; // Our window class, etc
LPCTSTR szLogName = "VTray";

LPCTSTR szVtraySetting = "VTray";
LPCTSTR szCompatSetting = "Systray";

const char rcsRevision[] = "1.10$"; // Our Version*/
const char rcsId[] = "VTray.cpp,v 1.10$";

const Bang CommonBangList[] =
{
	{ "IconInfo", BangIconInfo },
	{ "ShowIcons", BangShowAllIcons },
	{ "DisableFlash", BangDisableFlash },
	{ "EnableFlash", BangEnableFlash },
	{ "ScrollIcons", BangScrollIcons },
	{ "IconCleaning", BangIconCleaning }
};
// calculate how many bangs there are
const int nCommonBangs = sizeof(CommonBangList) / sizeof(CommonBangList[0]);

const Bang GuiBangList[] =
{
	{ "Hide", BangHide },
	{ "Show", BangShow },
	{ "Toggle", BangToggle },
	{ "OnTop", BangSetOnTop },
	{ "Move", BangMove },
	{ "MoveBy", BangMoveBy },
	{ "Size", BangSize },
	{ "Hook", BangHook }
};
// calculate how many bangs there are
const int nGuiBangs = sizeof(GuiBangList) / sizeof(GuiBangList[0]);

vtray *tray; // The module

//=========================================================
// Initialization and cleanup
//=========================================================

int initModuleEx(HWND parentWnd, HINSTANCE dllInst, LPCSTR szPath)
{
	int code;
	Window::init(dllInst);
	tray = new vtray(GetLitestepWnd(), code, false);
	return code;
}
int initWharfModule(HWND parentWnd, HINSTANCE dllInst, void *wharfData)
{
	int code;
	Window::init(dllInst);
	tray = new vtray(parentWnd, code, true);
	return code;
}
void quitModule(HINSTANCE dllInst)
{
	delete tray;
}
void quitWharfModule(HINSTANCE dllInst)
{
	delete tray;
}

/***************************/
/* !BANG COMMAND FUNCTIONS */
/***************************/

// cut & paste module hook..
void BangHook(HWND caller, LPCSTR szArgs)
{
	tray->BoxHook(szArgs);
}

void BangIconInfo( HWND sender, const char *args )
{
	tray->IconListInfo();
}

void BangIconCleaning( HWND sender, const char *args )
{
	tray->IconListCleanup();
}

void BangShow( HWND sender, const char *args )
{
	tray->Show();
}
void BangHide( HWND sender, const char *args )
{
	tray->Hide();
}
void BangToggle( HWND sender, const char *args )
{
	tray->ToggleVisibility();
}
void BangSetOnTop( HWND sender, const char *args )
{
	if (stricmp(args, "off") == 0)
		tray->SetOnTop(false);
	else
		tray->SetOnTop(true);
}
void BangMove( HWND sender, const char *args )
{
	char szX[MAX_LINE_LENGTH], szY[MAX_LINE_LENGTH];
	char *tokens[2] = {szX, szY};
	LCTokenize( args, tokens, 2, NULL );
	tray->Move(atoi(szX), atoi(szY));
}
void BangMoveBy( HWND sender, const char *args )
{
	char szX[MAX_LINE_LENGTH], szY[MAX_LINE_LENGTH];
	char *tokens[2] = {szX, szY};
	LCTokenize( args, tokens, 2, NULL );
	tray->MoveBy(atoi(szX), atoi(szY));
}
void BangSize( HWND sender, const char *args )
{
	char szCX[MAX_LINE_LENGTH], szCY[MAX_LINE_LENGTH];
	char *tokens[2] = {szCX, szCY};
	LCTokenize( args, tokens, 2, NULL );
	tray->Size(atoi(szCX), atoi(szCY));
}

void BangShowAllIcons( HWND sender, const char *args )
{
	tray->IconListShowAll();
}

void BangDisableFlash( HWND sender, const char *args )
{
	tray->AcceptFlash(FALSE);
}

void BangEnableFlash( HWND sender, const char *args )
{
	tray->AcceptFlash(TRUE);
}

void BangScrollIcons( HWND sender, const char *args )
{
#ifdef _DEBUG
	_LSLogPrintf(LOG_WARNING, szLogName, "Scroll argument: %s (%d)", args, args);
#endif
	if ( strstr(args, "next") )
		tray->IconListScroll(SCROLL_NEXT);
	else if (strstr(args, "back") || strstr(args, "prev"))
		tray->IconListScroll(SCROLL_BACK);
	else
	{
#ifdef _DEBUG
		_LSLogPrintf(LOG_WARNING, szLogName, "Auto scrolling on argument: %s", args);
#endif
		tray->IconListScroll(SCROLL_AUTO);
	}
}


//=========================================================
// Module code
//=========================================================

//---------------------------------------------------------
// Module constructor
//---------------------------------------------------------
vtray::vtray(HWND parentWnd, int& code, bool inWharf) :
		GuiWindow(szAppName, parentWnd, inWharf),
		scrollWas(GetKeyState(VK_SCROLL) != 0),
		bHover(false),
		bPaintNext(true),
		bScreenSaverRunning(FALSE),
		desktop(NULL),
		parentWindow(parentWnd),
		bCheckOnCommands(false),
		bRememberIcons(false),
		nScrollStep(0)
{
	InitAPIs();

#ifdef _DEBUG
	_LSLogPrintf(LOG_WARNING, szLogName, "*Debug* Version info: %s, %s, %s %s", rcsRevision, rcsId, __DATE__, __TIME__);
	char log[256];
	LSGetVariable( "LSLogLevel", log );
	_LSLogPrintf(LOG_WARNING, szLogName, "Log Level at %s", log);
	itoa(4, log, 10);
	_LSSetVariable( "LSLogLevel", log );
	LSGetVariable( "LSLogLevel", log );
	_LSLogPrintf(LOG_WARNING, szLogName, "New Log Level at %s", log);
#else
	_LSLogPrintf(LOG_DEBUG, szLogName, "Version info: %s, %s, %s %s", rcsRevision, rcsId, __DATE__, __TIME__);
#endif

	// get ls window handle
	liteStep = GetLitestepWnd();
	if (!liteStep) {
		_LSLog(LOG_ERROR, szLogName, "Could not find litestep window, aborting");
		code = 2;
		return;
	}

	desktop = WinUtils::GetDesktopHWND();

	if (PrefixedGetRCBool(szVtraySetting, TEXT("SettingsCompatibility"), TRUE))
		szSettingsPrefix = (LPTSTR) szCompatSetting;
	else
		szSettingsPrefix = (LPTSTR) szVtraySetting;

	// get the position and size of the main window
	// using different method if it's docked in the wharf
	ReadSizeAndPos();

	// FIXME: this logging stuff might need an update
	_LSLogPrintf(LOG_DEBUG, szLogName, "Creating Window, pos:%d,%d size:%d,%d, p:%d, l:%d, d:%d, ontop: %d", nXPos, nYPos, nWidth, nHeight, parentWindow, liteStep, desktop, bOnTop);

	if ( !CreateWnd(NULL) )
	{
		_LSLog(LOG_ERROR, szLogName, "unable to create window");

		MessageBox(NULL, TEXT("Unable to create window."), szAppName, MB_TOPMOST);
		code = 1;
		return;
	}
	else
	{
		_LSLogPrintf(LOG_DEBUG, szLogName, "window created, 0x%X", hWnd);
	}

#ifdef ICONS_REGISTRY
	bRememberIcons = PrefixedGetRCBool(szSettingsPrefix, TEXT("RememberIcons"), TRUE);

	// restore icons saved to registry
	if (bRememberIcons)
		RestoreIcons();
#endif

	// register bangs
	_LSLog(LOG_DEBUG, szLogName, "Registering Tray Bangs");
	PrefixedInsertBangs(szSettingsPrefix, CommonBangList, nCommonBangs);
	if ( !bInWharf )
	{
		// NOT FOR WHARF
		PrefixedInsertBangs(szSettingsPrefix, GuiBangList, nGuiBangs);
	}
	_LSLog(LOG_DEBUG, szLogName, "Done Tray Bangs");

	PostMessage(liteStep, LM_SYSTRAYREADY, NULL, NULL);
	_LSLog(LOG_NOTICE, szLogName, "loaded successfully");

	OSVERSIONINFO OSVersionInfo;
	OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVersionInfo);
	GetVersionEx(&OSVersionInfo);
	bWinNT5 = OSVersionInfo.dwMajorVersion >= 5 && OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT;

	m_pAllowSetForegroundWindow = (ASFWPROC)GetProcAddress(
        GetModuleHandle("USER32.DLL"), "AllowSetForegroundWindow");

	code = 0;
}

vtray::~vtray()
{
	_LSLog(LOG_DEBUG, szLogName, "Removing Tray Bangs");
	PrefixedRemoveBangs(szSettingsPrefix, CommonBangList, nCommonBangs);
	if ( !bInWharf )
	{
		// NOT FOR WHARF
		PrefixedRemoveBangs(szSettingsPrefix, GuiBangList, nGuiBangs);
	}
	_LSLog(LOG_DEBUG, szLogName, "Done Tray Bangs");

	destroyWindow();

	// remove commands after window is destroyed and icons removed
	_LSLog(LOG_NOTICE, szLogName, "shut down");
}

// needed for hover implementation
HWND vtray::GetModuleWindow()
{
	return hWnd;
}

// check if drag
bool vtray::IsDragPoint(int x, int y)
{
	if ( (x < borderLeft) || (x >= nWidth - borderRight) ||
		(y < borderTop) || (y >= nHeight - borderBottom) )
		return true;
	else
		return false;
}

/***************************/
/**** MESSAGE HANDLING *****/
/***************************/

void vtray::windowProc(Message& message)
{
#ifdef DEBUG_SPAM
	_LSLogPrintf(LOG_DEBUG, szLogName, "Message: 0x%X", message.uMsg) ;
#endif

	BEGIN_MESSAGEPROC
	MESSAGE(onCreate, WM_CREATE)
	MESSAGE(onDestroy, WM_DESTROY)
	MESSAGE(onEndSession, WM_ENDSESSION)
	MESSAGE(onEndSession, WM_QUERYENDSESSION)
	MESSAGE(onGetRevId, LM_GETREVID)
	MESSAGE(onSysTray, LM_SYSTRAY)
	MESSAGE(onRefresh, LM_REFRESH)
	MESSAGE(onWindowPosChanging, WM_WINDOWPOSCHANGING)
	MESSAGE(onKeyMessage, WM_KEYDOWN)
	MESSAGE(onKeyMessage, WM_KEYUP)
	MESSAGE(onKeyMessage, WM_HOTKEY)
	MESSAGE(onMouse, WM_LBUTTONDOWN)
	MESSAGE(onMouse, WM_MBUTTONDOWN)
	MESSAGE(onMouse, WM_RBUTTONDOWN)
	MESSAGE(onMouse, WM_LBUTTONUP)
	MESSAGE(onMouse, WM_MBUTTONUP)
	MESSAGE(onMouse, WM_RBUTTONUP)
	MESSAGE(onMouse, WM_LBUTTONDBLCLK)
	MESSAGE(onMouse, WM_MBUTTONDBLCLK)
	MESSAGE(onMouse, WM_RBUTTONDBLCLK)
	MESSAGE(onMouse, WM_MOUSEMOVE)
	MESSAGE(onPaint, WM_PAINT)
	MESSAGE(onPaint, LM_REPAINT)
	MESSAGE(onSysCommand, WM_SYSCOMMAND)
	MESSAGE(onTimer, WM_TIMER)
	END_MESSAGEPROC
}

void vtray::onEndSession(Message& message)
{
	message.lResult = SendMessage(liteStep, message.uMsg, message.wParam, message.lParam);
}

void vtray::onKeyMessage(Message& message)
{
	// Forward these messages
	PostMessage(liteStep, message.uMsg, message.wParam, message.lParam);
}

void vtray::onSysCommand(Message& message)
{
	if (message.wParam == SC_CLOSE)
		PostMessage(liteStep, WM_KEYDOWN, LM_SHUTDOWN, 0);
	else if (message.wParam == SC_SCREENSAVE)
	{
		bScreenSaverRunning = TRUE;
		_LSLog(LOG_DEBUG, szLogName, "screensaver start msg");
	}
	else
		message.lResult = DefWindowProc(hWnd, message.uMsg, message.wParam, message.lParam);
}

void vtray::onRefresh(Message& message)
{
	_LSLog(LOG_DEBUG, szLogName, "refreshing...");

	// ondestroy
	DeleteTimers();
	ReadSetTimers();

	// get the position and size of the main window
	// using different method if it's docked in the wharf
	ReadSizeAndPos();

	// FIXME: this logging stuff might need an update
	_LSLogPrintf(LOG_DEBUG, szLogName, "Moving Window, pos:%d,%d", nXPos, nYPos);
	Move(nXPos, nYPos);

	_LSLogPrintf(LOG_DEBUG, szLogName, "Sizing Window, size:%d,%d", nWidth, nHeight);
	Size(nWidth, nHeight);

	// oncreate
	ReadGUIProps();
	ReadSizeProps();

	ReadCommands();

	ReadWindowLists();

	IconListSetFirstLast();
	painter.SetSize(nWidth, nHeight);
	bool trans = painter.CreateBackground();
	TrayIcon::SetTransparentBg(trans);

	// set always on top
	if (IsOnTop())
		SetOnTop(true);
	else
		SetOnTop(false);

	// show window
	if (!IsVisible() || (hideIfEmpty && IconListCount(COUNT_VISIBLE) == 0) )
	{
#ifdef _DEBUG
		_LSLog(LOG_DEBUG, szLogName, "hiding tray");
#endif
		Hide();
	}
	else
		Show();

	if (autoSize)
		AdjustSize();
	else
		InvalidateRect( hWnd, NULL, TRUE );

	int icons = IconListCount(COUNT_VISIBLE);
	int i = 0;
#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "starting add bang execution, %d icons to go through", icons);
#endif
	for (i=1; i<=icons; i++) {
		if (bCheckOnCommands)
		{
			// re-read, necessary? export evars?
			ReadOnAddCommand();
			ReadOnDelCommand();
		}

		commands.RunPrevious();
		commands.RunNext();

#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "finished nr %d add bang execution", i);
#endif

	}

	_LSLog(LOG_DEBUG, szLogName, "done refresh");
}

void vtray::onCreate(Message& message)
{
	_LSLog(LOG_DEBUG, szLogName, "Reading Tray Config Lines");
	int tmpInteger = 0;
	int msgs[] = {LM_GETREVID, LM_SYSTRAY, LM_REFRESH, 0};

	// register messages
	SendMessage(liteStep, LM_REGISTERMESSAGE, (WPARAM)hWnd, (LPARAM)msgs);

	// has to be done
	SetWindowLong(hWnd, GWL_USERDATA, magicDWord);

	// make sure it handle dblclicks... =)
	SetClassLong(hWnd, GCL_STYLE, CS_DBLCLKS | GetClassLong(hWnd, GCL_STYLE));
	painter.SetWindow(hWnd);

	ReadGUIProps();
	ReadSizeProps();

	ReadCommands();

	ReadWindowLists();

	_LSLog(LOG_DEBUG, szLogName, "Done Reading Tray Config Lines");

	IconListSetFirstLast();
	painter.SetSize(nWidth, nHeight);
	bool trans = painter.CreateBackground();
	// notify the icons if they need to use regions
	TrayIcon::SetTransparentBg(trans);

	_LSLog(LOG_DEBUG, szLogName, "Setting Tray Window Props.");

	// set always on top
	if (IsOnTop())
		SetOnTop(true);
	else
		SetOnTop(false);

	// show window
	if (!IsVisible() || hideIfEmpty)
		Hide();
	else
		Show();
	InvalidateRect( hWnd, NULL, TRUE );

	ReadSetTimers();

	_LSLog(LOG_DEBUG, szLogName, "Done Tray Window Props.");
}

void vtray::onDestroy(Message& message)
{
	int i = 0;
	int msgs[] = {LM_GETREVID, LM_SYSTRAY, LM_REFRESH, 0};

	SendMessage(liteStep, LM_UNREGISTERMESSAGE, (WPARAM)hWnd, (LPARAM)msgs);

	// destroy the list of icons
	IconListDeleteAll();

	DeleteTimers();
}

void vtray::onGetRevId(Message& message)
{
	LPSTR buf = (LPSTR)(message.lParam);

	switch (message.wParam)
	{
		case 0:
		sprintf(buf, "vtray.dll: %s", &rcsRevision[0]);
		buf[strlen(buf) - 1] = '\0';
		break;
		case 1:
		strcpy(buf, &rcsId[0]);
		buf[strlen(buf) - 1] = '\0';
		break;
		default:
		strcpy(buf, "");
	}
	message.lResult = strlen(buf);
}

void vtray::onSysTray(Message& message)
{
	switch ( message.wParam )
	{
		case NIM_ADD:
		{
			message.lResult = (BOOL)( IconAdd((PLSNOTIFYICONDATA)message.lParam ) != NULL);
			break;
		}
		case NIM_MODIFY:
		{
			message.lResult = (BOOL)( IconModify((PLSNOTIFYICONDATA)message.lParam ) != NULL);
			break;
		}
		case NIM_DELETE:
		{
			message.lResult = (BOOL)IconDelete((PLSNOTIFYICONDATA)message.lParam);
			break;
		}
		default:
		{
			message.lResult = FALSE;
			break;
		}
	}
}

void vtray::onMouse(Message& message)
{
	bool trapped;

	if (!onMouseIcon(message))
	{		
		trapped = MouseExecute(message);
		// check hover and execute commands if so
		if (!trapped)
			HoverCheck();
	}

	message.lResult = DefWindowProc(hWnd, message.uMsg, message.wParam, message.lParam);
}

BOOL vtray::onMouseIcon(Message& message)
{
	TrayIcon* pTrayIcon;
	MSG msg;
	POINT pt;
	pt.x = message.lParamLo;
	pt.y = message.lParamHi;

	// get icon at point
	pTrayIcon = IconListFind(pt);

	//SetForegroundWindow(pTrayIcon->hWnd);

	msg.hwnd = hWnd;
	msg.message = message.uMsg;
	msg.wParam = message.wParam;
	msg.lParam = message.lParam;
	msg.time = GetTickCount();
	msg.pt = pt;
	SendMessage( TrayIcon::hTooltip, TTM_RELAYEVENT, 0, (LPARAM) &msg );

	// icon was activated, clear flash
	if (pTrayIcon != NULL && !AlwaysFlash && pTrayIcon->state.isFlashing)
	{
#ifdef _DEBUG
	_LSLog(LOG_DEBUG, szLogName, "Mouse pass over icon, clear flash");
#endif
		pTrayIcon->SetFlashing(false);
		pTrayIcon->state.FlashCount = 0;
	}

	// remove "unwanted" icons
	IconListCleanup();

	// post message to that icon
	if (pTrayIcon && pTrayIcon->uCallbackMessage)
	{
		if(message.uMsg == WM_RBUTTONDOWN && bWinNT5)
			SetForegroundWindow(pTrayIcon->hOwnerWnd);

		AllowSetForeground(pTrayIcon->hOwnerWnd);

		PostMessage(pTrayIcon->hOwnerWnd, pTrayIcon->uCallbackMessage, (WPARAM)pTrayIcon->uID, (LPARAM)message.uMsg);
		return TRUE;
	}
	return FALSE;
}

void vtray::onTimer(Message& message)
{
#ifdef DEBUG_SPAM
	_LSLogPrintf(LOG_DEBUG, szLogName, "Timer activated: %d", (int)message.wParam);
#endif

	switch ( message.wParam )
	{
		case TIMER_AUTOHIDE:
		{
			TrayIcon* pTrayIcon = NULL;

			// check & cleanup icons
			if (ClearIconsOnTimer)
				IconListCleanup();

			if (hideconfig.GetConfigCount() >0)
			{
				IconList::iterator i = normalIcons.begin();
				while (i != normalIcons.end() )
				{
					pTrayIcon = (*i);
					++i;
					if ( hideconfig.CheckIconTimeout(pTrayIcon) ) // if the icon isn't already hidden
					{
						IconHide(pTrayIcon);
					}
				}
			}
		}
		case TIMER_SCROLL:
		{
			// check if screensaver is running
			bScreenSaverRunning = WinUtils::IsScreenSaverRunning();

			// don't issue flash if screensaver running or if specified to do so
			if(TrayFlash && (FlashInSS || !bScreenSaverRunning) )
			{
				if ( IconListCount(COUNT_FLASHING) > 0 )
				{
					ScrollFlashing = TRUE;
					keybd_event( VK_SCROLL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
					keybd_event( VK_SCROLL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
				}
				else if (ScrollFlashing && (scrollWas && !GetKeyState(VK_SCROLL)) || (!scrollWas && GetKeyState(VK_SCROLL)))
				{
					ScrollFlashing = FALSE;
		  			keybd_event( VK_SCROLL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
					keybd_event( VK_SCROLL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
				}
			}
		}
		/*else if (resetScroll == true){
			if ((scrollWas && !GetKeyState(VK_SCROLL)) || (!scrollWas && GetKeyState(VK_SCROLL))){
		  		keybd_event( VK_SCROLL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
				keybd_event( VK_SCROLL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
			}
			resetScroll = false;
		}*/
	}
}

void vtray::onWindowPosChanging(Message& message)
{
#ifdef _DEBUG
	_LSLog(LOG_DEBUG, szLogName, "Onwindowpos received");
#endif

	LPWINDOWPOS lpwp = (LPWINDOWPOS)message.lParam;

	if ( !( lpwp->flags & SWP_NOSIZE ) )
	{
		if ( (nWidth != lpwp->cx) || (nHeight != lpwp->cy) )
		{
			// save size
			SetSize(lpwp->cx, lpwp->cy);
		}
	}

	if ( !( lpwp->flags & SWP_NOMOVE ) )
	{
		if (snapDistance)
		{
			if ( abs(lpwp->x) <= snapDistance )
				lpwp->x = 0;
			if ( abs(lpwp->y) <= snapDistance )
				lpwp->y = 0;
			if ( abs(lpwp->x + nWidth - GetSystemMetrics(SM_CXSCREEN)) <= snapDistance )
				lpwp->x = GetSystemMetrics(SM_CXSCREEN) - nWidth;
			if ( abs(lpwp->y + nHeight - GetSystemMetrics(SM_CYSCREEN)) <= snapDistance )
				lpwp->y = GetSystemMetrics(SM_CYSCREEN) - nHeight;
		}
		// save position
		SetPosition(lpwp->x, lpwp->y);
	}

	if ( !( lpwp->flags & SWP_NOSIZE) )
	{
		IconListSetFirstLast();
		painter.SetSize(nWidth, nHeight);
		bool trans = painter.CreateBackground();
		TrayIcon::SetTransparentBg(trans);

		InvalidateRect(hWnd, NULL, TRUE);
	}
	message.lResult = DefWindowProc(hWnd, message.uMsg, message.wParam, message.lParam);
}

void vtray::onPaint(Message& message)
{
	HDC hdcBuffer;
	HBITMAP hbmBuffer, hbmBufOld;
	HRGN hrgn = NULL;

	PAINTSTRUCT ps;
	HDC	hdcScreen = BeginPaint(hWnd, &ps);

	hdcBuffer = CreateCompatibleDC(hdcScreen);
	hbmBuffer = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
	hbmBufOld = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

	bool bTranspBack = painter.IsTransparent();

	if (bTranspBack)
		hrgn = CreateRectRgn( 0, 0, 0, 0 );

	// blit the skin
	painter.PaintBackground(hdcBuffer, hrgn);

	// blit the icons
	RECT rc;
	IconListPaint(hdcBuffer, hrgn, GetUpdateRect(hWnd, &rc, FALSE) ? &rc : NULL );

	// set the windowRgn, windows deletes region after it's done with it
	if (bTranspBack)
		SetWindowRgn(hWnd, hrgn, TRUE);

	// when the region is applied you can use a simple SRCCOPY from the buffer
	BitBlt(hdcScreen, 0, 0, nWidth, nHeight, hdcBuffer, 0, 0, SRCCOPY);

	// reselect the oldBitmap and kill the new one
	SelectObject(hdcBuffer, hbmBufOld);
	DeleteObject(hbmBuffer);

	// remove the memory DC
	DeleteDC(hdcBuffer);

	EndPaint(hWnd, &ps);
}

/***************************/
/***** !BANG COMMANDS ******/
/***************************/

void vtray::AcceptFlash(bool flash)
{
	// enable flash or not...
	TrayFlash = flash;
}

void vtray::IconListInfo(void)
{
	int nricons[2] = {0, 0};

	TrayIcon* pTrayIcon;
	IconList::iterator i;

	_LSLogPrintf(LOG_DEBUG, szLogName, "Normal icons (%d):", normalIcons.size());
	for (i = normalIcons.begin(); i != normalIcons.end(); ++i)
	{
		// checking if in correct list
		pTrayIcon = (*i);
		if (pTrayIcon->state.VisibilityStatus != ICON_SHOWN)
			_LSLogPrintf(LOG_NOTICE, szLogName, "Icon in wrong list (shown) \"%s\" - (0x%.8X) - (0x%.8X) shown:%d", pTrayIcon->szTip, pTrayIcon->hOwnerWnd, pTrayIcon->uID, pTrayIcon->state.VisibilityStatus);

		pTrayIcon->LogInfo();
	}

	_LSLogPrintf(LOG_DEBUG, szLogName, "Hidden icons (%d):", hiddenIcons.size());
	for (i = hiddenIcons.begin(); i != hiddenIcons.end(); ++i)
	{
		// checking if in correct list
		pTrayIcon = (*i);
		if (pTrayIcon->state.VisibilityStatus == ICON_SHOWN)
			_LSLogPrintf(LOG_NOTICE, szLogName, "Icon in wrong list (!shown) \"%s\" - (0x%.8X) - (0x%.8X) shown:%d", pTrayIcon->szTip, pTrayIcon->hOwnerWnd, pTrayIcon->uID, pTrayIcon->state.VisibilityStatus);
		pTrayIcon->LogInfo();
	}
}

void vtray::IconListShowAll(void)
{
	TrayIcon* pTrayIcon;

	IconList::iterator i = hiddenIcons.begin();
	while (i != hiddenIcons.end())
	{
		pTrayIcon = (*i);
		++i;
		IconShow(pTrayIcon);
	}
}

// calculates the paint offset for scrolling
void vtray::IconListScroll(int Type)
{
#ifdef _DEBUG
	_LSLogPrintf(LOG_WARNING, szLogName, "** Starting scroll of icons: %d **", Type);
#endif

	int nricons = IconListCount(COUNT_VISIBLE);
	int scrolledicons = IconListCount(COUNT_SCROLLED);
	int roomfor = IconListCount(COUNT_ROOMFOR);
	// if wrapcount is defined use that, otherwise change with amount that can fit
	int deltaIcons;
	if (nScrollStep > 0)
		deltaIcons = nScrollStep;
	else
		deltaIcons = (wrapCount > 0) ? wrapCount : roomfor;

#ifdef _DEBUG
	_LSLogPrintf(LOG_WARNING, szLogName, "DeltaIcons: %d, ScrollStep: %d, WrapCount: %d, RoomFor %d", deltaIcons, nScrollStep, wrapCount, roomfor);
#endif

	TrayIcon* pTrayIcon;
	int toggleIcons;
	IconList::iterator i;

	if (Type == SCROLL_AUTO)
	{
		if (bPaintNext)
			Type = SCROLL_NEXT;
		else
			Type = SCROLL_BACK;
	}

	if (!autoSize && nricons+scrolledicons > deltaIcons)
	{
		switch(Type)
		{
		case SCROLL_BACK:
			if (scrolledicons >= deltaIcons)
				toggleIcons = deltaIcons;
			else
			{
				toggleIcons = scrolledicons;
				// switch auto direction
				bPaintNext = true;
			}

#ifdef _DEBUG
			_LSLogPrintf(LOG_WARNING, szLogName, "ToggleIcons: %d", toggleIcons);
#endif

			// show here
			i = hiddenIcons.begin();
			while (i != hiddenIcons.end() && toggleIcons > 0)
			{
				pTrayIcon = (*i);
				++i;
				if (pTrayIcon->state.VisibilityStatus == ICON_SCROLLED)
				{
					--toggleIcons;
					IconShow(pTrayIcon, true);
					_LSLogPrintf(LOG_DEBUG, szLogName, "Unscrolled icon again: \"%s\" ", pTrayIcon->szTip);
				}
			}
			IconListFixPositions();
		break;
		case SCROLL_NEXT:
			// if there is more than one line left to scroll to
			if ( nricons-deltaIcons > 0 )
				toggleIcons = deltaIcons;
			else
			{
				toggleIcons = 0; //nricons-roomfor;
				// switch auto direction
				bPaintNext = false;
			}

#ifdef _DEBUG
			_LSLogPrintf(LOG_WARNING, szLogName, "ToggleIcons: %d", toggleIcons);
#endif

			// hide here
			i = normalIcons.begin();
			while (i != normalIcons.end() && toggleIcons > 0)
			{
				pTrayIcon = (*i);
				++i;
				_LSLogPrintf(LOG_DEBUG, szLogName, "Scrolling icon: \"%s\" ", pTrayIcon->szTip);
				IconHide(pTrayIcon, ICON_SCROLLED);
				--toggleIcons;
			}
		break;
		}
	}
#ifdef _DEBUG
	IconListInfo();
#endif
}

/***************************/
/********* HELPERS *********/
/***************************/

int vtray::IconListCount(int Type)
{
	int nr = 0;
	RECT rcOrg;
	int x_size, y_size, x_room, y_room, iconSize;

	TrayIcon* pTrayIcon = NULL;
	IconList::iterator i;
	switch(Type)
	{
	case COUNT_VISIBLE:
		nr = normalIcons.size();
	break;
	case COUNT_HIDDEN:
		for ( i = hiddenIcons.begin(); i != hiddenIcons.end(); ++i)
		{
			pTrayIcon = (*i);
			if (pTrayIcon->state.VisibilityStatus == ICON_HIDDEN)
				nr++;
		}
	break;
	case COUNT_SCROLLED:
		for ( i = hiddenIcons.begin(); i != hiddenIcons.end(); ++i)
		{
			pTrayIcon = (*i);
			if (pTrayIcon->state.VisibilityStatus == ICON_SCROLLED)
				nr++;
		}
	break;
	case COUNT_NOTVISIBLE:
		nr = hiddenIcons.size();
	break;
	case COUNT_ALL:
		nr = hiddenIcons.size() + normalIcons.size();
	break;
	case COUNT_FLASHING:
		for ( i = normalIcons.begin(); i != normalIcons.end(); ++i)
		{
			pTrayIcon = (*i);
			if (  pTrayIcon->state.isFlashing && ( flashconfig.GetMaxFlash(pTrayIcon->setting.MatchFlashPos) == 0 || (pTrayIcon->state.FlashCount < flashconfig.GetMaxFlash(pTrayIcon->setting.MatchFlashPos) ) )  )
			{
				nr++;
				pTrayIcon->state.FlashCount++;
#ifdef _DEBUG
				_LSLogPrintf(LOG_DEBUG, szLogName, "flash count: %d < %d", pTrayIcon->state.FlashCount, flashconfig.GetMaxFlash(pTrayIcon->setting.MatchFlashPos));
#endif
			}
		}
	break;
	case COUNT_ROOMFOR:
		// calculate amount of icons that can fit into the tray window
		iconSize = TrayIcon::GetIconSize();
		GetWindowRect(hWnd, &rcOrg);
		x_size = rcOrg.right - rcOrg.left;
		y_size = rcOrg.bottom - rcOrg.top;
		x_room = (x_size - (borderLeft+borderRight)-iconSpacingX) / (iconSize+iconSpacingX);
		y_room = (y_size - (borderTop+borderBottom)-iconSpacingY) / (iconSize+iconSpacingY);
		nr = x_room * y_room;
	break;
	case COUNT_PERWRAP:
		// calculate amount of icons that can fit into the tray window
		RECT rcOrg;
		int x_size, y_size;
		int iconSize = TrayIcon::GetIconSize();
		GetWindowRect(hWnd, &rcOrg);
		x_size = rcOrg.right - rcOrg.left;
		y_size = rcOrg.bottom - rcOrg.top;
		if (direction & TD_VERTICAL)
			nr = (y_size - (borderTop+borderBottom)-iconSpacingY) / (iconSize+iconSpacingY);
		else
			nr = (x_size - (borderLeft+borderRight)-iconSpacingX) / (iconSize+iconSpacingX);
	break;
	}
	return nr;
}

// calculates first and last icon positions
void vtray::IconListSetFirstLast()
{
	int iconSize = TrayIcon::GetIconSize();
	// the following settings are used with the icon rectangles
	// to ensure they match the desired pattern
	firstX = ( ( direction == TD_LEFT ) || ( wrapDirection == TD_LEFT ) ) ?
	         ( nWidth - borderRight - iconSpacingX - iconSize ) :
	         ( borderLeft + iconSpacingX );
	lastX = firstX + ( wrapCount - 1 ) * deltaX;

	firstY = ( ( direction == TD_DOWN ) || ( wrapDirection == TD_DOWN ) ) ?
	         ( borderTop + iconSpacingY ) :
	         ( nHeight - borderBottom - iconSpacingY - iconSize );

	lastY = firstY + ( wrapCount - 1 ) * deltaY;
}

TrayIcon* vtray::IconListFind(HWND hwnd, UINT uID)
{
	TrayIcon* pTrayIcon;
	IconList::iterator i;

	for (i = normalIcons.begin(); i != normalIcons.end(); ++i)
	{
		pTrayIcon = (*i);
		if ((pTrayIcon->hOwnerWnd == hwnd) && (pTrayIcon->uID == uID))
			return pTrayIcon;
	}
	for (i = hiddenIcons.begin(); i != hiddenIcons.end(); ++i)
	{
		pTrayIcon = (*i);
		if ((pTrayIcon->hOwnerWnd == hwnd) && (pTrayIcon->uID == uID))
			return pTrayIcon;
	}

	return NULL;
}

TrayIcon* vtray::IconListFind(POINT pt)
{
	TrayIcon* pTrayIcon;

	for (IconList::iterator i = normalIcons.begin(); i != normalIcons.end(); ++i)
	{
		pTrayIcon = (*i);

		if ( pTrayIcon->IsPointOver(pt) )
		{
			if (IsWindow(pTrayIcon->hOwnerWnd) )
			{
				return pTrayIcon;
			}
			else
			{
				IconDelete(pTrayIcon);
				return NULL;
			}
		}
	}
	return NULL;
}

void vtray::IconListCleanup()
{
	TrayIcon* pTrayIcon;
	IconList::iterator i;

	i = normalIcons.begin();
	while ( i != normalIcons.end())
	{
		pTrayIcon = (*i);
		++i;

		#ifdef DEBUG_SPAM
			_LSLogPrintf(LOG_DEBUG, szLogName, "checking window: 0x%X", pTrayIcon->hOwnerWnd);
		#endif

		if (!IsWindow(pTrayIcon->hOwnerWnd))
			IconDelete(pTrayIcon);
	}
	i = hiddenIcons.begin();
	while ( i != hiddenIcons.end())
	{
		pTrayIcon = (*i);
		++i;

		#ifdef DEBUG_SPAM
			_LSLogPrintf(LOG_DEBUG, szLogName, "checking window: 0x%X", pTrayIcon->hOwnerWnd);
		#endif

		if (!IsWindow(pTrayIcon->hOwnerWnd))
			IconDelete(pTrayIcon);
	}
}

void vtray::IconListFixPositions()
{
	IconList::iterator i;

	for (i = normalIcons.begin(); i != normalIcons.end(); ++i)
	{
		IconAdjustRect(*i);
	}
}

BOOL vtray::IconListDeleteAll()
{
	int removed[2] = {0, 0};
	_LSLog(LOG_DEBUG, szLogName, "Deleting all icons");

	TrayIcon* pTrayIcon;
	IconList::iterator i;

	i = normalIcons.begin();
	while ( i != normalIcons.end() )
	{
		pTrayIcon = (*i);
		++i;
		IconDelete(pTrayIcon);
		removed[0]++;
	}
	i = hiddenIcons.begin();
	while ( i != hiddenIcons.end() )
	{
		pTrayIcon = (*i);
		++i;
		IconDelete(pTrayIcon);
		removed[1]++;
	}

	_LSLogPrintf(LOG_DEBUG, szLogName, "Deleted icons: shown %d, hidden %d", removed[0], removed[1]);
	return TRUE;
}

void vtray::IconListPaint( HDC hdcDst, HRGN hrgnDst, LPRECT updateRC )
{
	for (IconList::const_iterator i = normalIcons.begin(); i != normalIcons.end(); ++i)
	{
		(*i)->Paint(hdcDst, hrgnDst, updateRC);
	}
}

/***************************/
/**** PROPERTY CHANGERS ****/
/***************************/

// calculates & sets where this icon should be placed, using previous location
void vtray::IconAdjustRect(TrayIcon* pTrayIcon)
{
	// if it should be painted, extra precaution
	if ( pTrayIcon->state.VisibilityStatus != ICON_SHOWN )
		return;

	// find the previous icon
	TrayIcon* pPrevTrayIcon = NULL;
	if (pTrayIcon->state.VisibilityStatus == ICON_SHOWN)
	{
		for (IconList::const_iterator i = normalIcons.begin(); i != normalIcons.end(); ++i)
		{
			if ( *i == pTrayIcon)
				break;

			pPrevTrayIcon = (*i);
		}
	}

	if (pPrevTrayIcon)
	{
		if (direction & TD_VERTICAL)
		{
			// if there is room for one more icon
			if (pPrevTrayIcon->rc.top != lastY)
			{
				pTrayIcon->SetPosition(pPrevTrayIcon->rc.left, pPrevTrayIcon->rc.top + deltaY);
			}
			else
			{
				pTrayIcon->SetPosition(pPrevTrayIcon->rc.left + deltaX, firstY);
			}
		}
		else
		{
			// if there is room for one more icon
			if (pPrevTrayIcon->rc.left != lastX)
			{
				pTrayIcon->SetPosition(pPrevTrayIcon->rc.left + deltaX, pPrevTrayIcon->rc.top);
			}
			else
			{
				pTrayIcon->SetPosition(firstX, pPrevTrayIcon->rc.top + deltaY);
			}
		}
	}
	else
	{
		pTrayIcon->SetPosition(pTrayIcon->rc.left = firstX, firstY);
	}

	if (pTrayIcon->szTip)
	{
		TOOLINFO ti;
		ti.cbSize = sizeof(TOOLINFO);
		ti.hwnd = hWnd;
		ti.uId = pTrayIcon->uToolTip;
		ti.rect = pTrayIcon->rc;
		SendMessage(TrayIcon::hTooltip, TTM_NEWTOOLRECT, 0, (LPARAM)&ti);
	}
}

// newer adjustsize, uses nr of visible icons, old one failed to resize correctly sometimes
void vtray::AdjustSize()
{
	RECT rcOrg;
	int x_pos, y_pos, x_size, y_size;
	int Icons = IconListCount(COUNT_VISIBLE);
	int iconSize = TrayIcon::GetIconSize();
	int wrapLines = 0; // wrap testing
	int wrapDivlines = (wrapCount > 0) ? (Icons / wrapCount) : 0; // integer division lines, maybe used later
	int wrapRest = (wrapCount > 0) ? Icons % wrapCount : Icons; // nr icons in last line
	int wrapSize = (wrapDivlines > 0) ? wrapCount : Icons; // nr of icons in each wrapline

	// if we have wrap, check how many lines we wrap
	// and check if the integer division leaves any icon, then extra line
	// also we get a line if no wrap at all, but there are icons...
	wrapLines = ((wrapCount > 0) ? wrapDivlines:0) + ((wrapRest > 0) ? 1:0);

	// this is the new size...
	if (normalIcons.size() > 0) // if we have icons at all
	{
		if (direction & TD_VERTICAL)
		{
			x_size = iconSize * wrapLines + iconSpacingX * (wrapLines+1) + borderRight + borderLeft;
			y_size = iconSize * wrapSize + iconSpacingY * (wrapSize+1) + + borderBottom + borderTop;
		}
		else
		{
			x_size = iconSize * wrapSize + iconSpacingX * (wrapSize+1) + borderRight + borderLeft;
			y_size = iconSize * wrapLines + iconSpacingY * (wrapLines+1) + + borderBottom + borderTop;
		}
	}
	else
	{
		x_size = borderLeft + borderRight;
		y_size = borderTop + borderBottom;
	}

	// position it according to the old position
	GetWindowRect(hWnd, &rcOrg);

	x_pos = ((direction == TD_RIGHT) || (wrapDirection == TD_RIGHT)) ? rcOrg.left : rcOrg.right - x_size;
	y_pos = ((direction == TD_DOWN) || (wrapDirection == TD_DOWN)) ? rcOrg.top : rcOrg.bottom - y_size;

#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "rcOrg: l%d r%d t%d b%d ", rcOrg.left, rcOrg.right, rcOrg.top, rcOrg.bottom);
	_LSLogPrintf(LOG_DEBUG, szLogName, "xpos: %d, %d, %d", rcOrg.right - x_size, rcOrg.right, x_size);
#endif

	MoveAndSize(x_pos, y_pos, x_size, y_size);

#ifdef _DEBUG
	// test:
	if ( normalIcons.size() > 0 ) {
		_LSLogPrintf(LOG_DEBUG, szLogName, "Adjusting size: x:%d, y:%d, size x:%d, size y:%d", x_pos,y_pos,x_size,y_size);
		_LSLogPrintf(LOG_DEBUG, szLogName, "wrapCount: %d, wrapRest: %d, wrapDivlines: %d, wrapSize: %d, wrapLines: %d", wrapCount, wrapRest, wrapDivlines, wrapSize, wrapLines);
		_LSLogPrintf(LOG_DEBUG, szLogName, "borders: l:%d, r:%d, t:%d, b:%d, sX:%d, sY:%d, ico:%d, icons:%d", borderLeft, borderRight, borderTop, borderBottom, iconSpacingX, iconSpacingY, iconSize, IconListCount(COUNT_VISIBLE) );
	}
#endif

	/*
	setFirstLast();
	createBackground();*/

	// place icons in window
	if ( normalIcons.size() > 0 )
	{
		TrayIcon* pTrayIcon;

		for (IconList::const_iterator i = normalIcons.begin(); i != normalIcons.end(); ++i)
		{
			pTrayIcon = (*i);
			IconAdjustRect(pTrayIcon);
		}
	}
}

/***************************/
/****** ICON HANDLING ******/
/***************************/

#ifdef ICONS_REGISTRY

bool vtray::SaveNewIcon(PLSNOTIFYICONDATA pnid)
{
	bool result = false;
	long lresult;
	HKEY traysession;
	lresult = RegCreateKeyEx( HKEY_CURRENT_USER, TEXT("Software\\LiteStep\\TraySession"), 0,
		NULL, REG_OPTION_VOLATILE, KEY_WRITE,
		NULL, &traysession, NULL
	);

	if ( lresult == ERROR_SUCCESS )
	{
		TCHAR keyname[128];
		_itot( (int) pnid->hWnd, keyname, 16);

		lresult = RegSetValueEx( traysession, keyname, 0,
			REG_BINARY, (BYTE*) pnid, sizeof(LSNOTIFYICONDATA)
		);

		if ( lresult == ERROR_SUCCESS )
		{
			result = true;
		}
	}
	return result;
}

bool vtray::RestoreIcons()
{
	bool result = false;
	long lresult;
	HKEY traysession;
	LSNOTIFYICONDATA nid;

	lresult = RegOpenKeyEx( HKEY_CURRENT_USER, TEXT("Software\\LiteStep\\TraySession"), 0,
		KEY_READ, &traysession
	);

	if ( lresult == ERROR_SUCCESS )
	{
		DWORD iconssaved;

		lresult = RegQueryInfoKey(traysession, NULL, NULL, 0,
			NULL, NULL, NULL,
			&iconssaved, NULL, NULL,
			NULL, NULL
		);

		if ( lresult == ERROR_SUCCESS )
		{
			long enumresult;
			TCHAR keyname[128];
			DWORD namelength;
			for (int i = 0; i < iconssaved; ++i)
			{
				namelength = 128; 
				keyname[0] = '\0';
				DWORD pnidsize = sizeof(LSNOTIFYICONDATA);

				enumresult = RegEnumValue( traysession, i, keyname,
					&namelength, 0, NULL,
					(BYTE*) &nid, &pnidsize
				);

				if (enumresult == ERROR_SUCCESS)
				{
					// check if icon is still valid
					if ( IsWindow(nid.hWnd) )
					{
						IconAdd(&nid);
					}					
				}
			}
			result = true;
		}
	}
	return result;
}

bool vtray::RemoveIcons()
{
	bool result = false;
	long lresult;
	HKEY traysession;
	LSNOTIFYICONDATA nid;

	lresult = RegOpenKeyEx( HKEY_CURRENT_USER, TEXT("Software\\LiteStep\\TraySession"), 0,
		KEY_READ, &traysession
	);

	if ( lresult == ERROR_SUCCESS )
	{
		DWORD iconssaved;

		lresult = RegQueryInfoKey(traysession, NULL, NULL, 0,
			NULL, NULL, NULL,
			&iconssaved, NULL, NULL,
			NULL, NULL
		);

		if ( lresult == ERROR_SUCCESS )
		{
			long enumresult;
			TCHAR keyname[128];
			DWORD namelength;
			for (int i = 0; i < iconssaved; ++i)
			{
				namelength = 128; 
				keyname[0] = '\0';
				DWORD pnidsize = sizeof(LSNOTIFYICONDATA);

				enumresult = RegEnumValue( traysession, i, keyname,
					&namelength, 0, NULL,
					(BYTE*) &nid, &pnidsize
				);

				if (enumresult == ERROR_SUCCESS)
				{
					DestroyIcon(nid.hIcon);
				}
			}			
		}

		lresult = RegDeleteKey(traysession, NULL);
		if (lresult = ERROR_SUCCESS)
		{
			result = true;
		}
	}
	return result;
}

#endif

TrayIcon* vtray::IconAdd(PLSNOTIFYICONDATA pnid)
{
	if ( !IsWindow(pnid->hWnd) )
		return NULL;

	TrayIcon* pTrayIcon;

#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "Adding Icon: \"%s\" - (0x%.8X) - (0x%.8X)", pnid->szTip, pnid->hWnd, pnid->uID);
#endif

	// check if the icon already exists
	pTrayIcon = IconListFind(pnid->hWnd, pnid->uID);

	// if found, modify instead of add
	if ( pTrayIcon )
		return IconModify( pnid );

#ifdef ICONS_REGISTRY
	// save new icon to registry for restore
	if (bRememberIcons)
		SaveNewIcon(pnid);
#endif

	// if not found (which is the way it should go)
	pTrayIcon = new TrayIcon(pnid);

	// check class name once on add
	pTrayIcon->setting.MatchHidePos = hideconfig.MatchTrayIconClass(pTrayIcon);
	pTrayIcon->setting.MatchFlashPos = flashconfig.MatchTrayIconClass(pTrayIcon);

	// check tooltip, if no tip, set as possible to add if the class match worked
	pTrayIcon->setting.canHide = (pnid->uFlags & NIF_TIP && pTrayIcon->setting.MatchHidePos != MATCH_NONE) ? hideconfig.MatchTrayIconTitle(pTrayIcon) : FALSE;
	if (pTrayIcon->setting.canHide)
		pTrayIcon->state.updated = time(NULL); // update time
	//pTrayIcon->setting.canFlash = MatchIconTitle(pTrayIcon, pnid->szTip, MATCH_FLASH);

	// check if icon should be hidden from start
	if ( hideconfig.CheckIconTimeout(pTrayIcon) )
	{
		IconHide(pTrayIcon); // should do all the handling
	}
	else
	{
		// add to last position in the list
		normalIcons.push_back(pTrayIcon);

		// if it's the first icon, send an SHOW message
		if ( IconListCount(COUNT_VISIBLE) == 1 && (hideIfEmpty && !IsVisible()))
			Show();
	}

	// adjust the rectangle to the correct position ( uses list position )
	IconAdjustRect(pTrayIcon);

	// remove "unwanted" icons
	IconListCleanup();

	if (autoSize)
	{
		AdjustSize(); // which will repaint the window
	}
	else
	{
		InvalidateRect(hWnd, NULL, TRUE); // &pTrayIcon->rc
	}

	// run last
	if (pTrayIcon->state.VisibilityStatus == ICON_SHOWN)
	{
		ExportEvars();
		RunCommandAdd();
	}

	_LSLogPrintf(LOG_DEBUG, szLogName, "Icon Added \"%s\", shown:%d (settings F:%d, H:%d) (pos X:%d Y:%d)", pTrayIcon->szTip, pTrayIcon->state.VisibilityStatus, pTrayIcon->setting.MatchFlashPos, pTrayIcon->setting.MatchHidePos, pTrayIcon->rc.left, pTrayIcon->rc.top);

	// return the newly created icon
	return pTrayIcon;
}

TrayIcon* vtray::IconModify(PLSNOTIFYICONDATA pnid)
{
	TrayIcon* pTrayIcon;
	bool titleUpdated = false;

	// check if the icon already exists
	pTrayIcon = IconListFind(pnid->hWnd, pnid->uID);

	// if not found, add instead of modify
	if (!pTrayIcon)
		return IconAdd(pnid);

	titleUpdated = pTrayIcon->Update(pnid);

	// update if there is a new title...
	if (titleUpdated)
	{
		pTrayIcon->setting.canHide = hideconfig.MatchTrayIconTitle(pTrayIcon);
		pTrayIcon->SetFlashing(flashconfig.MatchTrayIconTitle(pTrayIcon));
#ifdef _DEBUG
		_LSLogPrintf(LOG_DEBUG, szLogName, "Icon update %s, flash: %d", pnid->szTip, pTrayIcon->state.isFlashing);
#endif
	}

	if ( pTrayIcon->state.VisibilityStatus == ICON_HIDDEN && !hideconfig.CheckIconTimeout(pTrayIcon) ) // icon was updated & it has a timer > 0
	{
		IconShow(pTrayIcon);
	}

#ifdef DEBUG_SPAM
	if ( pTrayIcon->state.isFlashing )
	{
		_LSLogPrintf(LOG_DEBUG, szLogName, "Icon should flash: %s", pTrayIcon->szTip);
	}
#endif

	// remove "unwanted" icons
	IconListCleanup();

	// only update this icon?
	InvalidateRect(hWnd, &(pTrayIcon->rc), TRUE); // &(pTrayIcon->rc) / NULL

	// return the "refreshed" icon
	return pTrayIcon;
}

BOOL vtray::IconDelete(PLSNOTIFYICONDATA pnid)
{
	TrayIcon* pTrayIcon = IconListFind(pnid->hWnd, pnid->uID);

	if (pTrayIcon)
	{
		return IconDelete(pTrayIcon);
	}

	return FALSE;
}

BOOL vtray::IconDelete(TrayIcon* pTrayIcon)
{
	if ( !pTrayIcon )
		return FALSE;

	_LSLogPrintf(LOG_DEBUG, szLogName, "Deleting Icon: %s - (0x%.8X) - (0x%.8X) shown:%d", pTrayIcon->szTip, pTrayIcon->hOwnerWnd, pTrayIcon->uID, pTrayIcon->state.VisibilityStatus);

	if (pTrayIcon->state.VisibilityStatus == ICON_SHOWN)
	{
		IconShownRemove(pTrayIcon);
	}
	else
		hiddenIcons.remove(pTrayIcon);

	// remove the icon class
	delete pTrayIcon;

#ifdef _DEBUG
	_LSLog(LOG_DEBUG, szLogName, "Icon Deleted");
#endif

	return TRUE;
}

// handles the removal of an icon from the visible list, updates tooltips and locations
BOOL vtray::IconShownRemove(TrayIcon* pTrayIcon)
{
	TrayIcon* pNextTrayIcon = NULL;

	pTrayIcon->RemoveTooltip();
#ifdef _DEBUG
	_LSLog(LOG_DEBUG, szLogName, "Adjusting rectangles and tooltips");
#endif
	for (IconList::reverse_iterator i = normalIcons.rbegin(); i != normalIcons.rend(); ++i)
	{
		if (pNextTrayIcon == pTrayIcon)
			break;

		// change all the rectangles and update the tooltips
		// swap rectangles... this is smooth, only on shown icons
		if (pNextTrayIcon != NULL)
		{
			pNextTrayIcon->rc = (*i)->rc;
#ifdef _DEBUG
			_LSLogPrintf(LOG_DEBUG, szLogName, "Traded places: \"%s\" <-- \"%s\"", pNextTrayIcon->szTip, (*i)->szTip);
#endif
			pNextTrayIcon->UpdateTooltip();
		}
		pNextTrayIcon = (*i);
	}
	normalIcons.remove(pTrayIcon);

	if ( IconListCount(COUNT_VISIBLE) == 0 && hideIfEmpty)
	{
		Hide();
	}
	else
	{
		if (autoSize)
			AdjustSize();
		else
			InvalidateRect( hWnd, NULL, TRUE );
	}
	// run last
	ExportEvars();
	RunCommandDel();

	return TRUE;
}


/***************************/
/** ICON HIDING FUNCTIONS* */
/***************************/

void vtray::IconHide(TrayIcon* pTrayIcon, int status)
{
	if (pTrayIcon != NULL && pTrayIcon->state.VisibilityStatus == ICON_SHOWN)
	{
		pTrayIcon->state.VisibilityStatus = status;
		_LSLogPrintf(LOG_DEBUG, szLogName, "iconHide \"%s\" - (0x%.8X) - (0x%.8X) shown:%d", pTrayIcon->szTip, pTrayIcon->hOwnerWnd, pTrayIcon->uID, pTrayIcon->state.VisibilityStatus);
		// change position

		// move from displayed icon list to hidden
		IconShownRemove(pTrayIcon);
		if ( status == ICON_SCROLLED )
			hiddenIcons.push_front(pTrayIcon);
		else
			hiddenIcons.push_back(pTrayIcon);
	}
}

void vtray::IconShow(TrayIcon* pTrayIcon, bool bFront)
{
	if (pTrayIcon != NULL && pTrayIcon->state.VisibilityStatus != ICON_SHOWN)
	{
		pTrayIcon->state.VisibilityStatus = ICON_SHOWN;
		_LSLogPrintf(LOG_DEBUG, szLogName, "iconShow \"%s\" - (0x%.8X) - (0x%.8X) shown:%d", pTrayIcon->szTip, pTrayIcon->hOwnerWnd, pTrayIcon->uID, pTrayIcon->state.VisibilityStatus);
		// change position

		// link out of hidden chain
		hiddenIcons.remove(pTrayIcon);
		if (bFront)
			normalIcons.push_front(pTrayIcon);
		else
			normalIcons.push_back(pTrayIcon);

		pTrayIcon->UpdateTooltip(true);

		// set this icons position
		IconAdjustRect(pTrayIcon);

		// if it's the first icon, send an SHOW message
		if ( normalIcons.size() == 1 && (hideIfEmpty && !IsVisible()) )
			Show();

		if (autoSize)
			AdjustSize();
		else
			InvalidateRect( hWnd, NULL, TRUE );

		ExportEvars();
		RunCommandAdd(); // run command on show
	}
}

void vtray::ExportEvars()
{
	char var[256];

	// nr of icons visible
	memset(var, 0, 256);
	sprintf(var, "%d", IconListCount(COUNT_VISIBLE));
	PrefixedLSSetVariable(szSettingsPrefix, TEXT("CurrentIcons"), var);

	memset(var, 0, 256);
	sprintf(var, "%d", nXPos);
	PrefixedLSSetVariable(szSettingsPrefix, TEXT("CurrentX"), var);

	memset(var, 0, 256);
	sprintf(var, "%d", nYPos);
	PrefixedLSSetVariable(szSettingsPrefix, TEXT("CurrentY"), var);

	memset(var, 0, 256);
	sprintf(var, "%d", nWidth);
	PrefixedLSSetVariable(szSettingsPrefix, TEXT("CurrentWidth"), var);

	memset(var, 0, 256);
	sprintf(var, "%d", nHeight);
	PrefixedLSSetVariable(szSettingsPrefix, TEXT("CurrentHeight"), var);

}

void vtray::RunCommandAdd(void)
{
	int ExecuteNr = IconListCount(COUNT_VISIBLE);

#ifdef _DEBUG
	_LSLog(LOG_DEBUG, szLogName, "starting add bang execution");
#endif

	// re-read
	if (bCheckOnCommands)
	{
		ReadOnAddCommand();
	}
	commands.RunNext(ExecuteNr);

#ifdef _DEBUG
	_LSLog(LOG_DEBUG, szLogName, "finished add bang execution");
#endif
}

void vtray::RunCommandDel(void)
{
#ifdef _DEBUG
	_LSLog(LOG_DEBUG, szLogName, "starting del bang execution");
#endif

	int ExecuteNr = IconListCount(COUNT_VISIBLE);

	if (bCheckOnCommands)
	{
		ReadOnDelCommand();
	}
	commands.RunPrevious(ExecuteNr);

#ifdef _DEBUG
	_LSLog(LOG_DEBUG, szLogName, "finished del bang execution");
#endif
}

/***************************/
/**** SETTINGS READERS *****/
/***************************/

void vtray::ReadWindowLists()
{
	hideconfig.ClearAll();
	flashconfig.ClearAll();

	_LSLog(LOG_DEBUG, szLogName, "starting read of *Hide commands");
	ReadIconHide(); // which icons should be hidden
	_LSLogPrintf(LOG_DEBUG, szLogName, "finished reading of *Hide commands, total: %d", hideconfig.GetConfigCount());

	_LSLog(LOG_DEBUG, szLogName, "starting read of *Flash commands");
	ReadIconFlash(); // which icons should be flashing
	_LSLogPrintf(LOG_DEBUG, szLogName, "finished reading of *Flash commands, total: %d", flashconfig.GetConfigCount());
}

void vtray::DeleteTimers()
{
	// kill timers
	if (nHideTimer)
		KillTimer(hWnd, TIMER_AUTOHIDE);
	if (nScrollTimer)
		KillTimer(hWnd, TIMER_SCROLL);
}

void vtray::ReadSetTimers()
{
	ClearIconsOnTimer = PrefixedGetRCBool(szSettingsPrefix, TEXT("TimerCleaning"), TRUE);

	nHideTimer   = 1000 * min( max(PrefixedGetRCInt(szSettingsPrefix, TEXT("IconTimer"), 10), 1), 600); // how often we look for icons to hide.
	nScrollTimer = min( max(PrefixedGetRCInt(szSettingsPrefix, TEXT("ScrollFlashInterval"), 500), 100), 10000);

	if (nHideTimer)
		SetTimer(hWnd, TIMER_AUTOHIDE, nHideTimer, NULL); // set timer in seconds
	if (nScrollTimer)
		SetTimer(hWnd, TIMER_SCROLL, nScrollTimer, NULL);
}

void vtray::ReadOnAddCommand()
{
	TCHAR szTemp[MAX_LINE_LENGTH];
	PrefixedGetRCLine(szSettingsPrefix, TEXT("OnAdd"), szTemp, MAX_LINE_LENGTH, TEXT("!NONE")); // added icon command
	commands.SetOnAddConfig(szTemp);
}

void vtray::ReadOnDelCommand()
{
	TCHAR szTemp[MAX_LINE_LENGTH];
	PrefixedGetRCLine(szSettingsPrefix, TEXT("OnDel"), szTemp, MAX_LINE_LENGTH, TEXT("!NONE")); // added icon command
	commands.SetOnDelConfig(szTemp);
}

void vtray::ReadCommands()
{
	bCheckOnCommands = !PrefixedGetRCBool(szSettingsPrefix, TEXT("DoNotCheckOnCommands"), TRUE);

	// OnAdd and OnDel commands
	ReadOnDelCommand();
	ReadOnAddCommand();

	ReadMouseCommands();

	_LSLog(LOG_DEBUG, szLogName, "starting read of *On commands");
	ReadAddCommands(); // certain commands for amount of icons
	_LSLogPrintf(LOG_DEBUG, szLogName, "finished reading of *On commands, total: %d", commands.GetConfigCount());

	// for 24.7 compatibility, why is this needed?
	//#define size_test MAX_LINE_LENGTH // between 1200-1300
	//char tok1[size_test], tok2[size_test], tok3[size_test]; // can name these any way you like
	//char* tokn[3] = { tok1, tok2, tok3}; // can name these any way you like
	//LCTokenize (NULL, tokn, 0, NULL); // removing this line crashes 0.24.7
}

void vtray::ReadMouseCommands()
{
	TCHAR szTemp[MAX_LINE_LENGTH];
	memset(szTemp, 0, sizeof(TCHAR)*MAX_LINE_LENGTH);

	// OnMouseEnter and OnMouseLeave commands
	PrefixedGetRCLine(szSettingsPrefix, TEXT("OnMouseEnter"), szTemp, MAX_LINE_LENGTH, TEXT("!NONE")); // mouse enter command
	SetMouseEnterCommand(szTemp);

	PrefixedGetRCLine(szSettingsPrefix, TEXT("OnMouseLeave"), szTemp, MAX_LINE_LENGTH, TEXT("!NONE")); // mouse leave command
	SetMouseLeaveCommand(szTemp);	

	PrefixedGetRCLine(szSettingsPrefix, TEXT("OnLeftDBL"), szTemp, MAX_LINE_LENGTH, TEXT("!None"));
	SetMouseCommand(WM_LBUTTONDBLCLK, szTemp);

	PrefixedGetRCLine(szSettingsPrefix, TEXT("OnLeftDown"), szTemp, MAX_LINE_LENGTH, TEXT(".drag"));
	SetMouseCommand(WM_LBUTTONDOWN, szTemp);

	PrefixedGetRCLine(szSettingsPrefix, TEXT("OnLeftUp"), szTemp, MAX_LINE_LENGTH, TEXT("!None"));
	SetMouseCommand(WM_LBUTTONUP, szTemp);

	PrefixedGetRCLine(szSettingsPrefix, TEXT("OnRightDBL"), szTemp, MAX_LINE_LENGTH, TEXT("!None"));
	SetMouseCommand(WM_RBUTTONDBLCLK, szTemp);

	PrefixedGetRCLine(szSettingsPrefix, TEXT("OnRightDown"), szTemp, MAX_LINE_LENGTH, TEXT("!None"));
	SetMouseCommand(WM_RBUTTONDOWN, szTemp);

	PrefixedGetRCLine(szSettingsPrefix, TEXT("OnRightUp"), szTemp, MAX_LINE_LENGTH, TEXT("!Popup"));
	SetMouseCommand(WM_RBUTTONUP, szTemp);

	PrefixedGetRCLine(szSettingsPrefix, TEXT("OnMiddleDBL"), szTemp, MAX_LINE_LENGTH, TEXT("!None"));
	SetMouseCommand(WM_MBUTTONDBLCLK, szTemp);

	PrefixedGetRCLine(szSettingsPrefix, TEXT("OnMiddleDown"), szTemp, MAX_LINE_LENGTH, TEXT("!None"));
	SetMouseCommand(WM_MBUTTONDOWN, szTemp);

	PrefixedGetRCLine(szSettingsPrefix, TEXT("OnMiddleUp"), szTemp, MAX_LINE_LENGTH, TEXT("!None"));
	SetMouseCommand(WM_MBUTTONUP, szTemp);
}

void vtray::SetMouseCommand(const int ID, LPCTSTR command)
{
	if ( _tcscmp(command, TEXT(".drag")) == 0 )
	{
		DragAction::ConnectGuiWindow(this);
		SetMouseKey(ID, new DragAction);		
	}
	else
	{
		SetMouseKey(ID, command);
	}
}

void vtray::ReadGUIProps()
{
	TCHAR szTemp[MAX_LINE_LENGTH];
	int tmpInteger = 0;

	// load settings here
	// ===========================================

	hideIfEmpty = !bInWharf && PrefixedGetRCBool(szSettingsPrefix, TEXT("HideIfEmpty"), TRUE);
	autoSize = !bInWharf && PrefixedGetRCBool(szSettingsPrefix, TEXT("AutoSize"), TRUE);
	tmpInteger = PrefixedGetRCInt(szSettingsPrefix, TEXT("BorderSize"), 0);
	snapDistance = max(PrefixedGetRCInt(szSettingsPrefix, TEXT("SnapDistance"), (PrefixedGetRCBool(szSettingsPrefix, TEXT("Snap"), TRUE) ? 10 : 0)), 0);

	borderDrag = !bInWharf && PrefixedGetRCBool(szSettingsPrefix, TEXT("BorderDrag"), TRUE);
	SetDraggable(borderDrag);

	// icon effects
	COLORREF hue = PrefixedGetRCColor(szSettingsPrefix, TEXT("IconHueColor"), RGB(128,128,128) );
	BYTE hueintensity = (BYTE)min( max( PrefixedGetRCInt(szSettingsPrefix, TEXT("IconHueIntensity"), 0), 0), 255 );
	BYTE saturation = (UCHAR)min( max( PrefixedGetRCInt(szSettingsPrefix, TEXT("IconSaturation"), 255), 0), 255 );
	TrayIcon::AddColorSetting(hue, hueintensity, saturation, TINT_STANDARD);

	hue = PrefixedGetRCColor(szSettingsPrefix, TEXT("IconHueColor"), hue );
	hueintensity = (BYTE)min( max( PrefixedGetRCInt(szSettingsPrefix, TEXT("IconHueIntensity"), hueintensity), 0), 255 );
	saturation = (UCHAR)min( max( PrefixedGetRCInt(szSettingsPrefix, TEXT("IconSaturation"), saturation), 0), 255 );
	TrayIcon::AddColorSetting(hue, hueintensity, saturation, TINT_FLASHING);

	// borders;
	borderTop = max(PrefixedGetRCInt(szSettingsPrefix, TEXT("BorderTop"), tmpInteger), 0);
	borderLeft = max(PrefixedGetRCInt(szSettingsPrefix, TEXT("BorderLeft"), tmpInteger), 0);
	borderRight = max(PrefixedGetRCInt(szSettingsPrefix, TEXT("BorderRight"), tmpInteger), 0);
	borderBottom = max(PrefixedGetRCInt(szSettingsPrefix, TEXT("BorderBottom"), tmpInteger), 0);
	painter.SetBorders(borderLeft, borderTop, borderRight, borderBottom);

	// skinning
	PrefixedGetRCString(szSettingsPrefix, TEXT("Bitmap"), szTemp, TEXT(""), MAX_LINE_LENGTH);
	if ( szTemp[0] == '\0' ) // TCHAR version?
	{
		painter.SetColors(
			PrefixedGetRCColor(szSettingsPrefix, TEXT("BGColor"), RGB(255, 255, 255)),
			PrefixedGetRCColor(szSettingsPrefix, TEXT("BorderColor"), RGB(0, 0, 0))
		);
	}
	else
	{
		painter.SetBitmap(LoadLSImage(szTemp, NULL));
		painter.SetTiled(PrefixedGetRCBool(szSettingsPrefix, TEXT("BitmapTiled"), TRUE));
	}
}

void vtray::ReadSizeProps()
{
	TCHAR szTemp[MAX_LINE_LENGTH];

	// get direction
	PrefixedGetRCString(szSettingsPrefix, TEXT("Direction"), szTemp, TEXT(""), MAX_LINE_LENGTH);
	if ( !::_tcsnicmp(szTemp, TEXT("left"), 5) )
		direction = TD_LEFT;
	else if ( !::_tcsnicmp(szTemp, TEXT("up"), 3) )
		direction = TD_UP;
	else if ( !::_tcsnicmp(szTemp, TEXT("down"), 5) )
		direction = TD_DOWN;
	else
		direction = TD_RIGHT;

	// get alignment
	PrefixedGetRCString(szSettingsPrefix, TEXT("Align"), szTemp, TEXT(""), MAX_LINE_LENGTH);
	_LSLogPrintf(LOG_DEBUG, szLogName, "align string: %s", szTemp);
	SetAlignment(szTemp);

	// get wrap-direction
	PrefixedGetRCString(szSettingsPrefix, TEXT("WrapDirection"), szTemp, TEXT(""), MAX_LINE_LENGTH);
	if (direction & TD_VERTICAL)
	{
		if ( !::_tcsnicmp( szTemp, TEXT("left"), 5 ) )
			wrapDirection = TD_LEFT;
		else
			wrapDirection = TD_RIGHT;
	}
	else
	{
		if ( !::_tcsnicmp( szTemp, TEXT("up"), 3) )
			wrapDirection = TD_UP;
		else
			wrapDirection = TD_DOWN;
	}

	// icon
	iconSpacingX = max(PrefixedGetRCInt(szSettingsPrefix, TEXT("IconSpacingX"), 1 ), 0);
	iconSpacingY = max(PrefixedGetRCInt(szSettingsPrefix, TEXT("IconSpacingY"), 1 ), 0);
	int iconSize = PrefixedGetRCInt(szSettingsPrefix, TEXT("IconSize"), 16 );
	TrayIcon::SetIconSize(iconSize);
	TrayIcon::SetParent(hWnd);

	//tray wrapping
	wrapCount = IconListCount(COUNT_PERWRAP);
#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "WrapCount wharf default: %d", wrapCount);
#endif
	wrapCount = max(PrefixedGetRCInt(szSettingsPrefix, TEXT("WrapCount"), (bInWharf ? wrapCount : 0) ), 0);
	nScrollStep = max(PrefixedGetRCInt(szSettingsPrefix, TEXT("IconScrollStep"), 0), 0);

	// this section evaluates how the placing of new icons should be calculated
	deltaX = ((( direction & TD_VERTICAL ) ? wrapDirection : direction ) == TD_LEFT) ? -1 : 1;
	deltaY = ((( direction & TD_VERTICAL ) ? direction : wrapDirection ) == TD_DOWN) ? 1 : -1;

	deltaX *= iconSize + iconSpacingX;
	deltaY *= iconSize + iconSpacingY;
}

void vtray::ReadSizeAndPos()
{
	if ( bInWharf )
	{
		_LSLog(LOG_DEBUG, szLogName, "loaded in wharf, reading wharf settings");
		RECT rc;
		int wharfBorder;

		// get the wharf window size
		GetClientRect( parentWindow, &rc );
		wharfBorder = GetRCInt( TEXT("WharfBevelWidth"), 1 );

#ifdef _DEBUG
		_LSLogPrintf(LOG_DEBUG, szLogName, "wharf: p: %d,%d, s:%d,%d", rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
#endif

		SetPosition(wharfBorder, wharfBorder);
		SetSize(rc.right - 2 * wharfBorder, rc.bottom - 2 * wharfBorder);
		_LSLog(LOG_DEBUG, szLogName, "Done reading wharf settings");
	}
	else
	{
#ifdef _DEBUG
		_LSLog(LOG_DEBUG, szLogName, "not in wharf, reading size settings");
#endif
		SetPosition( PrefixedGetRCCoordinate(szSettingsPrefix, TEXT("X"), 0, GetSystemMetrics(SM_CXSCREEN)),
					PrefixedGetRCCoordinate(szSettingsPrefix, TEXT("Y"), 0, GetSystemMetrics(SM_CYSCREEN))
		);
		SetSize(PrefixedGetRCInt(szSettingsPrefix, TEXT("Width"), 64 ), PrefixedGetRCInt(szSettingsPrefix, TEXT("Height"), 64 ) );
#ifdef _DEBUG
		_LSLog(LOG_DEBUG, szLogName, "Done reading size settings");
#endif
	}

	BOOL hidden = PrefixedGetRCBool(szSettingsPrefix, TEXT("Hidden"), TRUE);
	SetVisible(bInWharf || !hidden);

#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "Hidden: %d, Visible: %d", hidden, IsVisible());
#endif

	bOnTop = !bInWharf && PrefixedGetRCBool(szSettingsPrefix, TEXT("AlwaysOnTop"), TRUE);
}

void vtray::ReadIconHide(void)
{
	// format *Hide "classname" "title(optional)" MinUntilHide
	MultipleConfigReader iconhide(szSettingsPrefix, TEXT("Hide"), true);
	TokenSafeString config;
	int nr, count;
	while (iconhide.HasMoreConfigs())
	{
		config.assign(iconhide.GetNextConfig());
		count = config.Tokenize(4);

		if (count >= 3)
		{
			if (count > 3)
			{
				nr = atoi(config.GetToken(3));
#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "hideconfig adding: \"%s\" tip:\"%s\", hide: %d", config.GetToken(1), config.GetToken(2), nr);
#endif
				hideconfig.AddSetting(config.GetToken(1), config.GetToken(2), nr);
			}
			else
			{
				nr = atoi(config.GetToken(2));
#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "hideconfig adding: \"%s\" (no tip), hide: %d", config.GetToken(1), nr);
#endif
				hideconfig.AddSetting(config.GetToken(1), (LPCTSTR) NULL, nr);
			}
		}
	}
}

void vtray::ReadAddCommands(void)
{
	// format *On Nr "!bangcommand"
	MultipleConfigReader oncmds(szSettingsPrefix, TEXT("On"), true);
	TokenSafeString config;
	int nr, count;
	while (oncmds.HasMoreConfigs())
	{
		config.assign(oncmds.GetNextConfig());
		count = config.Tokenize(2);

		if (count >= 2)
		{
			nr = atoi(config.GetToken(1));
#ifdef _DEBUG
			_LSLogPrintf(LOG_DEBUG, szLogName, "On command for nr: %d", nr);
#endif
			commands.AddNrConfig( nr, config.GetExtra() );
		}
	}
}

// read tray flash lines:
// format *Flash "classname" "title(optional)" flashes/update
void vtray::ReadIconFlash(void)
{
	MultipleConfigReader iconhide(szSettingsPrefix, TEXT("Flash"), true);
	TokenSafeString config;
	int nr, count;
	while (iconhide.HasMoreConfigs())
	{
		config.assign(iconhide.GetNextConfig());
		count = config.Tokenize(4);

		if (count > 2)
		{
			if (count < 4)
			{
				nr = atoi(config.GetToken(2))*2;
#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "flashconfig adding: \"%s\" (no tip), flash: %d", config.GetToken(1), nr);
#endif
				flashconfig.AddSetting(config.GetToken(1), (LPCTSTR) NULL, nr );
			}
			else
			{
				nr = atoi(config.GetToken(3))*2;
#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "flashconfig adding: \"%s\" tip:\"%s\", flash: %d", config.GetToken(1), config.GetToken(2), nr);
#endif
				flashconfig.AddSetting(config.GetToken(1), config.GetToken(2), nr );
			}
		}
	}

	// flash settings
	TrayFlash = PrefixedGetRCBool(szSettingsPrefix, TEXT("ScrollFlash"), TRUE);
	FlashInSS = PrefixedGetRCBool(szSettingsPrefix, TEXT("FlashInScreensaver"), TRUE);
	AlwaysFlash = PrefixedGetRCBool(szSettingsPrefix, TEXT("AlwaysFlashOnMatch"), TRUE);
	timerScroll = max(100, PrefixedGetRCInt(szSettingsPrefix, TEXT("ScrollFlashInterval"), 500));

}

void vtray::AllowSetForeground(HWND hWnd) const
{
    if (m_pAllowSetForegroundWindow)
    {
        DWORD dwProcess = 0;
        GetWindowThreadProcessId(hWnd, &dwProcess);
        m_pAllowSetForegroundWindow(dwProcess);
    }
}