/*

	BoxPainter

	Author:	Vendicator
	
	Description:
	Class that encapsulates box skinning of a window.
	Either uses a bitmap or colors (border/middle bg).

	Requires:

	Changelog:
	[2003-08-02 - Vendicator]
	- Fixed bug with new colors not getting painted.
	- More logical naming of the two bitmaps (as-on-disk/stretched).
	- Default bg/border colors and border sizes.

	[2003-07-25 - Vendicator]
	- Filling bg with magic pink when using transparent bitmaps.

	[2003-07-19 - Vendicator]
	- First work, split out from systray code (by Maduin/grd).
	
*/

#include "BoxPainter.h"
#include "apis.h"
#include "lsapi_location.h"

#define MAGICPINK RGB(255, 0, 255)

extern LPCSTR szLogName;


BoxPainter::BoxPainter() :
	hbmBitmap(NULL),
	hbmPaintBg(NULL),
	hrgnBack(NULL),
	hPaintWnd(NULL),
	bTransBack(false),
	bSkinTiled(false),
	clrBack(0),
	clrBorder(0),
	nWidth(0),
	nHeight(0),
	nBorderLeft(0),
	nBorderRight(0),
	nBorderTop(0),
	nBorderBottom(0)
{
}

BoxPainter::~BoxPainter()
{
#ifdef _DEBUG
	_LSLog(LOG_DEBUG, szLogName, "BoxPainter Desctructor");
#endif
	CleanUp();
}

bool BoxPainter::IsTransparent() const
{
	return bTransBack;
}

bool BoxPainter::IsUsingBitmap() const
{
	return bBitmap;
}

void BoxPainter::GetBitmapSize(int* cX, int* cY)
{
	BITMAP bm;
	GetObject( hbmBitmap, sizeof(BITMAP), &bm );
	*cX = bm.bmWidth;
	*cY = bm.bmHeight;
}

// Needed in order to acquire a compatible DC
void BoxPainter::SetWindow(HWND hwnd)
{
	hPaintWnd = hwnd;
}

void BoxPainter::SetSize(int width, int height)
{
	nWidth = width;
	nHeight = height;
}

void BoxPainter::SetTiled(bool tile)
{
	bSkinTiled = tile;
}

void BoxPainter::SetBorders(int nLeft, int nTop, int nRight, int nBottom)
{
	nBorderLeft = nLeft;
	nBorderTop = nTop;
	nBorderRight = nRight;
	nBorderBottom = nBottom;
}

void BoxPainter::CleanUp()
{
	// delete gdi objects...
	bBitmap = false;

	if (hbmBitmap)
	{
		DeleteObject( hbmBitmap );
		hbmBitmap = NULL;
	}
	if (hbmPaintBg)
	{
		DeleteObject( hbmPaintBg );
		hbmPaintBg = NULL;
	}
	if (hrgnBack)
	{
		DeleteObject( hrgnBack );
		hrgnBack = NULL;
	}
}

void BoxPainter::SetColors(COLORREF clrBg, COLORREF clrBrdr)
{
	CleanUp();

	bBitmap = false;
	clrBack = clrBg;
	clrBorder = clrBrdr;

#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "Setting colors to: %d,%d,%d - border: %d,%d,%d", GetRValue(clrBg),GetGValue(clrBg),GetBValue(clrBg), GetRValue(clrBrdr),GetGValue(clrBrdr),GetBValue(clrBrdr) );
#endif

	bTransSkin = (clrBack == MAGICPINK);
	if (!bTransSkin)
		bTransSkin = (nBorderTop + nBorderLeft + nBorderRight + nBorderBottom > 0) && (clrBorder == MAGICPINK);
}

