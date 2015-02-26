#ifndef WINDOW_H
#define WINDOW_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fcntl.h>
#include "WindowAPI_Defs.h"

#if defined(_WIN32) || defined(_WIN64)
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
	FWindow(const char*  WindowName, GLuint Width = 1280, GLuint Height = 720, GLuint ColourBits = 8,
		GLuint DepthBits = 24, GLuint StencilBits = 8);

	//window deconstruction
	~FWindow();

	//Initializes the window depending on OS
	GLboolean Initialize();

	//shut down respective OpenGL context
	GLboolean Shutdown();

	//return the size/resolution of the window
	GLboolean GetResolution(GLuint& Width, GLuint& Height);
	//return the size/resolution of the window
	GLuint* GetResolution();
	//set the size/Resolution of the window
	GLboolean SetResolution(GLuint Width, GLuint Height);

	//return the position of the mouse cursor relative to the window co-ordinates
	GLboolean GetMousePosition(GLuint& X, GLuint& Y);
	//return the Position of the mouse cursor relative to the window co-ordinates
	GLuint* GetMousePosition();
	//set the position of the mouse cursor relative the the window co-ordinates
	GLboolean SetMousePosition(GLuint X, GLuint Y);

	//return the Position of the window relative to the screen co-ordinates
	GLboolean GetPosition(GLuint& X, GLuint& Y);
	//return the Position of the window relative to the screen co-ordinates
	GLuint* GetPosition();
	//Set the Position of the window relative to the screen co-ordinates
	GLboolean SetPosition(GLuint X, GLuint Y);

	//return the current state of the window
	GLuint GetCurrentState();
	//set the current state of the window
	GLboolean SetCurrentState(GLuint NewState);
	
	//get the state of a key(Down/Up) by index
	GLboolean GetKeyState(GLuint Key);

	//whether or not the window should be closing
	GLboolean GetShouldClose();

	//make the window swap draw buffers
	GLboolean SwapDrawBuffers();

	//toggle full screen mode depending on NewState. (true = Full screen, false = normal)
	GLboolean FullScreen(GLboolean NewState);
	//return if the window is in full screen mode
	GLboolean GetIsFullScreen();

	//toggle minimization depending on NewState. (true = minimized, false = normal)
	GLboolean Minimize(GLboolean NewState);
	//return if the window is Minimized
	GLboolean GetIsMinimized();

	// set and get for maximizing a window
	GLboolean Maximize(GLboolean NewState);
	GLboolean GetIsMaximized();

	//restore the window to its natural state
	GLboolean Restore();

	//creates on OpenGL Context
	GLboolean InitializeGL();

	//get and set for window name
	const char* GetWindowName();
	GLboolean SetTitleBar(const char* NewText);

	//set the style for the window
	GLboolean SetStyle(GLuint WindowType);

	//set the window icon
	GLboolean SetIcon(const char* Icon, GLuint Width, GLuint Height);

	//make the window the current OpenGL context to be drawn
	GLboolean MakeCurrentContext();

	//returns Whether the current window is the current OpenGL context to be drawn
	GLboolean GetIsCurrentContext();

	//returns whether the OpenGL context for this window has been created
	GLboolean GetContextHasBeenCreated();

	//whether the window is in focus
	GLboolean GetInFocus();
	GLboolean Focus(GLboolean NewState);

	//enable vertical sync if supported
	//a swap setting of -1 turns on adaptive V-sync on supported systems
	GLboolean SetSwapInterval(GLint SwapSetting);

	//set the on key event callback for this window
	GLboolean SetOnKeyEvent(OnKeyEvent OnKey);
	//set the on mouse button event callback for this window
	GLboolean SetOnMouseButtonEvent(OnMouseButtonEvent OnMouseButton);
	//set the on mouse wheel event callback for this window
	GLboolean SetOnMouseWheelEvent(OnMouseWheelEvent OnMouseWheel);
	//set the window on destroyed event callback for this window
	GLboolean SetOnDestroyed(OnDestroyedEvent OnDestroyed);
	//set the window on maximizes event callback for this window
	GLboolean SetOnMaximized(OnMaximizedEvent OnMaximized);
	//set the window on minimized event callback for this window
	GLboolean SetOnMinimized(OnMinimizedEvent OnMinimized);
	//set the window on restored event callback for this window
	//void SetOnRestored(OnRestoredEvent OnRestored);
	//set the window on focus event callback for this window
	GLboolean SetOnFocus(OnFocusEvent OnFocus);
	//set the window on moved event callback for this window
	GLboolean SetOnMoved(OnMovedEvent OnMoved);
	//set the window on resize event callback for this window
	GLboolean SetOnResize(OnResizeEvent OnResize);
	//set the window on Mouse move callback event for this window
	GLboolean SetOnMouseMove(OnMouseMoveEvent OnMouseMove);

	//print the current OpenGL version
	GLboolean PrintOpenGLVersion();
	//return the current OpenGL version as a string
	const char* GetOpenGLVersion();
	//print all supported extensions
	GLboolean PrintOpenGLExtensions();
	//return all the supported extensions
	const char* GetOpenGLExtensions();

	//enable window decorator
	GLboolean EnableDecorator(GLbitfield Decorator);
	//disable window decorator
	GLboolean DisableDecorator(GLbitfield Decorator);

	friend class WindowManager; // lets window use private variables of WindowManager

