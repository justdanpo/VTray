#ifndef __STATEBOXPAINTER_H
#define __STATEBOXPAINTER_H

#include "BoxPainter.h"
#include <map>
using std::map;

struct StateSetting {
	bool bBitmap;
	COLORREF clrBack;
	COLORREF clrBorder;
	HBITMAP hbmBack;
};

typedef map<int, StateSetting> StateSettings;

#define BOX_NORMAL 1


class StateBoxPainter : public BoxPainter
{
public:
	StateBoxPainter();
	~StateBoxPainter();

	void SetStateColors(int state, COLORREF clrBg, COLORREF clrBrdr);
	void SetStateBitmap(int state, HBITMAP hbmBg);

	void SetState(int state);

protected:
	StateSettings settings;
	int nCurrentState;

};

#endif
