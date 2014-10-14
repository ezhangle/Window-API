#ifndef WINDOW_H
#define WINDOW_H

#ifdef _MSC_VER
#include <windows.h>
#include <gl/GL.h>
#include <io.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#pragma comment (lib, "opengl32.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#ifdef __linux__ || __GNUG__ || __GNUC__ || __clang__
#include <GL/glx.h>
#include <GL/glu.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <GL/gl.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include "Keys.h"

class Foundation_WindowManager;

class Foundation_Window
{
public:

	Foundation_Window(const char* a_WindowName, GLuint a_Width = 1280, GLuint a_Height = 720, GLuint a_ColourBits = 32,
		GLuint a_DepthBits = 8, GLuint a_StencilBits = 8, bool a_ShouldCreateTerminal = true);

	~Foundation_Window(){}

	void InitializeGL();

	void Redraw();

	void Resize(GLuint a_Width, GLuint a_Height);

	void GetResolution(GLuint& a_Width, GLuint& a_Height);

	void SetResolution(GLuint a_Width, GLuint a_Height);
	
	bool GetKey(GLuint a_Key);
	void PollForEvents();
	void CreateTerminal();
	bool GetWindowShouldClose();	
	void Window_SwapBuffers();
	void SetFullScreen(bool a_FullScreenState);
	void Initialize();
	const char* GetWindowName();

	friend Foundation_WindowManager;

private:

	GLuint m_ColourBits;
	GLuint m_DepthBits;
	GLuint m_StencilBits;
	bool m_Keys[KEY_LAST];
	bool m_MouseEvents[MOUSE_LAST];
	const char* m_WindowName;
	GLuint m_Resolution[2];
	GLuint m_WindowPosition[2];
	bool WindowShouldClose;
	GLuint m_WindowID;

	void ShutDownWindow();	
	void InitializePixelFormat();
	
#ifdef _MSC_VER

public:
	HWND GetWindowHandle();
private:

	HDC m_DeviceContextHandle;
	HGLRC m_GLRenderingcontextHandle;
	HPALETTE m_PaletteHandle;
	PIXELFORMATDESCRIPTOR m_PFD;

	WNDCLASS m_WindowClass;
	MSG m_Message;
	HWND m_WindowHandle;

	LPARAM m_LongParam;
	WPARAM m_WordParam;

	void Win32TranslateKey(WPARAM a_WordParam, LPARAM a_LongParam, bool a_KeyState);

	void InitializeWin32(LPCSTR a_MenuName,
		UINT a_Style = CS_OWNDC | CS_HREDRAW | CS_DROPSHADOW,
		int a_ClearScreenExtra = 0, int a_WindowExtra = 0,
		HINSTANCE a_Instance = GetModuleHandle(0),
		HICON a_Icon = LoadIcon(0, IDI_APPLICATION),
		HCURSOR a_Cursor = LoadCursor(0, IDC_ARROW),
		HBRUSH a_Brush = (HBRUSH)BLACK_BRUSH);
#endif

#ifdef __linux__ || __GNUG__ || __GNUC__ || __clang__

	/*ok now this is for fullscreen functionality. we need to save certain
	 * window attributes in order to be able to easily toggle between
	 * fullscreen states*/ 
	struct SavedScreenState
	{
		int m_Count;
		int m_TimeOut;
		int m_Interval;
		int m_Blanking;
		int m_Exposure;		
	};

	struct WindowHints
	{
		GLuint m_Flags;
		GLuint m_Functions;
		GLuint m_Decorations;
		GLint m_InputMode;
		GLuint m_Status;
	};


	void XTranslateKey(GLuint a_KeySym, bool a_KeyState);
	Display* m_Display;
	Window m_Window;
	XEvent m_Event;
	GLXContext m_Context;
	XVisualInfo* m_VisualInfo;
	GLint* m_Attributes;

	Colormap m_Colourmap;
	XSetWindowAttributes m_SetAttributes;
	XWindowAttributes m_WindowAttributes;
	
	SavedScreenState m_SavedScreenState;

	WindowHints m_WindowHints;

	Atom WM_STATE;
	Atom WM_DELETE_WINDOW;
	Atom NET_WM_STATE;
	Atom NET_WM_STATE_FULLSCREEN;
	Atom NET_WM_BYPASS_COMPOSITOR;
	Atom NET_ACTIVE_WINDOW;
	Atom _MOTIF_WM_HINTS;
	Atom _NET_WM_STATE_ADD;
	bool m_OverrideRedirect;

	Atom m_WindowProperty;

#endif

};

#endif
