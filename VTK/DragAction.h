#ifndef __DRAGACTION_H
#define __DRAGACTION_H

#include "Action.h"
#include "GuiWindow.h"
#include "WinUtils.h"
#include "Hover.h"

class DragAction : public Action
{
public:
	static void ConnectGuiWindow(GuiWindow* wnd)
	{
		window = wnd;
	}

	void Execute(Message& message)
	{
		int x = message.lParamLo, y = message.lParamHi;

		if (window->IsDraggable() && window->IsDragPoint(x, y))
		{
			_LSLog(LOG_DEBUG, "drag", "dragging");
			ReleaseCapture();
			//SendMessage( window->GetModuleWindow(), WM_NCLBUTTONDOWN, HTCAPTION, 0);
			SendMessage( window->GetModuleWindow(), WM_SYSCOMMAND, SC_MOVE | 2, message.lParam );
		}
	}

protected:
	static GuiWindow* window;
};

GuiWindow* DragAction::window;

#endif