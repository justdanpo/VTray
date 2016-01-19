#ifndef __ICON_H
#define __ICON_H

#ifdef _MSC_VER // if using Microsoft C++ compiler
#pragma warning( disable : 4786 ) // disable warning C4786
#endif

#include <windows.h>
#include <map>
using std::map;

// if using bitmap with transparent blt or "real" icon functions
//#define BITMAPPED

// defined effects
#define FX_ICONHUE		0x1
#define FX_SATURATION	0x2
#define FX_ANY			(FX_ICONHUE|FX_SATURATION)

struct ColorSetting
{
	COLORREF clrHue;
	UCHAR cHueIntensity;
	UCHAR cSaturation;
};

typedef map<UCHAR, ColorSetting> ColorSettings;

class Icon
{
public:
	Icon();
	Icon(HICON hIcon);
	~Icon();

	void CreateBuffer();
	void DeleteBuffer();
	virtual void SetIcon(HICON icon, COLORREF clrHue = 0, UCHAR ucHueIntensity = 0, UCHAR ucSaturation = 255);
	void DeleteIcon();

	void Paint(HDC hdcDst, HRGN hrgnDst, LPRECT updateRC);

	void SetPosition(const int nXLeft, const int nYTop);
	void SetPosition(const RECT srcRC);
	void SetPosition(const Icon* otherIcon);
	void SetRelativePosition(const Icon* otherIcon, const int deltaX, const int deltaY);
	bool IsPointOver(const POINT& pt);

	static int GetIconSize();

	static void SetParent(HWND hwnd);
	static void SetTransparentBg(bool transparent);
	static void SetIconSize(const int size);
	static void AddColorSetting(COLORREF clrHue, UCHAR cHueIntensity, UCHAR cSaturation, UCHAR cID);


public:
#ifndef BITMAPPED
	// modified icon
	HICON hModifiedIcon;
#endif
	// simple copy of the icon
	HICON hIcon;
	// icon region
	HRGN hRgn;
	// trayicon rectangle
	RECT rc;
	// buffer hbitmap
	HBITMAP hbmBuffer;
	HDC hdcBuffer;

protected:
	UCHAR effectFlags;

	static HWND hParentWindow;
	static bool bTransparent;
	static int nIconSize;
	static UINT uIconCounter;

	static ColorSettings ColorTintings;

};

#endif