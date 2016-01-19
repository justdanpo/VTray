/*

	Icon

	Author:	Vendicator

	Description:
	Icon class that encapsulates some basic functionality, including tinting.

	Requires:

	Changelog:
	[2003-08-20 - Vendicator]
	- Using separate bitmap for icon mask.
	- Removed an invalid DeleteObject().
	- Made a define for whether the bitmap version with transparent blit
	  or real icon version should be used.

	[2003-08-09 - Vendicator]
	- Recreating an icon after tinting effects have been applied.

	[2003-07-30 - Vendicator]
	- Fixed possible GDI leak.

	[2003-07-24 - Vendicator]
	- Allowed SetIcon to accept it's own icon for only changing tint.

	[2003-07-18 - Vendicator]
	- Fixed SetPosition rc mix ups.

	[2003-07-16 - Vendicator]
	- First work, split out from TrayIcon class.

*/

#include "Icon.h"
#include "apis.h"

#include "lsapi_location.h"

extern LPCSTR szLogName;

int Icon::nIconSize = 0;
ColorSettings Icon::ColorTintings;
bool Icon::bTransparent;
HWND Icon::hParentWindow;
UINT Icon::uIconCounter = 0;


Icon::Icon() :
	hIcon(NULL),
#ifndef ICONS_BITMAPPED
	hModifiedIcon(NULL),
#endif
	hRgn(NULL),
	hbmBuffer(NULL),
	hdcBuffer(NULL),
	effectFlags(0)
{
	CreateBuffer();

	++uIconCounter;
}

Icon::Icon(HICON hIcon) :
	hIcon(NULL),
#ifndef ICONS_BITMAPPED
	hModifiedIcon(NULL),
#endif
	hRgn(NULL),
	hbmBuffer(NULL),
	hdcBuffer(NULL),
	effectFlags(0)
{
	CreateBuffer();
	SetIcon(hIcon);

	rc.bottom = 0;
	rc.top = 0;
	rc.left = 0;
	rc.right = 0;

	++uIconCounter;
}

Icon::~Icon()
{
#ifdef _DEBUG
	_LSLog(LOG_DEBUG, szLogName, "Icon Desctructor");
#endif
	// remove the icon
	DeleteIcon();
	DeleteBuffer();

	--uIconCounter;
}


