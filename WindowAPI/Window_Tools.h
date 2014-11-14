#ifndef WINDOW_TOOLS_H
#define WINDOW_TOOLS_H
#include "WindowAPI_Defs.h"

class Foundation_Tools
{
public:

   static bool IsValid(const char* a_String)
	{
		return (a_String != nullptr);
	}

	static bool IsValid(OnKeyEvent a_OnKeyPressed)
	{
		return (a_OnKeyPressed != nullptr);
	}

	static bool IsValid(OnMouseWheelEvent a_OnMouseWheelEvent)
	{
		return (a_OnMouseWheelEvent != nullptr);
	}

	static bool IsValid(OnMaximized a_OnMaximized)
	{
		return (a_OnMaximized != nullptr);
	}

	static bool IsValid(OnMoved a_OnMoved)
	{
		return (a_OnMoved != nullptr);
	}

	static bool IsValid(OnMouseMove a_OnMouseMove)
	{
		return (a_OnMouseMove != nullptr);
	}
};


#endif
