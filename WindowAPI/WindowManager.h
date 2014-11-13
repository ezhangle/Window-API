#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#if defined(CURRENT_OS_WINDOWS)
	#include <Windows.h>
#endif

#if defined(CURRENT_OS_LINUX)
	#include <X11/X.h>
	#include <X11/Xlib.h>
	#include <X11/keysym.h>
	#include <string>
#endif

#include "WindowAPI_Defs.h"
#include "Window.h"

class F_W;

class F_WM
{
	friend F_W;

	public:
		F_WM();
		~F_WM();

		static void ShutDown();

		static F_W* GetWindowByName(const char* a_WindowName);
		static F_W* GetWindowByIndex(GLuint a_WindowIndex);

		static F_WM* AddWindow(F_W* a_Window);
		static F_WM* GetInstance();
		static GLuint GetNumWindows();

		static void GetMousePositionInScreen(GLuint& a_X, GLuint& a_Y);
		static GLuint* GetMousePositionInScreen();
		static void SetMousePositionInScreen(GLuint a_X, GLuint a_Y);

		static GLuint* GetScreenResolution();
		static void GetScreenResolution(GLuint& a_Width, GLuint& a_Height);

		//these are another way to set and get window variables
		
		//sets and gets for window resolution
		//static void GetWindowResolution(F_W* a_Window, GLuint& a_Width, GLuint& a_Height);
		static void GetWindowResolution(const char* a_WindowName, GLuint& a_Width, GLuint& a_Height);
		static void GetWindowResolution(GLuint a_WindowIndex, GLuint& a_Width, GLuint& a_Height);

		//static GLuint* GetWindowResolution(F_W* a_Window);
		static GLuint* GetWindowResolution(const char* a_WindowName);
		static GLuint* GetWindowResolution(GLuint a_WindowIndex);

		//static void SetWindowResolution(F_W* a_Window, GLuint a_Width, GLuint a_Height);
		static void SetWindowResolution(const char* a_WindowName, GLuint a_Width, GLuint a_Height);
		static void SetWindowResolution(GLuint a_WindowIndex, GLuint a_Width, GLuint a_Height);

		//sets and gets for window position
		//static void GetWindowPosition(F_W* a_Window, GLuint& a_X, GLuint& a_Y);
		static void GetWindowPosition(const char* a_WindowName, GLuint& a_X, GLuint& a_Y);
		static void GetWindowPosition(GLuint a_WindowIndex, GLuint& a_X, GLuint& a_Y);

		//static GLuint* GetWindowPosition(F_W* a_Window);
		static GLuint* GetWindowPosition(const char* a_WindowName);
		static GLuint* GetWindowPosition(GLuint a_WindowIndex);

		//static void SetWindowPosition(F_W* a_Window, GLuint a_X, GLuint a_Y);
		static void SetWindowPosition(const char* a_WindowName, GLuint a_X, GLuint a_Y);
		static void SetWindowPosition(GLuint a_WindowIndex, GLuint a_X, GLuint a_Y);

		//sets and gets for the mouse position in window
		//static void GetMousePositionInWindow(F_W* a_Window, GLuint& a_X, GLuint& a_Y);
		static void GetMousePositionInWindow(const char* a_WindowName, GLuint& a_X, GLuint& a_Y);
		static void GetMousePositionInWindow(GLuint a_WindowIndex, GLuint& a_X, GLuint& a_Y);

		//static GLuint* GetMousePositionInWindow(F_W* a_Window);
		static GLuint* GetMousePositionInWindow(const char* a_WindowName);
		static GLuint* GetMousePositionInWindow(GLuint a_WindowIndex);

		//static void SetMousePositionInWindow(F_W* a_Window, GLuint a_X, GLuint a_Y);
		static void SetMousePositionInWindow(const char* a_WindowName, GLuint a_X, GLuint a_Y);
		static void SetMousePositionInWindow(GLuint a_WindowIndex, GLuint a_X, GLuint a_Y);

		//gets for window keys
		//static bool WindowGetKey(F_W* a_Window, GLuint a_Key);
		static bool WindowGetKey(const char* a_WindowName, GLuint a_Key);
		static bool WindowGetKey(GLuint a_WindowIndex, GLuint a_Key);

		//gets for window should close
		//static bool GetWindowShouldClose(F_W* a_Window);
		static bool GetWindowShouldClose(const char* a_WindowName);
		static bool GetWindowShouldClose(GLuint a_WindowIndex);

