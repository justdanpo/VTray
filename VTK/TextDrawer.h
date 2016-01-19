#ifndef __TEXTDRAWER_H
#define __TEXTDRAWER_H

#include <windows.h>
#include "../VTK/SafeString.h"


class TextDrawer
{
public:
	TextDrawer(bool adaptregion, LPCTSTR text = NULL);
	TextDrawer(bool adaptregion, LPCTSTR text, int x, int y, int w, int h);
	~TextDrawer();

	void SetText(LPCTSTR text);

	void SetPosition(int x_pos, int y_pos);
	void SetSize(int width, int height);
	void SetRect(int x_pos, int y_pos, int width, int height);
	void SetRect(const RECT rc);
	void SetBorder(int border);

	void LoadFont(LPCTSTR fontname, int fontsize, bool bold, bool italic);
	void SetProperties(int horiJustify, int vertJustify, bool workbreak);
	void SetColors(COLORREF text, COLORREF back);
	void SetShadow(COLORREF shadow, int xpitch, int ypitch);
	void SetShadow(COLORREF shadow, int shadowdepth);

	void DrawText(const HDC hdc, HRGN hrgn, LPCTSTR text, int x, int y, int w, int h);
	void Draw(const HDC hdc, HRGN hrgn = NULL);


protected:
	SafeString szText;

	HFONT hFont;
	RECT rcOrg;
	RECT rcText;
	int nBorder;

	COLORREF clrText;
	COLORREF clrBack;
	COLORREF clrShadow;

	int nXShadowPitch;
	int nYShadowPitch;

	int nTextProps;
	int nTextHeight;

	bool bWordBreak;
	bool bBold;
	bool bItalic;
	bool bShadow;
	bool bAdaptRegion;

};

#endif