void Icon::CreateBuffer()
{
#ifdef _DEBUG
	if (hParentWindow == NULL)
		MessageBox(NULL, "no parent window for icon creation", "Icon Error!", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
#endif

	// create buffer & dc to it
	HDC	hdcScreen = GetDC(hParentWindow);

	hdcBuffer = CreateCompatibleDC(hdcScreen);
	hbmBuffer = CreateCompatibleBitmap(hdcScreen, nIconSize, nIconSize);
	SetBitmapDimensionEx(hbmBuffer, nIconSize, nIconSize, NULL);
	::SelectObject(hdcBuffer, hbmBuffer);

#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "Buffer created with size: %d", nIconSize);
#endif

	ReleaseDC(hParentWindow, hdcScreen);
}

void Icon::DeleteBuffer()
{
	effectFlags = 0;

	// delete old gdi buffers
	if (hbmBuffer)
	{
		DeleteObject(hbmBuffer);
		hbmBuffer = NULL;
	}
	if (hdcBuffer)
	{
		DeleteObject(hdcBuffer);
		hdcBuffer = NULL;
	}
}

void Icon::SetIcon(HICON icon, COLORREF clrHue, UCHAR ucHueIntensity, UCHAR ucSaturation)
{
#ifndef ICONS_BITMAPPED
	ICONINFO modified, standard;
#endif

	// if there is a new icon (not the same)
	if (icon != hIcon || icon == NULL)
		DeleteIcon();

	effectFlags = 0;
	if (ucHueIntensity)
		effectFlags |= FX_ICONHUE;
	if (ucSaturation != 255)
		effectFlags |= FX_SATURATION;

	// copy the new icon
	if (icon && icon != hIcon)
	{
#ifdef _DEBUG
		_LSLogPrintf(LOG_DEBUG, szLogName, "New Icon set (size: %d)", nIconSize);
#endif
		hIcon = CopyIcon(icon);
	}

	// create icon region
	if (hIcon && bTransparent)
	{
		hRgn = ::CreateRectRgn(0, 0, 0, 0);

		HDC hdc = ::CreateCompatibleDC( NULL );
		HBITMAP hbmNew = ::CreateCompatibleBitmap( hdc, nIconSize, nIconSize );
		HBITMAP hbmOld = (HBITMAP) ::SelectObject( hdc, hbmNew );

		if ( DrawIconEx(hdc, 0, 0, hIcon, nIconSize, nIconSize, 0, NULL, DI_MASK) )
		{
			UINT y = 0;
			while ( y < nIconSize )
			{
				UINT x = 0;
				while ( x < nIconSize )
				{
					UINT firstNTX = 0;
					// loop through all transparent pixels...
					while ( x < nIconSize )
					{
						// if the pixel is white, then break
						if ( !::GetPixel(hdc, x, y) )
							break;
						x++;
					}
					// set first non transparent pixel
					firstNTX = x;
					// loop through all non transparent pixels
					while ( x < nIconSize )
					{
						// if the pixel is white, then break
						if ( GetPixel(hdc, x, y) )
							break;
						x++;
					}
					// if found one or more non-transparent pixels in a row, add them to the rgn...
					if ((x - firstNTX) > 0)
					{
						HRGN hTempRgn = CreateRectRgn(firstNTX, y, x, y + 1);
						CombineRgn(hRgn, hRgn, hTempRgn, RGN_OR);
						DeleteObject(hTempRgn);
					}
					x++;
				}
				y++;
			}
		}
		SelectObject( hdc, hbmOld );
		DeleteObject( hbmOld ); // testing
		DeleteObject( hbmNew );
		DeleteDC(hdc);
	}

	// create buffer copy with fx changes applied
	if (effectFlags&FX_ANY)
	{
		if (hIcon)
		{
			UINT y = 0;
			HDC hdcNew = ::CreateCompatibleDC( NULL );
			HDC hdcMask = ::CreateCompatibleDC( NULL );
			HBITMAP hbmNew = ::CreateCompatibleBitmap( hdcBuffer, nIconSize, nIconSize );
			HBITMAP hbmMask = ::CreateCompatibleBitmap( hdcBuffer, nIconSize, nIconSize );
			::SelectObject(hdcNew, hbmNew);
			::SelectObject(hdcMask, hbmMask);

			// temporarily draw up the icon to read it's pixels
			DrawIconEx(hdcNew, 0, 0, hIcon, nIconSize, nIconSize, 0, NULL, DI_NORMAL);
			// the mask of the icon
			DrawIconEx(hdcMask, 0, 0, hIcon, nIconSize, nIconSize, 0, NULL, DI_MASK);

			while (y < nIconSize)
			{
				UINT x = 0;
				while (x < nIconSize)
				{
					// loop through all transparent pixels...
					while (x < nIconSize)
					{
						// if the mask-pixel is white, then break
						if ( !GetPixel(hdcMask, x, y) )
						{
							COLORREF cl = ::GetPixel(hdcNew, x, y);
							BYTE r = GetRValue(cl);
							BYTE g = GetGValue(cl);
							BYTE b = GetBValue(cl);

							// saturation effect
							if (effectFlags & FX_SATURATION)
							{
								BYTE gray = (BYTE)(r*0.3086+g*0.6094+b*0.0820);

								r = (BYTE)((r*ucSaturation+gray*(255-ucSaturation)+255)>>8);
								g = (BYTE)((g*ucSaturation+gray*(255-ucSaturation)+255)>>8);
								b = (BYTE)((b*ucSaturation+gray*(255-ucSaturation)+255)>>8);
							}
							// icon hue effect
							if (effectFlags & FX_ICONHUE)
							{
								// the B & R values of the hue is swapped here, can somebody tell me why it only works that way?
								// test normal
								r = (BYTE)((r*(255-ucHueIntensity)+GetRValue(clrHue)*ucHueIntensity+255)>>8);
								g = (BYTE)((g*(255-ucHueIntensity)+GetGValue(clrHue)*ucHueIntensity+255)>>8);
								b = (BYTE)((b*(255-ucHueIntensity)+GetBValue(clrHue)*ucHueIntensity+255)>>8);
							}
							//_LSLogPrintf(LOG_DEBUG, szLogName, "working on pixel: %d,%d", x, y);
							SetPixel(hdcBuffer, x, y, RGB(r, g, b));
						}
#ifdef ICONS_BITMAPPED
						// for transparent blt
						else
							SetPixel(hdcBuffer, x, y, RGB(255, 0, 255));
#endif
						x++;
					}
				}
				y++;
			}

			//SelectObject(hdc, hbmOld);
			DeleteObject(hbmNew);
			DeleteDC(hdcNew);
			DeleteObject(hbmMask);
			DeleteDC(hdcMask);

#ifndef ICONS_BITMAPPED
			// create an icon from the buffer
			GetIconInfo(hIcon, &standard);
			//modified = standard;
			modified.fIcon = TRUE;
			modified.hbmMask = standard.hbmMask;
			modified.hbmColor = hbmBuffer;
			SelectObject(hdcBuffer, NULL);
			hModifiedIcon = CreateIconIndirect(&modified);
			SelectObject(hdcBuffer, hbmBuffer);

			DeleteObject(standard.hbmMask);
			DeleteObject(standard.hbmColor);
			// DeleteObject(modified.hbmColor); // = hbmBuffer, deleted with icon
			// DeleteObject(modified.hbmMask); // same as std
#endif
		}
	}
}

void Icon::DeleteIcon()
{
	if (hRgn)
	{
		DeleteObject(hRgn);
		hRgn = NULL;
	}

	if (hIcon)
	{
		DestroyIcon(hIcon);
		hIcon = NULL;
	}

#ifndef ICONS_BITMAPPED
	if (hModifiedIcon)
	{
		DestroyIcon(hModifiedIcon);
		hModifiedIcon = NULL;
	}
#endif
}

void Icon::Paint(HDC hdcDst, HRGN hrgnDst, LPRECT updateRC)
{
	// don't paint if there is no reason to (not included in update rectangle)
	RECT intersection;
	if (updateRC != NULL && !IntersectRect(&intersection, &rc, updateRC) )
		return;

	HRGN hrgn = (HRGN)hRgn;

	#ifdef DEBUG_SPAM
		char title[128];
		memset(title, 0, sizeof(title)/sizeof(title[0]) );
		GetWindowText( hWnd, title, sizeof(title)/sizeof(title[0]) );
		_LSLogPrintf(LOG_DEBUG, szLogName, "painting icon, owner window title=\"%s\"", title);
	#endif

	if(hIcon)
	{
		if (effectFlags & FX_ANY)
		{
			// paint from the buffer
#ifdef ICONS_BITMAPPED
			TransparentBltLS(hdcDst, rc.left, rc.top, nIconSize, nIconSize, hdcBuffer, 0, 0, RGB(255, 0, 255));
#else
			DrawIconEx(hdcDst, rc.left, rc.top, hModifiedIcon, nIconSize, nIconSize, 0, NULL, DI_NORMAL);
#endif
		}
		else
			DrawIconEx(hdcDst, rc.left, rc.top, hIcon, nIconSize, nIconSize, 0, NULL, DI_NORMAL);

		// combine the region
		if (hrgn && bTransparent)
		{
			OffsetRgn(hrgn, rc.left, rc.top);
			CombineRgn(hrgnDst, hrgnDst, hrgn, RGN_OR);
			OffsetRgn(hrgn, -rc.left, -rc.top);
		}
	}
}

void Icon::SetPosition(const int nXLeft, const int nYTop)
{
	rc.top = nYTop;
	rc.left = nXLeft;

	rc.bottom = rc.top + nIconSize;
	rc.right = rc.left + nIconSize;

#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "Icon.cpp: Setting Position (int) x: %d, y:%d", rc.left, rc.top);
#endif
}

void Icon::SetPosition(const RECT srcRC)
{
	CopyRect(&rc, &srcRC);
#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "Icon.cpp: Setting Position (rc) x: %d, y:%d", rc.left, rc.top);
#endif
}

void Icon::SetPosition(const Icon* otherIcon)
{
	CopyRect(&rc, &otherIcon->rc);
#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "Icon.cpp: Setting Position (icon) x: %d, y:%d", rc.left, rc.top);
#endif
}

// position the icon relative to the other one
void Icon::SetRelativePosition(const Icon* otherIcon, const int deltaX, const int deltaY)
{
	rc.top = otherIcon->rc.top + deltaY;
	rc.left = otherIcon->rc.left + deltaX;

	rc.bottom = rc.top + nIconSize;
	rc.right = rc.left + nIconSize;
}

bool Icon::IsPointOver(const POINT& pt)
{
	return (PtInRect(&rc, pt) == TRUE);
}


// static

void Icon::SetTransparentBg(bool transparent)
{
	bTransparent = transparent;
}

void Icon::SetParent(HWND hwnd)
{
	hParentWindow = hwnd;
}

void Icon::SetIconSize(const int size)
{
	nIconSize = max(size, 1);
}

int Icon::GetIconSize()
{
#ifdef _DEBUG
	if (nIconSize == 0)
		MessageBox(NULL, TEXT("nIconSize not set"), TEXT("Error"), MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
#endif
	return nIconSize;
}

void Icon::AddColorSetting(COLORREF clrHue, UCHAR cHueIntensity, UCHAR cSaturation, UCHAR cID)
{
	ColorSetting newTint = {
		clrHue,
		cHueIntensity,
		cSaturation
	};

#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "Icon.cpp: New colorpreset (%d): hue %d,%d,%d, intensity %d, saturation %d", cID, GetRValue(clrHue), GetGValue(clrHue), GetBValue(clrHue), cHueIntensity, cSaturation);
#endif

	ColorTintings.insert( std::make_pair(cID, newTint) );
}