		//swap buffers
		//static void WindowSwapBuffers(F_W* a_Window);
		static void WindowSwapBuffers(const char* a_WindowName);
		static void WindowSwapBuffers(GLuint a_WindowIndex);

		//sets and gets for fullscreen
		//static void SetFullScreen(F_W* a_Window, bool a_FullScreenState);
		static void SetFullScreen(const char* a_WindowName, bool a_FullScreenState);
		static void SetFullScreen(GLuint a_WindowIndex, bool a_FullScreenState);

		//static bool GetWindowIsFullScreen(F_W* a_Window);
		static bool GetWindowIsFullScreen(const char* a_WindowName);
		static bool GetWindowIsFullScreen(GLuint a_WindowIndex);

		//gets and sets for minimized
		//static bool GetWindowIsMinimized(F_W* a_Window);
		static bool GetWindowIsMinimized(const char* a_WindowName);
		static bool GetWindowIsMinimized(GLuint a_WindowIndex);

		//static void MinimizeWindow(F_W* a_Window, bool a_MinimizeState);
		static void MinimizeWindow(const char* a_WindowName, bool a_MinimizeState);
		static void MinimizeWindow(GLuint a_WindowIndex, bool a_MinimizeState);

		//gets and sets for maximised
		//static bool GetWindowIsMaximized(F_W* a_Window);
		static bool GetWindowIsMaximized(const char* a_WindowName);
		static bool GetWindowIsMaximized(GLuint a_WindowIndex);

		//static void MaximizeWindow(F_W* a_Window, bool a_MaximizeState);
		static void MaximizeWindow(const char* a_WindowName, bool a_MaximizeState);
		static void MaximizeWindow(GLuint a_WindowIndex, bool a_MaximizeState);

		//gets and sets for window name and index
		//static const char* GetWindowName(F_W* a_Window);
		static const char* GetWindowName(GLuint a_WindowIndex);
		static GLuint GetWindowIndex(const char*  a_WindowName);

		//static void SetWindowName(F_W* a_Window, const char* a_NewName);
		static void SetWindowName(const char* a_WindowName, const char* a_NewName);
		static void SetWindowName(GLuint a_WindowIndex, const char* a_NewName);

		//gets and sets window is in focus
		//static bool GetWindowIsInFocus(F_W* a_Window);
		static bool GetWindowIsInFocus(const char* a_WindowName);
		static bool GetWindowIsInFocus(GLuint a_WindowIndex);

		//static void FocusWindow(F_W* a_Window, bool a_FocusState);
		static void FocusWindow(const char* a_WindowName, bool a_FocusState);
		static void FocusWindow(GLuint a_WindowIndex, bool a_FocusState);

		//get window obscurity
		//static bool GetWindowIsObscured(F_W* a_Window);
		static bool GetWindowIsObscured(const char* a_WindowName);
		static bool GetWindowIsObscured(GLuint a_WindowIndex);
		
		static void Initialize();

		

		static void PollForEvents();

		static void RemoveWindow(F_W* a_Window);
	private:

		std::vector<F_W*> m_Windows;
		static F_WM* m_Instance;

		GLuint m_ScreenResolution[2];
		GLuint m_ScreenMousePosition[2];

	//	GLEWContext m_GLEWContext;

#if defined(CURRENT_OS_WINDOWS)
		LRESULT CALLBACK WindowProcedure(HWND a_WindowHandle, GLuint a_Message, WPARAM a_WordParam, LPARAM a_LongParam);
	
		static LRESULT CALLBACK StaticWindowProcedure(HWND a_WindowHandle, UINT a_Message, WPARAM a_WordParam, LPARAM a_LongParam);

		static F_W* GetWindowByHandle(HWND a_WindowHandle);

		static void Windows_PollForEvents();
		static void Windows_Initialize();
		static void Windows_Shutdown();
		static void Windows_SetMousePositionInScreen(GLuint a_X, GLuint a_Y);

		MSG m_Message;
		HDC m_DeviceContextHandle;
#endif

#if defined(CURRENT_OS_LINUX)
		static F_W* GetWindowByHandle(Window a_WindowHandle);
		static F_W* GetWindowByEvent(XEvent a_Event);

		static void Linux_Initialize();
		static void Linux_Shutdown();

		static void Linux_PollForEvents();
		static void Linux_SetMousePositionInScreen(GLuint a_X, GLuint a_Y);
		static Display* GetDisplay();

		Display* m_Display;
		XEvent m_Event;
#endif
};
#endif 
