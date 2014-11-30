#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H


#include "WindowAPI_Defs.h"
#include "Window.h"
#include "Tools.h"

class FWindow;
class Foundation_Tools;

class WindowManager
{
	friend FWindow;
	friend Foundation_Tools;
	public:

	WindowManager();
	~WindowManager();
		/**
		* shutdown and delete all windows in the manager
		*/
		static void ShutDown();

		/**get a pointer to a window via name or index */
		static FWindow* GetWindowByName(const char* WindowName);
		static FWindow* GetWindowByIndex(GLuint WindowIndex);

		/**
		*add a window to the manager. i ripped off a tree feature
		*that allows the user to create multiple windows easily
		*/
		static WindowManager* AddWindow(FWindow* NewWindow);

		//return the total amount of windows the manager has
		static GLuint GetNumWindows();

		//gets and sets for the mouse position in the screen
		static void GetMousePositionInScreen(GLuint& X, GLuint& Y);
		static GLuint* GetMousePositionInScreen();
		static void SetMousePositionInScreen(GLuint X, GLuint Y);

		// get the screen resolution for the screen that is being drawn to
		static GLuint* GetScreenResolution();
		static void GetScreenResolution(GLuint& Width, GLuint& Height);

		//these are another way to set and get window variables
		//apart from the functions that are available to the user
		//via each window
		
		//sets and gets for window resolution
		static void GetWindowResolution(const char* WindowName, GLuint& Width, GLuint& Height);
		static void GetWindowResolution(GLuint WindowIndex, GLuint& Width, GLuint& Height);
		static GLuint* GetWindowResolution(const char* WindowName);
		static GLuint* GetWindowResolution(GLuint WindowIndex);
		static void SetWindowResolution(const char* WindowName, GLuint Width, GLuint Height);
		static void SetWindowResolution(GLuint WindowIndex, GLuint Width, GLuint Height);

		//sets and gets for window position
		static void GetWindowPosition(const char* WindowName, GLuint& X, GLuint& Y);
		static void GetWindowPosition(GLuint WindowIndex, GLuint& X, GLuint& Y);
		static GLuint* GetWindowPosition(const char* WindowName);
		static GLuint* GetWindowPosition(GLuint WindowIndex);
		static void SetWindowPosition(const char* WindowName, GLuint X, GLuint Y);
		static void SetWindowPosition(GLuint WindowIndex, GLuint X, GLuint Y);

		//sets and gets for the mouse position in window
		static void GetMousePositionInWindow(const char* WindowName, GLuint& X, GLuint& Y);
		static void GetMousePositionInWindow(GLuint WindowIndex, GLuint& X, GLuint& Y);
		static GLuint* GetMousePositionInWindow(const char* WindowName);
		static GLuint* GetMousePositionInWindow(GLuint WindowIndex);
		static void SetMousePositionInWindow(const char* WindowName, GLuint X, GLuint Y);
		static void SetMousePositionInWindow(GLuint WindowIndex, GLuint X, GLuint Y);

		//gets for window keys
		static GLboolean WindowGetKey(const char* WindowName, GLuint Key);
		static GLboolean WindowGetKey(GLuint WindowIndex, GLuint Key);

		//gets for window should close
		static GLboolean GetWindowShouldClose(const char* WindowName);
		static GLboolean GetWindowShouldClose(GLuint WindowIndex);

		//swap buffers
		static void WindowSwapBuffers(const char* WindowName);
		static void WindowSwapBuffers(GLuint WindowIndex);

		//sets and gets for fullscreen
		static void SetFullScreen(const char* WindowName, GLboolean NewState);
		static void SetFullScreen(GLuint WindowIndex, GLboolean NewState);
		static GLboolean GetWindowIsFullScreen(const char* WindowName);
		static GLboolean GetWindowIsFullScreen(GLuint WindowIndex);

		//gets and sets for minimized
		static GLboolean GetWindowIsMinimized(const char* WindowName);
		static GLboolean GetWindowIsMinimized(GLuint WindowIndex);
		static void MinimizeWindow(const char* WindowName, GLboolean NewState);
		static void MinimizeWindow(GLuint WindowIndex, GLboolean NewState);

		//gets and sets for maximised state
		static GLboolean GetWindowIsMaximized(const char* WindowName);
		static GLboolean GetWindowIsMaximized(GLuint WindowIndex);
		static void MaximizeWindow(const char* WindowName, GLboolean NewState);
		static void MaximizeWindow(GLuint WindowIndex, GLboolean NewState);

		//gets and sets for window name and index
		static const char* GetWindowName(GLuint WindowIndex);
		static GLuint GetWindowIndex(const char*  WindowName);

		static void SetWindowTitleBar(const char* WindowName, const char* NewName);
		static void SetWindowTitleBar(GLuint WindowIndex, const char* NewName);

		//gets and sets window is in focus(Linux only?)
		static GLboolean GetWindowIsInFocus(const char* WindowName);
		static GLboolean GetWindowIsInFocus(GLuint WindowIndex);
		static void FocusWindow(const char* WindowName, GLboolean NewState);
		static void FocusWindow(GLuint WindowIndex, GLboolean NewState);

