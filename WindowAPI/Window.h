#ifndef WINDOW_H
#define WINDOW_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fcntl.h>
#include "WindowAPI_Defs.h"

#if defined(CURRENT_OS_WINDOWS)
#include <io.h>

LRESULT CALLBACK FWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//this automatically loads the OpenGL library if you are using Visual studio 
#pragma comment (lib, "opengl32.lib")
//this makes sure that the entry point of your program is main(). not Winmain
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

class WindowManager; // just a forward declaration for the window manager

class FWindow
{
public:
	//window constructor
	FWindow(const char*  WindowName, GLuint Width = 1280, GLuint Height = 720, GLuint ColourBits = 32,
		GLuint DepthBits = 8, GLuint StencilBits = 8);

	//window deconstruction
	~FWindow();

	//Initializes the window depending on OS
	void Initialize();

	//shut down respective OpenGL context
	void Shutdown();

	//return the size/resolution of the window
	void GetResolution(GLuint& Width, GLuint& Height);
	//return the size/resolution of the window
	GLuint* GetResolution();
	//set the size/Resolution of the window
	void SetResolution(GLuint Width, GLuint Height);

	//return the position of the mouse cursor relative to the window co-ordinates
	void GetMousePosition(GLuint& X, GLuint& Y);
	//return the Position of the mouse cursor relative to the window co-ordinates
	GLuint* GetMousePosition();
	//set the position of the mouse cursor relative the the window co-ordinates
	void SetMousePosition(GLuint X, GLuint Y);

	//return the Position of the window relative to the screen co-ordinates
	void GetPosition(GLuint& X, GLuint& Y);
	//return the Position of the window relative to the screen co-ordinates
	GLuint* GetPosition();
	//Set the Position of the window relative to the screen co-ordinates
	void SetPosition(GLuint X, GLuint Y);

	//return the current state of the window
	GLuint GetCurrentState();
	//set the current state of the window
	void SetCurrentState(GLuint NewState);
	
	//get the state of a key(Down/Up) by index
	GLboolean GetKeyState(GLuint Key);

	//whether or not the window should be closing
	GLboolean GetShouldClose();

	//make the window swap draw buffers
	void SwapDrawBuffers();

	//toggle full screen mode depending on NewState. (true = Full screen, false = normal)
	void FullScreen(GLboolean NewState);
	//return if the window is in full screen mode
	GLboolean GetIsFullScreen();

	//toggle minimization depending on NewState. (true = minimized, false = normal)
	void Minimize(GLboolean NewState);
	//return if the window is Minimized
	GLboolean GetIsMinimized();

	// set and get for maximizing a window
	void Maximize(GLboolean NewState);
	GLboolean GetIsMaximized();

	//restore the window to its natural state
	void Restore();

	//creates on OpenGL Context
	void InitializeGL();

	//get and set for window name
	const char* GetWindowName();
	void SetName(const char* WindowName);

	//set the window icon
	void SetIcon();

	//make the window the current OpenGL context to be drawn
	void MakeCurrentContext();

	//whether the window is in focus
	GLboolean GetInFocus();
	void Focus(GLboolean NewState);

	//enable vertical sync if supported
	//a swap setting of -1 turns on adaptive V-sync on supported systems
	void SetSwapInterval(GLint SwapSetting);

	void SetOnKeyEvent(OnKeyEvent OnKey);
	void SetOnMouseButtonEvent(OnMouseButtonEvent OnMouseButton);
	void SetOnMouseWheelEvent(OnMouseWheelEvent OnMouseWheelEvent);
	void SetOnDestroyed(OnDestroyedEvent OnDestroyed);
	void SetOnMaximized(OnMaximizedEvent OnMaximized);
	void SetOnMinimized(OnMinimizedEvent OnMinimized);
	void SetOnRestored(OnRestoredEvent OnRestored);
	void SetOnFocus(OnFocusEvent OnFocus);
	void SetOnMoved(OnMovedEvent OnMoved);
	void SetOnResize(OnResizeEvent OnResize);
	void SetOnMouseMove(OnMouseMoveEvent OnMouseMove);

