/*

	TextDrawer

	Author:	Vendicator
	
	Description:
	Class for drawing text w/wo shadow.

	Requires:
	SafeString class.

	Changelog:
	[2003-08-04 - Vendicator]
	- Added support for calculating the rectangle if no size is entered.

	[2003-07-28 - Vendicator]
	- Added shadows.
	- Using TCHAR and SafeString.
	
*/

#include <windows.h>
#include "TextDrawer.h"
#include "../VTK/apis.h"

extern LPCSTR szLogName;

TextDrawer::TextDrawer(bool adaptregion, LPCTSTR text) :
	hFont(NULL),
	clrText(NULL),
	clrBack(NULL),
	nTextProps(NULL),
	nTextHeight(0),
	bWordBreak(false),
	bBold(false),
	bItalic(false),
	bShadow(false),
	bAdaptRegion(adaptregion),
	nBorder(0)
{
	SetRect(0, 0, 0, 0);

	SetText(text);
}

TextDrawer::TextDrawer(bool adaptregion, LPCTSTR text, int x, int y, int w, int h) :
	hFont(NULL),
	clrText(NULL),
	clrBack(NULL),
	nTextProps(NULL),
	nTextHeight(0),
	bWordBreak(false),
	bBold(false),
	bItalic(false),
	bShadow(false),
	bAdaptRegion(adaptregion),
	nBorder(0)
{
	SetRect(x, y, w, h);

	SetText(text);
}

TextDrawer::~TextDrawer()
{
	if (hFont)
	{
		DeleteObject(hFont);
		hFont = NULL;
	}

	SetText(NULL);
}

void TextDrawer::LoadFont(LPCTSTR fontname, int fontsize, bool bold, bool italic)
{
	if (hFont)
	{
		DeleteObject(hFont);
		hFont = NULL;
	}

	LOGFONT font;
	memset(&font, 0, sizeof(LOGFONT));

	// copy the font name into struct
	strncpy(font.lfFaceName, fontname, 32);

	font.lfItalic = italic;
	font.lfWeight = bold ? FW_BOLD : FW_NORMAL;
	font.lfHeight = fontsize;
	// font.lfCharSet = ANSI_CHARSET; // DEFAULT_CHARSET?

	// load the font
	hFont = CreateFontIndirect(&font);
}

void TextDrawer::SetProperties(int horiJustify, int vertJustify, bool workbreak)
{
	// text justification in text print
	nTextProps |= horiJustify;
	nTextProps |= vertJustify;

	// word break
	nTextProps |= (workbreak) ? DT_WORDBREAK : DT_SINGLELINE;
}

void TextDrawer::SetBorder(int border)
{
	nBorder = border;
}

void TextDrawer::SetRect(const RECT rc)
{
	CopyRect(&rcOrg, &rc);
	CopyRect(&rcText, &rc);
}

void TextDrawer::SetRect(int x, int y, int w, int h)
{
	rcOrg.right = x + w;
	rcOrg.left = x;
	
	rcOrg.bottom = y + h;
	rcOrg.top = y;

	CopyRect(&rcText, &rcOrg);
}

void TextDrawer::SetPosition(int x, int y)
{
	// keep old size
	rcOrg.right = x + (rcOrg.right - rcOrg.left);
	rcOrg.left = x;
	
	rcOrg.bottom = y + (rcOrg.bottom - rcOrg.top);
	rcOrg.top = y;

	// copy it for use in Draw()
	CopyRect(&rcText, &rcOrg);
}

void TextDrawer::SetSize(int w, int h)
{
	rcOrg.right = rcOrg.left + w;
	rcOrg.bottom = rcOrg.top + h;

	// copy it for use in Draw()
	CopyRect(&rcText, &rcOrg);
}

void TextDrawer::SetText(LPCTSTR text)
{
	szText = text;	
}

void TextDrawer::SetColors(COLORREF text, COLORREF back)
{
	clrText = text;
	clrBack = back;
}

void TextDrawer::SetShadow(COLORREF shadow, int shadowdepth)
{
	SetShadow(shadow, shadowdepth, shadowdepth);
}

void TextDrawer::SetShadow(COLORREF shadow, int xpitch, int ypitch)
{
	if ( xpitch != 0 || ypitch != 0 )
	{
		bShadow = true;
#ifdef _DEBUG
		_LSLogPrintf(LOG_DEBUG, szLogName, "Text shadowing activated: %d %d", xpitch, ypitch);
#endif
	}
	
	clrShadow = shadow;
	nXShadowPitch = xpitch;
	nYShadowPitch = ypitch;
}

