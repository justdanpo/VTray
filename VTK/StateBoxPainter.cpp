/*

	StateBoxPainter

	Author:	Vendicator
	
	Description:
	Class building on BoxPainter to select different bgs.

	Requires:
	BoxPainter

	Changelog:
	[2003-08-02 - Vendicator]
	- First work.
	
*/

#include "StateBoxPainter.h"

StateBoxPainter::StateBoxPainter() :
	nCurrentState(0)
{
}

StateBoxPainter::~StateBoxPainter()
{
	for (StateSettings::iterator i = settings.begin(); i != settings.end(); ++i)
	{
		if ( i->second.bBitmap )
		{
			DeleteObject( i->second.hbmBack );
		}
	}
	hbmBitmap = NULL;
}

void StateBoxPainter::SetState(int state)
{
	if ( state != nCurrentState )
	{
		StateSettings::iterator i = settings.find(state);
		if ( i != settings.end() )
		{
			if ( i->second.bBitmap && i->second.hbmBack != NULL )
			{
				BoxPainter::SetBitmap( i->second.hbmBack );
			}
			else
			{
				BoxPainter::SetColors( i->second.clrBack, i->second.clrBorder );
			}
		}
	}	
}

void StateBoxPainter::SetStateBitmap(int state, HBITMAP hbmBg)
{
	if (hbmBg != NULL)
	{
		StateSetting new_state;
		new_state.bBitmap = true;
		new_state.hbmBack = hbmBg;

		settings.insert( std::make_pair(state, new_state) );
	}	
}

void StateBoxPainter::SetStateColors(int state, COLORREF clrBg, COLORREF clrBrdr)
{
	StateSetting new_state;
	new_state.bBitmap = false;
	new_state.clrBack = clrBg;
	new_state.clrBorder = clrBrdr;

	settings.insert( std::make_pair(state, new_state) );	
}