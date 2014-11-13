#ifndef WINDOW_TOOLS_H
#define WINDOW_TOOLS_H
#include "WindowAPI_Defs.h"

class Foundation_Tools
{
public:

   static bool IsValid(const char* a_String)
	{
		return (a_String != 0 && a_String != nullptr);
	}

	static bool IsValid(OnKeyEvent a_OnKeyPressed)
	{
		return (a_OnKeyPressed != 0 && a_OnKeyPressed != nullptr);
	}

	static bool IsValid(OnMouseWheelEvent a_OnMouseWheelEvent)
	{
		return (a_OnMouseWheelEvent != 0 && a_OnMouseWheelEvent != nullptr);
	}

	static bool IsValid(OnCreated a_OnCreated)
	{
		return (a_OnCreated != 0 && a_OnCreated != nullptr);
	}

	static bool IsValid(OnMaximized a_OnMaximized)
	{
		return (a_OnMaximized != 0 && a_OnMaximized != nullptr);
	}

	/*static bool IsValid(OnFullscreen a_OnFullScreen)
	{
		return (a_OnFullScreen != 0 && a_OnFullScreen != nullptr);
	}*/

	/*static bool IsValid(OnNameChange a_OnNameChanged)
	{
		return (a_OnNameChanged != 0 && a_OnNameChanged != nullptr);
	}*/

	static bool IsValid(OnMoved a_OnMoved)
	{
		return (a_OnMoved != 0 && a_OnMoved != nullptr);
	}
};


#endif