private:

	const char*  Name; /**< Name of the window. also initially the title bar text*/
	GLuint ID; /**< ID of the FWindow. (where it belongs in the window manager) */
	GLint ColourBits; /**< Colour format of the window. (defaults to 32 bit Colour)*/
	GLint DepthBits; /**< Size of the Depth buffer. (defaults to 8 bit depth) */
	GLint StencilBits; /**< Size of the stencil buffer, (defaults to 8 bit) */
	GLboolean Keys[KEY_LAST]; /**< Record of keys that are either pressed or released in the respective window */
	GLboolean MouseButton[MOUSE_LAST];	 /**< Record of mouse buttons that are either presses or released*/
	GLuint Resolution[2]; /**< Resolution/Size of the window stored in an array*/
	GLuint Position[2]; /**< Position of the FWindow relative to the screen co-ordinates*/
	GLuint MousePosition[2]; /**< Position of the Mouse cursor relative to the window co-ordinates*/
	GLboolean ShouldClose; /**< Whether the FWindow should be closing*/
	GLboolean InFocus; /**< Whether the FWindow is currently in focus(if it is the current window be used)*/
	GLboolean Initialized; /**< Whether the FWindoa has been fully Initialized*/
	GLboolean ContextCreated; /**< Whether the OpenGL context for this window has been created*/
	GLboolean IsCurrentContext; /**< Whether the window is the current window that is being drawn to */
	GLuint CurrentState; /**< The current state of the window. these states include Normal, Minimized, Maximized and Full screen*/
	GLuint CurrentSwapInterval; /**< The current swap interval of the window(V-Sync). a value of -1 enables adaptive V-Sync on supported systems */
	GLbitfield CurrentWindowStyle; /**< the current window style */

	//set all the Events to null 
	void InitializeEvents();
	//Initializes OpenGL extensions
	void InitGLExtensions();

	OnKeyEvent KeyEvent; /**< this is the callback to be used when a key has been pressed*/
	OnMouseButtonEvent MouseButtonEvent; /**< this is the callback to be used when a mouse button has been pressed*/
	OnMouseWheelEvent MouseWheelEvent; /**< this is the callback to be used when the mouse wheel has been scrolled.*/
	OnDestroyedEvent DestroyedEvent; /**< this is the callback to be used when the window has been closed in a non-programmatic fashion*/
	OnMaximizedEvent MaximizedEvent; /**< this is the callback to be used when the window has been maximized in a non-programmatic fashion*/
	OnMinimizedEvent MinimizedEvent; /**<this is the callback to be used when the window has been minimized in a non-programmatic fashion */
	//OnRestoredEvent RestoredEvent; /**< this is the callback to be used when the window has been restored in a non-programmatic fashion*/
	OnFocusEvent FocusEvent;  /**< this is the callback to be used when the window has been given focus in a non-programmatic fashion */
	OnMovedEvent MovedEvent;  /**< this is the callback to be used the window has been moved in a non-programmatic fashion*/
	OnResizeEvent ResizeEvent; /**< this is a callback to be used when the window has been resized in a non-programmatic fashion*/
	OnMouseMoveEvent MouseMoveEvent; /**< this is a callback to be used when the mouse has been moved*/

	GLboolean EXTSwapControlSupported; /**< Whether the EXT_Swap_Control(Generic) GL extension is supported on this machine */
	GLboolean SGISwapControlSupported; /**< Whether the SGI_Swap_Control(Silicon graphics) GL extension is supported on this machine */
	GLboolean MESASwapControlSupported; /**< Whether the MESA_Swap_Control(Mesa) GL extension is supported on this machine*/

	//this section is for the windows side of the Window API
