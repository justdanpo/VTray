#ifndef __BOXPAINTER_H
#define __BOXPAINTER_H

#include <windows.h>

class BoxPainter
{
public:
	BoxPainter();
	~BoxPainter();

	void SetWindow(HWND hwnd);
	void SetSize(int width, int height);

	void SetColors(COLORREF clrBg, COLORREF clrBrdr);
	void SetBitmap(HBITMAP hbmBg);
	void SetTiled(bool tile);
	void SetBorders(int nLeft, int nTop, int nRight, int nBottom);

	bool IsTransparent() const;
	bool IsUsingBitmap() const;
	void GetBitmapSize(int* cX, int* cY);

	void PaintBackground(HDC hdcDst, HRGN hrgnDst);
	bool CreateBackground();

	BltSize(HDC hdcDst, int xDst, int yDst, int cxDst, int cyDst, HDC hdcSrc, int xSrc, int ySrc, int cxSrc, int cySrc);
	BltCopy(HDC hdcDst, int xDst, int yDst, int cxDst, int cyDst, HDC hdcSrc, int xSrc, int ySrc);

	void CleanUp();

protected:
	HWND hPaintWnd;

	bool bBitmap;
	bool bSkinTiled;
	bool bTransSkin;
	bool bTransBack;

	// size
	int nWidth;
	int nHeight;

	// borders
	int nBorderTop;
	int nBorderLeft;
	int nBorderRight;
	int nBorderBottom;

	HBITMAP hbmBitmap;
	HBITMAP hbmPaintBg;
	HRGN hrgnBack;

	COLORREF clrBack;
	COLORREF clrBorder;

};

#endif