		//gets and sets for restoring the window
		static void RestoreWindow(const char* WindowName);
		static void RestoreWindow(GLuint WindowIndex);

		//get window obscurity. I feel like this is completely useless
		//static GLboolean GetWindowIsObscured(const char* WindowName);
		//static GLboolean GetWindowIsObscured(GLuint WindowIndex);

		//enable vertical sync on selected window
		static void SetWindowSwapInterval(const char* WindowName, GLint EnableSync);
		static void SetWindowSwapInterval(GLuint WindowIndex, GLint EnableSync);
		
		//initialize the window manager
		static void Initialize();

		//ask the window to poll for window events
		static void PollForEvents();

		//remove a window from the manager
		static void RemoveWindow(FWindow* WindowToBeRemoved);

		//set callbacks for the selected window
		static void SetWindowOnKeyEvent(const char* WindowName, OnKeyEvent OnKey);
		static void SetWindowOnKeyEvent(GLuint WindowIndex, OnKeyEvent OnKey);

		static void SetWindowOnMouseButtonEvent(const char* WindowName, OnMouseButtonEvent a_OnMouseButtonEvent);
		static void SetWindowOnMouseButtonEvent(GLuint WindowIndex, OnMouseButtonEvent a_OnMouseButtonEvent);

		static void SetWindowOnMouseWheelEvent(const char* WindowName, OnMouseWheelEvent OnMouseWheelEvent);
		static void SetWindowOnMouseWheelEvent(GLuint WindowIndex, OnMouseWheelEvent OnMouseWheelEvent);

		static void SetWindowOnDestroyed(const char* WindowName, OnDestroyedEvent OnDestroyed);
		static void SetWindowOnDestroyed(GLuint WindowIndex, OnDestroyedEvent OnDestroyed);

		static void SetWindowOnMaximized(const char* WindowName, OnMaximizedEvent OnMaximized);
		static void SetWindowOnMaximized(GLuint WindowIndex, OnMaximizedEvent OnMaximized);

		static void SetWindowOnMinimized(const char* WindowName, OnMinimizedEvent a_OnMiniimzed);
		static void SetWindowOnMinimized(GLuint WindowIndex, OnMinimizedEvent a_OnMiniimzed);

	//	static void SetWindowOnRestored(const char* WindowName, OnRestoredEvent OnRestored);
		//static void SetWindowOnRestored(GLuint WindowIndex, OnRestoredEvent OnRestored);

		static void SetWindowOnFocus(const char* WindowName, OnFocusEvent OnFocus);
		static void SetWindowOnFocus(GLuint WindowIndex, OnFocusEvent OnFocus);

		static void SetWindowOnMoved(const char* WindowName, OnMovedEvent OnMoved);
		static void SetWindowOnMoved(GLuint WindowIndex, OnMovedEvent OnMoved);

		static void SetWindowOnResize(const char* WindowName, OnResizeEvent OnResize);
		static void SetWindowOnResize(GLuint WindowIndex, OnResizeEvent OnResize);

		static void SetWindowOnMouseMove(const char* WindowName, OnMouseMoveEvent OnMouseMove);
		static void SetWindowOnMouseMove(GLuint WindowIndex, OnMouseMoveEvent OnMouseMove);

	private:

		//make sure the window exists in the window manager
		static GLboolean DoesExist(const char* WindowName);
		static GLboolean DoesExist(GLuint WindowIndex);

		//get a static reference to the window manager
		static WindowManager* GetInstance();

		std::list<FWindow*> Windows; /**< The FWindows storage*/
		static WindowManager* Instance; /**< The static reference to the WindowManager */

		GLuint ScreenResolution[2]; /**< the resolution of the screen as an array */
		GLuint ScreenMousePosition[2]; /**< the position of the mouse relative to screen coordinates */

#if defined(CURRENT_OS_WINDOWS)
		LRESULT CALLBACK WindowProcedure(HWND WindowHandle, GLuint Message, WPARAM WordParam, LPARAM LongParam);
	
		static LRESULT CALLBACK StaticWindowProcedure(HWND WindowHandle, UINT Message, WPARAM WordParam, LPARAM LongParam);

		static FWindow* GetWindowByHandle(HWND WindowHandle);

		static void Windows_PollForEvents();
		static void Windows_Initialize();
		static void Windows_Shutdown();
		static void Windows_SetMousePositionInScreen(GLuint X, GLuint Y);

		static void CreateTerminal();

		HDC DeviceContextHandle; /**< the device context handle for the window*/
		MSG Message; /**< the Win32 message that contains event information */
#endif

#if defined(CURRENT_OS_LINUX)
		static FWindow* GetWindowByHandle(Window WindowHandle);
		static FWindow* GetWindowByEvent(XEvent Event);

		static void Linux_Initialize();
		static void Linux_Shutdown();

		static void Linux_PollForEvents();
		static void Linux_SetMousePositionInScreen(GLuint X, GLuint Y);
		static Display* GetDisplay();

		Display* m_Display; /**< a reference to the X11 display */
		XEvent m_Event; /**< the current X11 event*/
#endif
};
#endif 
