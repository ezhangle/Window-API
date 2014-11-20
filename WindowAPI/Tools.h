#ifndef TOOLS_H
#define TOOLS_H
#include "WindowAPI_Defs.h"

class F_WM;

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

	static bool IsValid(OnFocus a_OnFocus)
	{
		return (a_OnFocus != nullptr);
	}

	static bool IsValid(OnMoved a_OnMoved)
	{
		return (a_OnMoved != nullptr);
	}

	static bool IsValid(OnMouseMove a_OnMouseMove)
	{
		return (a_OnMouseMove != nullptr);
	}

	friend F_WM;

private:
	//these are internal. do not mess with these
#if defined(CURRENT_OS_WINDOWS)
	static GLuint Windows_TranslateKey(WPARAM a_WordParam, LPARAM a_LongParam);
#endif

#if defined(CURRENT_OS_LINUX)
	static GLuint Linux_TranslateKey(GLuint a_KeySym);

	static const char* Linux_GetEventType(XEvent a_Event);
#endif
};

#endif
