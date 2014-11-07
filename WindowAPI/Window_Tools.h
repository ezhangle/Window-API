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

static bool IsValid(OnKeyPressed a_OnKeyPressed)
{
	return (a_OnKeyPressed != 0 && a_OnKeyPressed != nullptr);
}

/*bool IsValid(OnKeyReleased a_OnKeyReleased)
{
	return (a_OnKeyReleased != 0 && a_OnKeyReleased != nullptr);
}*/

/*bool IsValid(OnButtonPressed a_OnButtonPressed)
{
	return (a_OnButtonPressed != 0 && a_OnButtonPressed != nullptr);
}*/

/*bool IsValid(OnButtonReleased a_OnButtonReleased)
{
	return (a_OnButtonReleased != 0 && a_OnButtonReleased != nullptr);
}*/

/*bool IsValid(OnMouseWheelEvent a_OnMouseWheelEvent)
{
	return (a_OnMouseWheelEvent != 0 && a_OnMouseWheelEvent != nullptr);
}*/

static bool IsValid(OnCreated a_OnCreated)
{
	return (a_OnCreated != 0 && a_OnCreated != nullptr);
}

/*bool IsValid(OnDestroyed a_OnDestroyed)
{
	return (a_OnDestroyed != 0 && a_OnDestroyed != nullptr);
}*/

static bool IsValid(OnFullscreen a_OnFullScreen)
{
	return (a_OnFullScreen != 0 && a_OnFullScreen != nullptr);
}

/*bool IsValid(OnMaximized a_OnMaximized)
{
	return (a_OnMaximized != 0 && a_OnMaximized != nullptr);
}*/

/*bool IsValid(OnMinimized a_OnMinimized)
{
	return (a_OnMinimized != 0  && a_OnMinimized != nullptr);
}*/

static bool IsValid(OnNameChange a_OnNameChanged)
{
	return (a_OnNameChanged != 0 && a_OnNameChanged != nullptr);
}

static bool IsValid(OnMoved a_OnMoved)
{
	return (a_OnMoved != 0 && a_OnMoved != nullptr);
}

/*bool IsValid(OnFocus a_OnFocus)
{
	return (a_OnFocus != 0 && a_OnFocus != nullptr);
}*/

/*bool IsValid(OnMouseMove a_OnMouseMove)
{
	return (a_OnMouseMove != 0 && a_OnMouseMove != nullptr);
}*/
};


#endif
