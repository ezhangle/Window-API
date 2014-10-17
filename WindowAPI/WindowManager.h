#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
	#include <Windows.h>
	#include <GL/GL.h>
#endif

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)
	#include <GL/gl.h>
	#include <X11/X.h>
	#include <X11/Xlib.h>
	#include <X11/keysym.h>	
#endif

#include "WindowAPI_Defs.h"
#include "Window.h"

class Foundation_Window;

class Foundation_WindowManager
{
	public:
		Foundation_WindowManager();
		~Foundation_WindowManager();

		static Foundation_Window* GetWindowByName(const char* a_WindowName);
		static Foundation_Window* GetWindowByIndex(GLuint a_WindowIndex);

		static Foundation_WindowManager* AddWindow(Foundation_Window* a_Window);
		static Foundation_WindowManager* GetInstance();
		static GLuint GetNumWindows();

		static void GetMousePositionInScreen(GLuint& a_X, GLuint& a_Y);
		static GLuint* GetMousePositionInScreen();
		static void SetMousePositionInScreen();

		static GLuint* GetScreenResolution();
		static void GetScreenResolution(GLuint& a_Width, GLuint& a_Height);

		friend Foundation_Window;

		static void PollForEvents();
	private:

		std::vector<Foundation_Window*> m_Windows;
		static Foundation_WindowManager* m_Instance;

		GLuint m_ScreenResolution[2];
		GLuint m_ScreenMousePosition[2];

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
		LRESULT CALLBACK WindowProcedure(HWND a_WindowHandle, GLuint a_Message, WPARAM a_WordParam, LPARAM a_LongParam);
	
		static LRESULT CALLBACK StaticWindowProcedure(HWND a_WindowHandle, UINT a_Message, WPARAM a_WordParam, LPARAM a_LongParam);

		static Foundation_Window* GetWindowByHandle(HWND a_WindowHandle);

		void Win32PollfForEvents();

		MSG m_Message;
		HDC m_DeviceContextHandle;
#endif

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)
		static Foundation_Window* GetWindowByHandle(Window a_WindowHandle);

		static void X11PollForEvents();

		Display* m_Display;
		XEvent m_Event;
#endif
};
#endif 