// handle to the bitmap used for painting
void BoxPainter::SetBitmap(HBITMAP hbmBg)
{
	BITMAP bm;
	HRGN hBitmapRgn, hOpaqueRgn;

	CleanUp();

	bBitmap = true;
	hbmBitmap = hbmBg;

	// get info from the bitmap
	GetObject( hbmBitmap, sizeof(BITMAP), &bm );

	// get color nearest to magic pink, if palettes are screwing things up
	HDC hdcScreen = GetDC(hPaintWnd);
	hBitmapRgn = BitmapToRegion( hbmBitmap, GetNearestColor(hdcScreen, MAGICPINK), 0x101010, 0, 0);
	hOpaqueRgn = CreateRectRgn( 0, 0, bm.bmWidth, bm.bmHeight );
	ReleaseDC(hPaintWnd, hdcScreen);

	// to see if any pixels are transparent
	bTransSkin = !EqualRgn( hBitmapRgn, hOpaqueRgn );

#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "Bitmap transparent: %d", bTransSkin);
#endif

	DeleteObject( hBitmapRgn );
	DeleteObject( hOpaqueRgn );
}

bool BoxPainter::CreateBackground()
{
	HDC hdcScreen, hdcDst;
	HBITMAP hbmDstOld;
	COLORREF magic_pink;

	// delete old objects if necessary...
	if ( hbmPaintBg )
	{
		DeleteObject( hbmPaintBg );
		hbmPaintBg = NULL;
	}
	if ( hrgnBack )
	{
		DeleteObject( hrgnBack );
		hrgnBack = NULL;
	}

#ifdef _DEBUG
	if (hPaintWnd == NULL)
		MessageBox(NULL, TEXT("BoxPainter could not create a background cause it does not know which window it belongs to"), TEXT("BoxPainter Initialization Error!"), MB_OK | MB_ICONERROR | MB_SETFOREGROUND);

	if (nWidth == 0 || nHeight == 0)
		MessageBox(NULL, TEXT("BoxPainter could not create a background cause it doesn't have a size"), TEXT("BoxPainter Initialization Error!"), MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
#endif

	// initiate a new destination dc
	hdcScreen = GetDC(hPaintWnd);

	hdcDst = CreateCompatibleDC(hdcScreen);
	hbmPaintBg = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
	hbmDstOld = (HBITMAP)SelectObject(hdcDst, hbmPaintBg);
	magic_pink = GetNearestColor(hdcScreen, MAGICPINK);

	ReleaseDC(hPaintWnd, hdcScreen);

	// if the background colors are needed put them on...
	if (!hbmBitmap)
	{
		RECT r;
		HBRUSH brush;

		// fill the border with the border colors
		if (nBorderTop || nBorderLeft || nBorderBottom || nBorderTop)
		{
			brush = CreateSolidBrush(clrBorder);

			if (nBorderTop)
			{
				SetRect(&r, 0, 0, nWidth, nBorderTop);
				FillRect(hdcDst, &r, brush);
			}
			if (nBorderLeft)
			{
				SetRect(&r, 0, nBorderTop, nBorderLeft, nHeight - nBorderBottom);
				FillRect(hdcDst, &r, brush);
			}
			if (nBorderRight)
			{
				SetRect(&r, nWidth - nBorderRight, nBorderTop, nWidth, nHeight - nBorderBottom);
				FillRect(hdcDst, &r, brush);
			}
			if (nBorderBottom)
			{
				SetRect(&r, 0, nHeight - nBorderBottom, nWidth, nHeight);
				FillRect(hdcDst, &r, brush);
			}
			DeleteObject(brush);
		}

		// now fill the inner rectangle (excluding the borders) with the background color
		brush = CreateSolidBrush(clrBack);

		SetRect(&r, nBorderLeft, nBorderTop, nWidth - nBorderRight, nHeight - nBorderBottom);
		FillRect(hdcDst, &r, brush);

		DeleteObject(brush);
	}
	else if (bTransSkin)
	{
		// fill bg with transparent color
		RECT r;
		HBRUSH brush = CreateSolidBrush( magic_pink );

		SetRect(&r, 0, 0, nWidth, nHeight);
		FillRect(hdcDst, &r, brush);

		DeleteObject(brush);
	}

	// if a background image is provided, put it on...
	if (hbmBitmap)
	{
		HDC hdcSrc;
		HBITMAP hbmSrcOld;
		BITMAP bm;
		int innerWidth, innerHeight, bmInnerWidth, bmInnerHeight;

		// get info from the bitmap
		GetObject(hbmBitmap, sizeof(BITMAP), &bm);

		// this is to prevent negative values on the width
		innerWidth = max(nWidth - (nBorderLeft + nBorderRight), 0);
		innerHeight = max(nHeight - (nBorderTop + nBorderBottom), 0);
		bmInnerWidth = max(bm.bmWidth - (nBorderLeft + nBorderRight), 1);
		bmInnerHeight = max(bm.bmHeight - (nBorderTop + nBorderBottom), 1);

		hdcSrc = CreateCompatibleDC(hdcDst);
		hbmSrcOld = (HBITMAP)SelectObject(hdcSrc, hbmBitmap);

		if (nBorderTop || nBorderLeft || nBorderBottom || nBorderTop)
		{

			// paint the four corners first, and then the borders
			// topleft, topright, bottomright, bottomleft
			BltCopy(hdcDst, 0, 0, nBorderLeft, nBorderTop, hdcSrc, 0, 0);
			BltCopy(hdcDst, nWidth - nBorderRight, 0, nBorderRight, nBorderTop, hdcSrc, bm.bmWidth - nBorderRight, 0);
			BltCopy(hdcDst, nWidth - nBorderRight, nHeight - nBorderBottom, nBorderRight, nBorderBottom, hdcSrc, bm.bmWidth - nBorderRight, bm.bmHeight - nBorderBottom);
			BltCopy(hdcDst, 0, nHeight - nBorderBottom, nBorderLeft, nBorderBottom, hdcSrc, 0, bm.bmHeight - nBorderBottom);

			if (nBorderTop && innerWidth)
				BltSize(hdcDst, nBorderLeft, 0, innerWidth, nBorderTop, hdcSrc, nBorderLeft, 0, bmInnerWidth, nBorderTop);
			if (nBorderLeft && innerHeight)
				BltSize(hdcDst, 0, nBorderTop, nBorderLeft, innerHeight, hdcSrc, 0, nBorderTop, nBorderLeft, bmInnerHeight);
			if (nBorderRight && innerHeight)
				BltSize(hdcDst, nWidth - nBorderRight, nBorderTop, nBorderRight, innerHeight, hdcSrc, bm.bmWidth - nBorderRight, nBorderTop, nBorderRight, bmInnerHeight);
			if (nBorderBottom && innerWidth)
				BltSize(hdcDst, nBorderLeft, nHeight - nBorderBottom, innerWidth, nBorderBottom, hdcSrc, nBorderLeft, bm.bmHeight - nBorderBottom, bmInnerWidth, nBorderBottom);
		}

		// draw area in the middle
		if (innerWidth && innerHeight)
			BltSize(hdcDst, nBorderLeft, nBorderTop, innerWidth, innerHeight, hdcSrc, nBorderLeft, nBorderTop, bmInnerWidth, bmInnerHeight);

		SelectObject( hdcSrc, hbmSrcOld );
		DeleteDC( hdcSrc );
	}

	SelectObject( hdcDst, hbmDstOld );
	DeleteDC( hdcDst );

	if (!bTransSkin)
	{
		bTransBack = false;
	}
	else
	{
		HRGN hOpaqueRgn = CreateRectRgn(0, 0, nWidth, nHeight);
		hrgnBack = BitmapToRegion(hbmPaintBg, magic_pink, 0x101010, 0, 0);

		// to see if any pixels are transparent
		bTransBack = !EqualRgn(hrgnBack, hOpaqueRgn);

		DeleteObject(hOpaqueRgn);

#ifdef _DEBUG
		_LSLogPrintf(LOG_DEBUG, szLogName, "Background transparent: %d", bTransBack);
#endif

		if (!bTransBack)
		{
			DeleteObject(hrgnBack);
			hrgnBack = NULL;
		}
	}

	return bTransBack;
}

// paints the background and fills the HRGN with the bg region 
void BoxPainter::PaintBackground(HDC hdcDst, HRGN hrgnDst)
{
	HDC hdcSrc;
	HBITMAP hbmOld;

	// if the background isn't there, make one...
	if (!hbmPaintBg)
	{
		CreateBackground();
	}
	else
	{
		BITMAP bm;
		GetObject(hbmPaintBg, sizeof(BITMAP), &bm);
		if (bm.bmWidth != nWidth || bm.bmHeight != nHeight)
		{
			CreateBackground();
		}
	}

	// if there is no background now... do nothing
	if (!hbmPaintBg)
		return ;

	hdcSrc = CreateCompatibleDC(hdcDst);
	hbmOld = (HBITMAP)SelectObject(hdcSrc, hbmPaintBg);

	BitBlt(hdcDst, 0, 0, nWidth, nHeight, hdcSrc, 0, 0, SRCCOPY);

	SelectObject(hdcSrc, hbmOld);
	DeleteDC(hdcSrc);

	if ( hrgnBack && bTransBack && hrgnDst )
		CombineRgn(hrgnDst, hrgnBack, NULL, RGN_COPY);
}

// grd Blitting functions...
// to enable trans / not trans painting
int BoxPainter::BltCopy(HDC hdcDst, int xDst, int yDst, int cxDst, int cyDst, HDC hdcSrc, int xSrc, int ySrc)
{
	if (bTransSkin)
		TransparentBltLS(hdcDst, xDst, yDst, cxDst, cyDst, hdcSrc, xSrc, ySrc, MAGICPINK);
	else
		return BitBlt(hdcDst, xDst, yDst, cxDst, cyDst, hdcSrc, xSrc, ySrc, SRCCOPY);

	return 0;
}

// to enable tile / stretch resizing
int BoxPainter::BltSize(HDC hdcDst, int xDst, int yDst, int cxDst, int cyDst, HDC hdcSrc, int xSrc, int ySrc, int cxSrc, int cySrc)
{
	HDC	hdcTmp;
	HBITMAP hbmTmp, hbmOld;
	int tmpX, tmpY;

	if (bTransSkin)
	{
		hdcTmp = CreateCompatibleDC( hdcDst );
		hbmTmp = CreateCompatibleBitmap( hdcDst, cxDst, cyDst );
		hbmOld = (HBITMAP)SelectObject( hdcTmp, hbmTmp );
		tmpX = 0, tmpY = 0;
	}
	else
	{
		hdcTmp = hdcDst;
		tmpX = xDst, tmpY = yDst;
	}

	if (bSkinTiled)
	{
		int x, y;
		// fill the first row of images
		for (x = 0; x < cxDst; x += cxSrc)
			BitBlt(hdcTmp, tmpX + x, tmpY, min(cxSrc, cxDst - x), min(cySrc, cyDst), hdcSrc, xSrc, ySrc, SRCCOPY);
		// copy this row down the y axis...
		for (y = cySrc; y < cyDst; y += cySrc)
			BitBlt(hdcTmp, tmpX, tmpY + y, cxDst, min(cySrc, cyDst - y), hdcTmp, tmpX, tmpY, SRCCOPY);
	}
	else
	{
		// stretch it
		StretchBlt(hdcTmp, tmpX, tmpY, cxDst, cyDst, hdcSrc, xSrc, ySrc, cxSrc, cySrc, SRCCOPY);
	}

	// blit the resulting image onto the destination dc
	if (bTransSkin)
	{
		TransparentBltLS(hdcDst, xDst, yDst, cxDst, cyDst, hdcTmp, tmpX, tmpY, MAGICPINK);

		// cleanup
		SelectObject( hdcTmp, hbmOld );
		DeleteObject( hbmTmp );
		DeleteDC( hdcTmp );
	}
	return 0;
}