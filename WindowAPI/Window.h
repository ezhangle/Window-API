#ifndef WINDOW_H
#define WINDOW_H

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <gl/GL.h>
#include <io.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#pragma comment (lib, "opengl32.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)
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
#include <string>

#include <fcntl.h>
#include "WindowAPI_Defs.h"

class Foundation_WindowManager;

class Foundation_Window
{
public:
	//window constructor
	Foundation_Window(const char*  a_WindowName, GLuint a_Width = 1280, GLuint a_Height = 720, GLuint a_ColourBits = 32,
		GLuint a_DepthBits = 8, GLuint a_StencilBits = 8, bool a_ShouldCreateTerminal = true);

	//window deconstructor
	~Foundation_Window();

	void Shutdown();

	//gets and set for window resolution(NOT IMPLEMENTED)
	void GetResolution(GLuint& a_Width, GLuint& a_Height);
	GLuint* GetResolution();
	void SetResolution(GLuint a_Width, GLuint a_Height);

	//gets and set for mouse position relative to window space(COMPLETED)
	void GetMousePositionInWindow(GLuint& a_X, GLuint& a_Y);
	GLuint* GetMousePositionInWindow();
	void SetMousePositionInWindow(GLuint a_X, GLuint a_Y);

	//gets and sets for window position(TESTING NEEDED)
	void GetPosition(GLuint& a_X, GLuint& a_Y);
	GLuint* GetPosition();
	void SetPosition(GLuint a_X, GLuint a_Y);
	
	//get mouse key by index
	bool GetKey(GLuint a_Key);

	//create a terminal output. linux not really required
	void CreateTerminal();

	//wether or not the window should be closing
	bool GetShouldClose();

	//make the window swap drawbuffers
	void SwapBuffers();

	//get and set for triggering fullscreen mode (SEMI FUNCTIONAL)
	void FullScreen(bool a_FullScreenState);
	bool GetIsFullScreen();

	//set and get for minimising a window(SEMI FUNCTIONAL)
	void Minimize(bool a_MinimizeState);
	bool GetIsMinimized();

	// set and get for maxmising a window(NOT  IMPLEMENTED)
	void Maximise(bool a_MaximizeState);
	bool GetIsMaximised();

	//creates on OpenGL Context
	void InitializeGL();

	//get and set for window Name(NOT IMPLEMENTED)
	const char* GetWindowName();
	void SetName(const char* a_WindowName);

	//make the window thr current OpenGL context to be drawn
	void MakeCurrentContext();

	//wether the window is in focus(WINDOWS NOT IMPLEMENTED)
	bool GetInFocus();

	//wether the window is obscured(NOT IMPLEMENTED)
	bool GetIsObscured();

	friend Foundation_WindowManager;

private:

	GLuint m_ColourBits;
	GLuint m_DepthBits;
	GLuint m_StencilBits;
	bool m_Keys[KEY_LAST];
	bool m_MouseEvents[MOUSE_LAST];
	const char*  m_WindowName;
	GLuint m_Resolution[2];
	GLuint m_Position[2];
	GLuint m_MousePosition[2];
	bool WindowShouldClose;
	GLuint m_WindowID;
	bool m_InFocus;
	bool m_IsObscured;

	bool m_IsFullScreen;
	bool m_IsMinimised;
	bool m_IsMaximised;

	void AddSelfToManager();

	void ShutDownWindow();	
	void InitializePixelFormat();
	
#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)

public:
	HWND GetWindowHandle();
private:

	//HDC m_DeviceContextHandle;
	HGLRC m_GLRenderingcontextHandle;
	HPALETTE m_PaletteHandle;
	PIXELFORMATDESCRIPTOR m_PFD;

	WNDCLASS m_WindowClass;
	//MSG m_Message;
	HWND m_WindowHandle;

	LPARAM m_LongParam;
	WPARAM m_WordParam;

	void Win32TranslateKey(WPARAM a_WordParam, LPARAM a_LongParam, bool a_KeyState);

	void InitializeWin32(LPCSTR a_MenuName,
		UINT a_Style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
		int a_ClearScreenExtra = 0, int a_WindowExtra = 0,
		HINSTANCE a_Instance = GetModuleHandle(0),
		HICON a_Icon = LoadIcon(0, IDI_APPLICATION),
		HCURSOR a_Cursor = LoadCursor(0, IDC_ARROW),
		HBRUSH a_Brush = (HBRUSH)BLACK_BRUSH);
#endif

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)

	void Linux_SetResolution();

	void Linux_SetPosition(GLuint a_X, GLuint a_Y);

	void Linux_SetMousePositionInWindow(GLuint a_X, GLuint a_Y);
	
	void Linux_FullScreen(bool a_FullScreenState);

	void Linux_Minimize(bool a_MinimizeState);

	void Linux_Maximize(bool a_MaximizeState);

	void Linux_SetWindowName();

	void Linux_FocusWindow(bool a_FocusState);

	void Linux_TranslateKey(GLuint a_KeySym, bool a_KeyState);

	void InitializeAtomics();

	void Linux_InitializeGL();

	Window GetWindowHandle();
	Window m_Window;
	GLXContext m_Context;
	XVisualInfo* m_VisualInfo;
	GLint* m_Attributes;

	Colormap m_Colourmap;
	XSetWindowAttributes m_SetAttributes;
	XWindowAttributes m_WindowAttributes;

	Atom m_AState; //_NET_WM_STATE
	Atom m_AAddState; //_NET_WM_STATE_ADD
	Atom m_ARemoveState; // _NET_WM_STATE_REMOVE
	Atom m_AFullScreenState; //NET_WM_STATE_FULLSCREEN
	Atom m_AMaximizedHorizontal; // _NET_WM_STATE_MAXIMIZED_HORZ
	Atom m_AMaximizedVertical; // _NET_WM_STATE_MAXIMIZED_VERT
	Atom m_AWindowMotifs; // _MOTIF_WM_HINTS
	Atom m_ABypassCompositor; // _NET_WM_BYPASS_COMPOSITOR
	Atom m_AActiveWindow; // _NET_ACTIVE_WINDOW
	Atom m_ACloseWindow; // _NET_CLOSE_WINDOW

	bool m_OverrideRedirect; //wether to use window manager or not	

#endif
};

#endif