	friend class WindowManager; // lets window use private variables of WindowManager

private:
	//Name of the window
	const char*  Name;
	//ID of the FWindow. (where it belongs in the window manager)
	GLuint ID;
	//Colour format of the window. (defaults to 32 bit Colour)
	GLuint ColourBits;
	//Size of the Depth buffer. (defaults to 8 bit depth)
	GLuint DepthBits;
	//Size of the stencil buffer, (defaults to 8 bit)
	GLuint StencilBits;
	//Record of keys that are either pressed or released in the respective window
	GLboolean Keys[KEY_LAST];
	//Record of mouse buttons that are either presses or released
	GLboolean MouseButton[MOUSE_LAST];	
	//Resolution/Size of the window stored in an array
	GLuint Resolution[2];
	//Position of the FWindow relative to the screen co-ordinates
	GLuint Position[2];
	//Position of the Mouse cursor relative to the window co-ordinates
	GLuint MousePosition[2];
	//Whether the FWindow should be closing
	GLboolean ShouldClose;
	//Whether the FWindow is currently in focus(if it is the current window be used)
	GLboolean InFocus;
	//The current state of the window. these states include Normal, Minimized, Maximized and Full screen
	GLuint CurrentState;
	//The current swap interval of the window(V-Sync). a value of -1 enables adaptive V-Sync on supported systems
	GLuint CurrentSwapInterval;
	//set all the Events to null 
	void InitializeEvents();
	//Initializes OpenGL extensions
	void InitGLExtensions();

	//this is the callback to be used when a key has been pressed
	OnKeyEvent KeyEvent;
	//this is the callback to be used when a mouse button has been pressed
	OnMouseButtonEvent MouseButtonEvent;
	//this is the callback to be used when the mouse wheel has been scrolled. 
	OnMouseWheelEvent MouseWheelEvent;
	//this is the callback to be used when the window has been closed in a non-programmatic fashion
	OnDestroyedEvent DestroyedEvent;
	//this is the callback to be used when the window has been maximized in a non-programmatic fashion
	OnMaximizedEvent MaximizedEvent;
	//this is the callback to be used when the window has been minimized in a non-programmatic fashion
	OnMinimizedEvent MinimizedEvent;
	//this is the callback to be used when the window has been restored in a non-programmatic fashion
	OnRestoredEvent RestoredEvent;
	//this is the callback to be used when the window has been given focus in a non-programmatic fashion
	OnFocusEvent FocusEvent;
	//this is the callback to be used the window has been moved in a non-programmatic fashion
	OnMovedEvent MovedEvent;
	//this is a callback to be used when the window has been resized in a non-programmatic fashion
	OnResizeEvent ResizeEvent;
	//this is a callback to be used when the mouse has been moved
	OnMouseMoveEvent MouseMoveEvent;

	//Whether the EXT_Swap_Control(Generic) GL extension is supported on this machine
	GLboolean EXTSwapControlSupported;
	//Whether the SGI_Swap_Control(Silicon graphics) GL extension is supported on this machine
	GLboolean SGISwapControlSupported;
	//Whether the MESA_Swap_Control(Mesa) GL extension is supported on this machine
	GLboolean MESASwapControlSupported;

#if defined(CURRENT_OS_WINDOWS)
	
private:

