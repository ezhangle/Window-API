#ifndef WINDOW_H
#define WINDOW_H


#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fcntl.h>
#include "WindowAPI_Defs.h"

#if defined(CURRENT_OS_WINDOWS)
#include <glew.h>
#include <wglew.h>
#include <windows.h>
#include <io.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#pragma comment (lib, "opengl32.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#if defined(CURRENT_OS_LINUX)
#include <GL/glew.h>
#include <GL/glxew.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#endif

class F_WM;

class F_W
{
public:
	//window constructor
	F_W(const char*  a_WindowName, GLuint a_Width = 1280, GLuint a_Height = 720, GLuint a_ColourBits = 32,
		GLuint a_DepthBits = 8, GLuint a_StencilBits = 8, bool a_ShouldCreateTerminal = true);

	//window deconstructor
	~F_W();

	//shut down respective OpenGL context
	void Shutdown();

	//gets and set for window resolution
	void GetResolution(GLuint& a_Width, GLuint& a_Height);
	GLuint* GetResolution();
	void SetResolution(GLuint a_Width, GLuint a_Height);

	//gets and set for mouse position relative to window space
	void GetMousePosition(GLuint& a_X, GLuint& a_Y);
	GLuint* GetMousePosition();
	void SetMousePosition(GLuint a_X, GLuint a_Y);

	//gets and sets for window position
	void GetPosition(GLuint& a_X, GLuint& a_Y);
	GLuint* GetPosition();
	void SetPosition(GLuint a_X, GLuint a_Y);
	
	//get mouse state key by index
	bool GetKeyState(GLuint a_Key);

	//create a terminal output. linux not really required
	void CreateTerminal();

	//wether or not the window should be closing
	bool GetShouldClose();

	//make the window swap drawbuffers
	void SwapDrawBuffers();

	//get and set for triggering fullscreen mode
	void FullScreen(bool a_FullScreenState);
	bool GetIsFullScreen();

	//set and get for minimizing a window
	void Minimize(bool a_MinimizeState);
	bool GetIsMinimized();

	// set and get for maximizing a window
	void Maximise(bool a_MaximizeState);
	bool GetIsMaximised();

	//creates on OpenGL Context
	void InitializeGL();

	//get and set for window name
	const char* GetWindowName();
	void SetName(const char* a_WindowName);

	//make the window the current OpenGL context to be drawn
	void MakeCurrentContext();

	//whether the window is in focus
	bool GetInFocus();
	void Focus(bool a_FocuState);

	//whether the window is obscured
	bool GetIsObscured();

	void SetOnKeyEvent(OnKeyEvent a_OnKeyEvent);
	void SetOnMouseButtonEvent(OnMouseButtonEvent a_OnButtonMouseEvent);
	void SetOnMouseWheelEvent(OnMouseWheelEvent a_OnMouseWheelEvent);
	void SetOnCreated(OnCreated a_OnCreated);
	void SetOnDestroyed(OnDestroyed a_OnDestroyed);
	void SetOnMaximized(OnMaximized a_OnMaximized);
	void SetOnMinimized(OnMinimized a_OnMinimized);
	void SetOnMoved(OnMoved a_OnMoved);
	void SetOnResize(OnResize a_OnResize);
	void SetOnFocus(OnFocus a_OnFocus);
	void SetOnMouseMove(OnMouseMove a_OnMouseMove);

	friend F_WM;

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
	bool m_ShouldClose;
	GLuint m_WindowID;

	bool m_InFocus;
	bool m_IsObscured;
	bool m_IsFullScreen;
	bool m_IsMinimised;
	bool m_IsMaximised;

	void InitializeEvents();

	OnKeyEvent m_OnKeyEvent;
	OnMouseButtonEvent m_OnMouseButtonEvent;
	OnMouseWheelEvent m_OnMouseWheel;
	OnCreated m_OnCreated;
	OnDestroyed m_OnDestroyed;
	OnMaximized m_OnMaximized;
	OnMinimized m_OnMinimized;
	OnMoved m_OnMoved;
	OnResize m_OnResize;
	OnFocus m_OnFocus;
	OnMouseMove m_OnMouseMove;
	
#if defined(CURRENT_OS_WINDOWS)
	
private:

	void Windows_Initialize(LPCSTR a_MenuName, UINT a_Style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
		GLint a_ClearScreenExtra = 0, GLint a_WindowExtra = 0,
		HINSTANCE a_Instance = GetModuleHandle(0),
		HICON a_Icon = LoadIcon(0, IDI_APPLICATION),
		HCURSOR a_Cursor = LoadCursor(0, IDC_ARROW),
		HBRUSH a_Brush = (HBRUSH)BLACK_BRUSH);
	void Windows_SetResolution(GLuint a_Width, GLuint a_Height);
	void Windows_SetPosition(GLuint a_X, GLuint a_Y);
	void Windows_SetMousePosition(GLuint a_X, GLuint& a_Y);
	void Windows_FullScreen(bool a_FullScreenState);
	void Windows_Minimize(bool a_MinimizeState);
	void Windows_Maximize(bool a_MaximizeState);
	void Windows_SetName(const char* a_NewName);
	void Windows_Focus(bool a_FocusState);
	GLuint Windows_TranslateKey(WPARAM a_WordParam, LPARAM a_LongParam);
	void Windows_InitializeGL();
	void Windows_SetSwapInterval(bool a_SwapInterval);
	void Windows_Shutdown();

	HWND GetWindowHandle();
	void InitializePixelFormat();

	HDC m_DeviceContextHandle;
	HGLRC m_GLRenderingcontextHandle;
	HPALETTE m_PaletteHandle;
	PIXELFORMATDESCRIPTOR m_PFD;

	WNDCLASS m_WindowClass;
	HWND m_WindowHandle;

	LPARAM m_LongParam;
	WPARAM m_WordParam;
	
#endif

#if defined(CURRENT_OS_LINUX)

	void Linux_Initialize();
	void Linux_SetResolution(GLuint a_Width, GLuint a_Height);
	void Linux_SetPosition(GLuint a_X, GLuint a_Y);
	void Linux_SetMousePosition(GLuint a_X, GLuint a_Y);
	void Linux_FullScreen(bool a_FullScreenState);
	void Linux_Minimize(bool a_MinimizeState);
	void Linux_Maximize(bool a_MaximizeState);
	void Linux_SetName(const char* a_NewName);
	void Linux_Focus(bool a_FocusState);
	GLuint Linux_TranslateKey(GLuint a_KeySym);
	void Linux_InitializeGL();
	void Linux_SetSwapInterval(GLuint a_SwapInterval);
	void Linux_Shutdown();

	void InitializeAtomics();

	Window GetWindowHandle();
	Window m_Window;
	GLXContext m_Context;
	XVisualInfo* m_VisualInfo;
	GLint* m_Attributes;

	Colormap m_Colourmap;
	XSetWindowAttributes m_SetAttributes;
	XWindowAttributes m_WindowAttributes;

	//these atomics are needed to change window states via code
	Atom m_AtomicState; //_NET_WM_STATE
	Atom m_AtomicHidden;// _NET_WM_STATE_HIDDEN
	Atom m_AtomicFullScreenState; //NET_WM_STATE_FULLSCREEN
	Atom m_AtomicMaximizedHorizontal; // _NET_WM_STATE_MAXIMIZED_HORZ
	Atom m_AtomicMaximizedVertical; // _NET_WM_STATE_MAXIMIZED_VERT
	Atom m_AtomicCloseWindow; // _NET_CLOSE_WINDOW

	bool m_OverrideRedirect; //whether to use window manager or not	

#endif
};

#endif