void TextDrawer::Draw(const HDC hdc, HRGN hrgn)
{
	HFONT oldFont;
	int height;

	oldFont = (HFONT)SelectObject(hdc, hFont);
	if ( clrBack != RGB(255, 0, 255) )
	{
		SetBkMode(hdc, OPAQUE);
		SetBkColor(hdc, clrBack);
	}
	else
	{
		SetBkMode(hdc, TRANSPARENT);
	}

	// if the user didn't know which size he wanted
	if (rcOrg.right - rcOrg.left == 0 || rcOrg.top - rcOrg.bottom == 0)
	{
		::DrawText(hdc, szText.c_str(), szText.length(), &rcText, DT_CALCRECT);
		SetSize(rcText.right-rcText.left, rcText.bottom-rcText.top);
	}

	// speciall handling since MS don't want to vertically align with multiple lines
	if (!(nTextProps&DT_SINGLELINE))
	{
		if (nTextProps&DT_VCENTER)
		{
			height = ::DrawText(hdc, szText.c_str(), szText.length(), &rcText, DT_CALCRECT);
			CopyRect(&rcText, &rcOrg);
			rcText.top = (rcOrg.bottom+rcOrg.top-height)/2; // middle: rcText.bottom-rcText.top/2 (move up:) - height/2
		}
		else if (nTextProps&DT_BOTTOM)
		{
			height = ::DrawText(hdc, szText.c_str(), szText.length(), &rcText, DT_CALCRECT);
			CopyRect(&rcText, &rcOrg);
			rcText.top = rcOrg.bottom - height; // place at bottom
		}
	}

	RECT rcArea;
	CopyRect(&rcArea, &rcText);

	// add a border around the text rectangle
	if (nBorder)
	{
		// push the text rect for the amount of the border
		CopyRect(&rcArea, &rcText);

		// shrink the text area by how large the borders are
		rcText.left += nBorder;
		rcText.top += nBorder;
		rcText.right -= nBorder;
		rcText.bottom -= nBorder;

		// fill rect with bg color
		HBRUSH bgbrush = CreateSolidBrush(clrBack);
		HBRUSH oldbrush;

		// save old brush
		oldbrush = (HBRUSH)SelectObject(hdc, bgbrush);
		// paint with new brush
		FillRect(hdc, &rcArea, bgbrush);
		// insert old brush again
		SelectObject(hdc, oldbrush);
	}

	if (bShadow)
	{
		RECT rcShadow;
		CopyRect(&rcShadow, &rcText);
		OffsetRect(&rcShadow, nXShadowPitch, nYShadowPitch);

		// adapt the region to include this text area
		if (bAdaptRegion)
		{		
#ifdef _DEBUG
			_LSLogPrintf(LOG_DEBUG, szLogName, "adapting region: l%d t%d r%d b%d", rcShadow.left, rcShadow.top, rcShadow.right, rcShadow.bottom);
#endif
			HRGN hrgnshadow = CreateRectRgnIndirect(&rcShadow);
			CombineRgn(hrgn, hrgn, hrgnshadow, RGN_OR);
		}

		SetTextColor(hdc, clrShadow);
		::DrawText(hdc, szText.c_str(), szText.length(), &rcShadow, nTextProps);
	}

	// adapt the region to include this text area
	if (bAdaptRegion)
	{
#ifdef _DEBUG
			_LSLogPrintf(LOG_DEBUG, szLogName, "adapting region: l%d t%d r%d b%d", rcArea.left, rcArea.top, rcArea.right, rcArea.bottom);
#endif
		HRGN hrgntext = CreateRectRgnIndirect(&rcArea);
		CombineRgn(hrgn, hrgn, hrgntext, RGN_OR);
	}

	SetTextColor(hdc, clrText);
	::DrawText(hdc, szText.c_str(), szText.length(), &rcText, nTextProps);

	SelectObject(hdc, oldFont);
}

void TextDrawer::DrawText(const HDC hdc, HRGN hrgn, LPCTSTR text, int x, int y, int w, int h)
{
	SetText(text);
	SetRect(x, y, w, h);

	Draw(hdc, hrgn);
}