	//tells windows to create a generic window. need to implement window styles sometime later
	void Windows_Initialize(LPCSTR a_MenuName, UINT a_Style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
		GLint a_ClearScreenExtra = 0, GLint FWindowExtra = 0,
		HINSTANCE a_Instance = GetModuleHandle(0),
		HICON a_Icon = LoadIcon(0, IDI_APPLICATION),
		HCURSOR a_Cursor = LoadCursor(0, IDC_ARROW),
		HBRUSH a_Brush = (HBRUSH)BLACK_BRUSH); 
	//uses the Win32 system to set the resolution/size of the window
	void Windows_SetResolution(GLuint Width, GLuint Height);
	//uses the win32 system to set the position of the window relative to the screen
	void Windows_SetPosition(GLuint X, GLuint Y);
	//uses the win32 system to set the position of the mouse cursor relative to the window
	void Windows_SetMousePosition(GLuint X, GLuint& Y);
	//uses the win32 system to have the window completely fill the screen and be drawn above the toolbar
	void Windows_FullScreen(GLboolean NewState);
	//uses the win32 system to minimize/hide the window. minimized windows don't receive events
	void Windows_Minimize(GLboolean NewState);
	//uses the win32 system to maximize the window. 
	void Windows_Maximize(GLboolean NewState);
	void Windows_Restore();
	void Windows_SetName(const char* NewName);
	void Windows_Focus(GLboolean NewState);
	
	void Windows_InitializeGL();
	void Windows_Shutdown();
	void Windows_VerticalSync(GLint EnableSync);

	HWND GetFWindowHandle();
	void InitializePixelFormat();

	void Windows_InitGLExtensions();

	HDC DeviceContextHandle;
	HGLRC GLRenderingContextHandle;
	HPALETTE PaletteHandle;
	PIXELFORMATDESCRIPTOR PixelFormatDescriptor;

	WNDCLASS FWindowClass;
	HWND FWindowHandle;
	HINSTANCE InstanceHandle;

	PFNWGLSWAPINTERVALEXTPROC SwapIntervalEXT;
	PFNWGLGETEXTENSIONSSTRINGEXTPROC GetExtensionsStringEXT;
#endif

#if defined(CURRENT_OS_LINUX)
	void Linux_Initialize();
	void Linux_SetResolution(GLuint Width, GLuint Height);
	void Linux_SetPosition(GLuint X, GLuint Y);
	void Linux_SetMousePosition(GLuint X, GLuint Y);
	void Linux_FullScreen(GLboolean NewState);
	void Linux_Minimize(GLboolean NewState);
	void Linux_Maximize(GLboolean NewState);
	void Linux_Restore();
	void Linux_Focus(GLboolean NewState);
	void Linux_SetName(const char* NewName);
	
	void Linux_InitializeGL();
	void Linux_VerticalSync(GLint EnableSync);
	void Linux_Shutdown();

	void InitializeAtomics();
	void Linux_InitGLExtensions();

	Window GetWindowHandle();
	Window WindowHandle;
	GLXContext Context;
	XVisualInfo* VisualInfo;
	GLint* Attributes;

	XSetWindowAttributes SetAttributes;
	
	//these are the callbacks for the GLX swap interval extension. 
	PFNGLXSWAPINTERVALMESAPROC SwapIntervalMESA;
	PFNGLXSWAPINTERVALEXTPROC SwapIntervalEXT;
	PFNGLXSWAPINTERVALSGIPROC SwapIntervalSGI;	

	//these atomics are needed to change window states via the extended window manager
	Atom AtomState; //_NET_WM_STATE
	Atom AtomHidden;// _NET_WM_STATE_HIDDEN
	Atom AtomFullScreen; //NET_WM_STATE_FULLSCREEN
	Atom AtomMaxHorz; // _NET_WM_STATE_MAXIMIZED_HORZ
	Atom AtomMaxVert; // _NET_WM_STATE_MAXIMIZED_VERT
	Atom AtomClose; // _NET_WM_CLOSE_WINDOW
	Atom AtomActive; //_NET_ACTIVE_WINDOW
	Atom AtomDemandsAttention;//_NET_WM_STATE_DEMANDS_ATTENTION
	Atom AtomFocused;//_NET_WM_STATE_FOCUSED

#endif
};

#endif