#if defined(_WIN32) || defined(_WIN64)
	
private:

	//tells windows to create a generic window. need to implement window styles sometime later
	GLboolean Windows_Initialize(UINT a_Style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
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
	/*uses the win32 system to have the window completely fill the screen and be 
	drawn above the toolbar. changing the screen resolution to match has been disabled 
	due to event handling issues*/
	void Windows_FullScreen();
	//uses the win32 system to minimize/hide the window. minimized windows don't receive events
	void Windows_Minimize();
	//uses the win32 system to maximize the window. 
	void Windows_Maximize();
	//uses the win32 system to restore the window
	void Windows_Restore();
	//uses the win32 system to set the Title Bar text
	void Windows_SetTitleBar(const char* NewTitle);
	//uses the Win32 system to set the window icon
	void Windows_SetIcon(const char* Icon, GLuint Width, GLuint Height);
	//uses the win32 system to put the window into event focus
	void Windows_Focus();
	//initialize OpenGL for this window
	GLboolean Windows_InitializeGL();
	//cleanly shutdown this window(window would still need to be deleted of course)
	void Windows_Shutdown();
	//turns of vertical sync using the EXT extension
	void Windows_VerticalSync(GLint EnableSync);
	//enables given window decoration via Win32
	void Windows_EnableDecorator(GLbitfield Decorator);
	//disables given window decoration via Win32
	void Windows_DisableDecorator(GLbitfield Decorator);

	void Windows_SetStyle(GLuint WindowType);
	//get the handle of the window. to be used internally only
	HWND GetWindowHandle();

	//initialize the pixel format 
	void InitializePixelFormat();

	//initialize NEEDED OpenGL extensions for the windows platform
	void Windows_InitGLExtensions();

	HDC DeviceContextHandle; /**< the handle to the device context */
	HGLRC GLRenderingContextHandle; /**< the handle to the OpenGL rendering context*/
	HPALETTE PaletteHandle; /**< handle to the draw palette*/
	PIXELFORMATDESCRIPTOR PixelFormatDescriptor; /**< describes the pixel format to be used by OpenGL*/

	WNDCLASS WindowClass; /**< this describes the type of Window that Win32 will create*/
	HWND WindowHandle; /**<handle to the Win32 window itself */
	HINSTANCE InstanceHandle; /**< handle to the Win32 instance */
	
	PFNWGLSWAPINTERVALEXTPROC SwapIntervalEXT; /**< OpenGL extension callback for setting the swap interval(V-Sync)*/
	PFNWGLSWAPBUFFERSMSCOMLPROC SwapIntervalMSCOM; // what the holy fuck is MSCOM?
	PFNWGLGETEXTENSIONSSTRINGEXTPROC GetExtensionsStringEXT; /**< OpenGL extension for revealing available extensions*/

#else
	//uses the X11 system to initialize the window
	GLboolean Linux_Initialize();
	//uses the X11 system to set the size/resolution of the window
	void Linux_SetResolution(GLuint Width, GLuint Height);
	//uses the X11 system to set the window position relative to screen co-ordinates
	void Linux_SetPosition(GLuint X, GLuint Y);
	//uses the X11 system to set the mouse position relative to the window co-ordinates
	void Linux_SetMousePosition(GLuint X, GLuint Y);
	//uses the X11 system to toggle full screen mode 
	void Linux_FullScreen(GLboolean NewState);
	//uses the X11 system to toggle minimization
	void Linux_Minimize(GLboolean NewState);
	//uses the X11 system to toggle maximization
	void Linux_Maximize(GLboolean NewState);
	//uses the X11 system to restore the window
	void Linux_Restore();
	//uses the X11 system to toggle the window's event focus state
	void Linux_Focus(GLboolean NewState);
	//uses the X11 system to set the title bar of the window
	void Linux_SetTitleBar(const char* NewName);
	//uses the X11 system to set the icon of the window
	void Linux_SetIcon(const char* Icon, GLuint Width, GLuint Height);
	//uses the X11 system to initialize create an OpenGL context for the window
	GLboolean Linux_InitializeGL();
	//uses OpenGL extensions for Linux to toggle Vertical syncing
	void Linux_VerticalSync(GLint EnableSync);
	//shut down the window. closes all connections to the X11 system
	void Linux_Shutdown();
	//enables given window decoration via Win32
	void Linux_EnableDecorator(GLbitfield Decorator);
	//disables given window decoration via Win32
	void Linux_DisableDecorator(GLbitfield Decorator);
	//set the style of the window by enabling/disabling certain decorators
	void Linux_SetStyle(GLuint WindowStyle);

	//initialize the window manager Atomics needed for the X11 extended window manager
	void InitializeAtomics();
	//initialize the NEEDED OpenGL extensions that are supported on Linux
	void Linux_InitGLExtensions();

	//get the Handle To the Window
	Window GetWindowHandle();

	GLXFBConfig GetBestFrameBufferConfig();

	Window WindowHandle; /**< the X11 handle to the window. I wish they didn't name the type 'Window' */
	GLXContext Context; /**< the handle to the GLX rendering context */
	XVisualInfo* VisualInfo; /**< the handle to the Visual Information. similar purpose to PixelformatDesriptor*/
	GLint* Attributes;/**< attributes of the window. RGB, depth, stencil, etc */
	XSetWindowAttributes SetAttributes; /**< the attributes to be set for the window */
	GLbitfield Decorators; /**< enabled window decorators */
	
	//these are the callbacks for the GLX swap interval extension. 
	PFNGLXSWAPINTERVALMESAPROC SwapIntervalMESA; /**< the mesa swap interval extension */
	PFNGLXSWAPINTERVALEXTPROC SwapIntervalEXT; /**< the generic swap interval extension*/
	PFNGLXSWAPINTERVALSGIPROC SwapIntervalSGI;	 /**< the Silicon graphics swap interval extension*/

	/*these atomics are needed to change window states via the extended window manager
	I might move them to window manager considering these are essentially constants
	*/
	Atom AtomState;  /**< atom for the state of the window */	//_NET_WM_STATE
	Atom AtomHidden; /**< atom for the current hidden state of the window */	// _NET_WM_STATE_HIDDEN
	Atom AtomFullScreen; /**< atom for the fullscreen state of the window*/ //NET_WM_STATE_FULLSCREEN
	Atom AtomMaxHorz; /**< atom for the maximized horizontally state of the window*/ // _NET_WM_STATE_MAXIMIZED_HORZ
	Atom AtomMaxVert; /**< atom for the maximized vertically state of the window*/ // _NET_WM_STATE_MAXIMIZED_VERT
	Atom AtomClose;  /**< atom for closing the window */	// _NET_WM_CLOSE_WINDOW
	Atom AtomActive;  /**<atom for the active window */	//_NET_ACTIVE_WINDOW
	Atom AtomDemandsAttention; /**<atom for when the window demands attention*/	//_NET_WM_STATE_DEMANDS_ATTENTION
	Atom AtomFocused; /**<atom for the focused state of the window */ //_NET_WM_STATE_FOCUSED
	Atom AtomCardinal; /**< atom for cardinal coordinates */ //_NET_WM_CARDINAL
	Atom AtomIcon; /**< atom for the icon of the window */ //_NET_WM_ICON
	Atom AtomHints; /**< atom for the window decorations */ //_NET_WM_HINTS

	Atom AtomWindowType; /**< atom for the type of window*/
	Atom AtomWindowTypeDesktop; /**< atom for the desktop window type */ //_NET_WM_WINDOW_TYPE_SPLASH
	Atom AtomWindowTypeSplash; /**< atom for the splash screen window type */
	Atom AtomWindowTypeNormal; /**<atom for the normal splash screen window type*/

	Atom AtomAllowedActions; /**< atom for allowed window actions */
	Atom AtomActionResize; /**< atom for allowing the window to be resized*/
	Atom AtomActionMinimize; /**< atom for allowing the window to be minimized */
	Atom AtomActionShade; /**< atom for allowing the window to be shaded */
	Atom AtomActionMaximizeHorz; /**< atom for allowing the window to be maximized horizontally */
	Atom AtomActionMaximizeVert; /**< atom for allowing the window to be maximized vertically */
	Atom AtomActionClose; /**< atom for allowing the window to be closed*/

	Atom AtomDesktopGeometry; /**< atom for Dedktop Geometry */

#endif
};

#endif
