#ifndef __GUIWINDOW_H
#define __GUIWINDOW_H

// if compiling with MSVC++ use its protection against multiple inclusion
#ifdef _MSC_VER // if using Microsoft C++ compiler
#pragma warning( disable : 4786 ) // disable warning C4786
#pragma once
#endif

#include "window.h"

#define ALIGN_OFF		0x0

#define ALIGN_VERTICAL	0x2
#define ALIGN_TOTOPLEFT	0x4

#define ALIGN_RIGHT		0x1
#define ALIGN_LEFT		(ALIGN_TOTOPLEFT)
#define ALIGN_UP		(ALIGN_TOTOPLEFT|ALIGN_VERTICAL)
#define ALIGN_DOWN		(ALIGN_VERTICAL)

class GuiWindow : public Window
{
protected:
	int nXPos;
	int nYPos;

	int nWidth;
	int nHeight;

	// save screen width
	int nScreenWidth;
	int nScreenHeight;

	// aligning of window
	int nAlign;
	bool firstAlignDone;

	bool bInWharf;
	bool bLsBoxed;
	bool bVisible;
	bool bOnTop;
	bool bDraggable;

	HWND hwndParent;

public:
	GuiWindow(LPCSTR szAppName, HWND parentWnd, bool Wharfed);
	~GuiWindow();

	// save settings
	void SetPosition(int x_pos, int y_pos);
	void SetSize(int width, int height);

	void SetAlignment(LPCSTR align);

	// move and size are made virtual to allow enhanced versions
	virtual bool IsDragPoint(int x, int y);
	virtual void Move(int x_pos, int y_pos);
	virtual void MoveBy(int x_pos, int y_pos);
	virtual void Size(int x_size, int y_size);
	virtual void MoveAndSize(int x_pos, int y_pos, int x_size, int y_size);

	void FixVisibility(void);
	void Show(void);
	void Hide(void);
	void ToggleVisibility(void);
	void SetOnTop(bool fAlwaysOnTop);
	void SetDraggable(bool draggable);
	void ToggleOnTop(void);
	void BoxHook(LPCSTR szArgs);

	// create window
	BOOL CreateWnd(LPCSTR szWCName);

	// re-set positions, just in case
	void FixPosition();

	int GetHeight(void);
	int GetWidth(void);
	int GetX(void);
	int GetY(void);
	HWND GetModuleWindow(void);
	HWND GetParentHWND(void);

	void SetLsBoxed(bool);
	void SetTop(bool);
	void SetVisible(bool);
	void SetInWharf(bool);

	bool IsInWharf(void);
	bool IsOnTop(void);
	bool IsVisible(void);
	bool IsLsBoxed(void);
	bool IsDraggable(void);

};




#endif