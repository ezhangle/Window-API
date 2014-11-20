#ifndef TOOLS_H
#define TOOLS_H
#include "WindowAPI_Defs.h"

class WindowManager;

class Foundation_Tools
{
public:

   static GLboolean IsValid(const char* String)
	{
		return (String != nullptr);
	}

	static GLboolean IsValid(OnKeyEvent OnKeyPressed)
	{
		return (OnKeyPressed != nullptr);
	}

	static GLboolean IsValid(OnMouseWheelEvent OnMouseWheelEvent)
	{
		return (OnMouseWheelEvent != nullptr);
	}

	static GLboolean IsValid(OnMaximizedEvent OnMaximized)
	{
		return (OnMaximized != nullptr);
	}

	static GLboolean IsValid(OnFocusEvent OnFocus)
	{
		return (OnFocus != nullptr);
	}

	static GLboolean IsValid(OnMovedEvent OnMoved)
	{
		return (OnMoved != nullptr);
	}

	static GLboolean IsValid(OnMouseMoveEvent OnMouseMove)
	{
		return (OnMouseMove != nullptr);
	}

	friend WindowManager;

private:
	//these are internal. do not mess with these
#if defined(CURRENT_OS_WINDOWS)
	static GLuint Windows_TranslateKey(WPARAM WordParam, LPARAM LongParam);
#endif

#if defined(CURRENT_OS_LINUX)
	static GLuint Linux_TranslateKey(GLuint KeySym);

	static const char* Linux_GetEventType(XEvent Event);
#endif
};

#endif
