#ifndef WINDOW_H
#define WINDOW_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fcntl.h>
#include "WindowAPI_Defs.h"

#if defined(CURRENT_OS_WINDOWS)
#include <io.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#pragma comment (lib, "opengl32.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

class F_WM;

class F_W
{
public:
	//window constructor
	F_W(const char*  a_WindowName, GLuint a_Width = 1280, GLuint a_Height = 720, GLuint a_ColourBits = 32,
		GLuint a_DepthBits = 8, GLuint a_StencilBits = 8, bool a_ShouldCreateTerminal = true);

	//window deconstruction
	~F_W();

	void Initialize();

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

	//gets and sets for the current state of the window
	GLuint GetCurrentState();
	void SetCurrentState(GLuint a_CurrentState);
	
	//get mouse state key by index
	bool GetKeyState(GLuint a_Key);

	//whether or not the window should be closing
	bool GetShouldClose();

	//make the window swap draw buffers
	void SwapDrawBuffers();

	//get and set for triggering full screen mode
	void FullScreen(bool a_NewState);
	bool GetIsFullScreen();

	//set and get for minimizing a window
	void Minimize(bool a_NewState);
	bool GetIsMinimized();

	// set and get for maximizing a window
	void Maximize(bool a_NewState);
	bool GetIsMaximized();

	//restore the window to its natural state
	void Restore();

	//creates on OpenGL Context
	void InitializeGL();

	//get and set for window name
	const char* GetWindowName();
	void SetName(const char* a_WindowName);

	//set the window icon
	void SetIcon();

	//make the window the current OpenGL context to be drawn
	void MakeCurrentContext();

	//whether the window is in focus
	bool GetInFocus();
	void Focus(bool a_NewState);

	//enable vertical sync if supported
	//a swap setting of -1 turns on adaptive V-sync
	void SetSwapInterval(GLint a_SwapSetting);

	void SetOnKeyEvent(OnKeyEvent a_OnKeyEvent);
	void SetOnMouseButtonEvent(OnMouseButtonEvent a_OnButtonMouseEvent);
	void SetOnMouseWheelEvent(OnMouseWheelEvent a_OnMouseWheelEvent);
	void SetOnDestroyed(OnDestroyed a_OnDestroyed);
	void SetOnMaximized(OnMaximized a_OnMaximized);
	void SetOnMinimized(OnMinimized a_OnMinimized);
	void SetOnRestored(OnRestored a_OnRestored);
	void SetOnFocus(OnFocus a_OnFocus);
	void SetOnMoved(OnMoved a_OnMoved);
	void SetOnResize(OnResize a_OnResize);
	void SetOnMouseMove(OnMouseMove a_OnMouseMove);

	friend class F_WM;

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
	bool m_OnConsole;

	bool m_InFocus;
	GLuint m_CurrentState;

	GLuint m_CurrentSwapInterval;

	void InitializeEvents();
	void InitGLExtensions();

	OnKeyEvent m_OnKeyEvent;
	OnMouseButtonEvent m_OnMouseButtonEvent;
	OnMouseWheelEvent m_OnMouseWheel;
	OnDestroyed m_OnDestroyed;
	OnMaximized m_OnMaximized;
	OnMinimized m_OnMinimized;
	OnRestored m_OnRestored;
	OnFocus m_OnFocus;
	OnMoved m_OnMoved;
	OnResize m_OnResize;
	OnMouseMove m_OnMouseMove;

	GLboolean EXTSwapControlSupported;
	GLboolean SGISwapControlSupported;
	GLboolean MESASwapControlSupported;
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
	void Windows_FullScreen(bool a_NewState);
	void Windows_Minimize(bool a_NewState);
	void Windows_Maximize(bool a_NewState);
	void Windows_Restore();
	void Windows_SetName(const char* a_NewName);
	void Windows_Focus(bool a_NewState);
	
	void Windows_InitializeGL();
	void Windows_Shutdown();
	void Windows_VerticalSync(GLint a_EnableSync);

	void HasConsole();
	HWND GetWindowHandle();
	void InitializePixelFormat();

	void Windows_InitGLExtensions();

	HDC m_DeviceContextHandle;
	HGLRC m_GLRenderingContextHandle;
	HPALETTE m_PaletteHandle;
	PIXELFORMATDESCRIPTOR m_PFD;

	WNDCLASS m_WindowClass;
	HWND m_WindowHandle;
	HINSTANCE m_InstanceHandle;

	LPARAM m_LongParam;
	WPARAM m_WordParam;

	PFNWGLSWAPINTERVALEXTPROC SwapIntervalEXT;
	PFNWGLGETEXTENSIONSSTRINGEXTPROC GetExtensionsStringEXT;
#endif

#if defined(CURRENT_OS_LINUX)
	void Linux_Initialize();
	void Linux_SetResolution(GLuint a_Width, GLuint a_Height);
	void Linux_SetPosition(GLuint a_X, GLuint a_Y);
	void Linux_SetMousePosition(GLuint a_X, GLuint a_Y);
	void Linux_FullScreen(bool a_NewState);
	void Linux_Minimize(bool a_NewState);
	void Linux_Maximize(bool a_NewState);
	void Linux_Restore();
	void Linux_Focus(bool a_NewState);
	void Linux_SetName(const char* a_NewName);
	
	void Linux_InitializeGL();
	void Linux_SetSwapInterval(GLint a_SwapInterval);
	void Linux_Shutdown();

	void InitializeAtomics();
	void Linux_InitGLExtensions();

	Window GetWindowHandle();
	Window m_Window;
	GLXContext m_Context;
	XVisualInfo* m_VisualInfo;
	GLint* m_Attributes;

	Colormap m_Colourmap;
	XSetWindowAttributes m_SetAttributes;
	XWindowAttributes m_WindowAttributes;
	
	//these are the callbacks for the glx swap interval extension. 
	PFNGLXSWAPINTERVALMESAPROC SwapIntervalMESA;
	PFNGLXSWAPINTERVALEXTPROC SwapIntervalEXT;
	PFNGLXSWAPINTERVALSGIPROC SwapIntervalSGI;	

	//these atomics are needed to change window states via the extended window manager
	Atom m_AtomicState; //_NET_WM_STATE
	Atom m_AtomicHidden;// _NET_WM_STATE_HIDDEN
	Atom m_AtomicFullScreenState; //NET_WM_STATE_FULLSCREEN
	Atom m_AtomicMaximizedHorizontal; // _NET_WM_STATE_MAXIMIZED_HORZ
	Atom m_AtomicMaximizedVertical; // _NET_WM_STATE_MAXIMIZED_VERT
	Atom m_AtomicCloseWindow; // _NET_WM_CLOSE_WINDOW
	Atom m_AtomActiveWindow; //_NET_ACTIVE_WINDOW
	Atom m_AtomDemandsAttention;//_NET_WM_STATE_DEMANDS_ATTENTION
	Atom m_AtomFocused;//_NET_WM_STATE_FOCUSED

	bool m_OverrideRedirect; //whether to use window manager or not	

#endif
};

#endif
