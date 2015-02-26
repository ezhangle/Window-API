#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H


#include "WindowAPI_Defs.h"
#include "Window.h"

class FWindow;

class WindowManager
{
	friend FWindow;
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
		static GLboolean GetMousePositionInScreen(GLuint& X, GLuint& Y);
		static GLuint* GetMousePositionInScreen();
		static GLboolean SetMousePositionInScreen(GLuint X, GLuint Y);

		// get the screen resolution for the screen that is being drawn to
		static GLuint* GetScreenResolution();
		static GLboolean GetScreenResolution(GLuint& Width, GLuint& Height);

		//these are another way to set and get window variables
		//apart from the functions that are available to the user
		//via each window
		
		//sets and gets for window resolution
		static GLboolean GetWindowResolution(const char* WindowName, GLuint& Width, GLuint& Height);
		static GLboolean GetWindowResolution(GLuint WindowIndex, GLuint& Width, GLuint& Height);
		static GLuint* GetWindowResolution(const char* WindowName);
		static GLuint* GetWindowResolution(GLuint WindowIndex);
		static GLboolean SetWindowResolution(const char* WindowName, GLuint Width, GLuint Height);
		static GLboolean SetWindowResolution(GLuint WindowIndex, GLuint Width, GLuint Height);

		//sets and gets for window position
		static GLboolean GetWindowPosition(const char* WindowName, GLuint& X, GLuint& Y);
		static GLboolean GetWindowPosition(GLuint WindowIndex, GLuint& X, GLuint& Y);
		static GLuint* GetWindowPosition(const char* WindowName);
		static GLuint* GetWindowPosition(GLuint WindowIndex);
		static GLboolean SetWindowPosition(const char* WindowName, GLuint X, GLuint Y);
		static GLboolean SetWindowPosition(GLuint WindowIndex, GLuint X, GLuint Y);

		//sets and gets for the mouse position in window
		static GLboolean GetMousePositionInWindow(const char* WindowName, GLuint& X, GLuint& Y);
		static GLboolean GetMousePositionInWindow(GLuint WindowIndex, GLuint& X, GLuint& Y);
		static GLuint* GetMousePositionInWindow(const char* WindowName);
		static GLuint* GetMousePositionInWindow(GLuint WindowIndex);
		static GLboolean SetMousePositionInWindow(const char* WindowName, GLuint X, GLuint Y);
		static GLboolean SetMousePositionInWindow(GLuint WindowIndex, GLuint X, GLuint Y);

		//gets for window keys
		static GLboolean WindowGetKey(const char* WindowName, GLuint Key);
		static GLboolean WindowGetKey(GLuint WindowIndex, GLuint Key);

		//gets for window should close
		static GLboolean GetWindowShouldClose(const char* WindowName);
		static GLboolean GetWindowShouldClose(GLuint WindowIndex);

		//swap buffers
		static GLboolean WindowSwapBuffers(const char* WindowName);
		static GLboolean WindowSwapBuffers(GLuint WindowIndex);

		//sets and gets for fullscreen
		static GLboolean SetFullScreen(const char* WindowName, GLboolean NewState);
		static GLboolean SetFullScreen(GLuint WindowIndex, GLboolean NewState);
		static GLboolean GetWindowIsFullScreen(const char* WindowName);
		static GLboolean GetWindowIsFullScreen(GLuint WindowIndex);

		//gets and sets for minimized
		static GLboolean GetWindowIsMinimized(const char* WindowName);
		static GLboolean GetWindowIsMinimized(GLuint WindowIndex);
		static GLboolean MinimizeWindow(const char* WindowName, GLboolean NewState);
		static GLboolean MinimizeWindow(GLuint WindowIndex, GLboolean NewState);

		//gets and sets for maximised state
		static GLboolean GetWindowIsMaximized(const char* WindowName);
		static GLboolean GetWindowIsMaximized(GLuint WindowIndex);
		static GLboolean MaximizeWindow(const char* WindowName, GLboolean NewState);
		static GLboolean MaximizeWindow(GLuint WindowIndex, GLboolean NewState);

		//gets and sets for window name and index
		static const char* GetWindowName(GLuint WindowIndex);
		static GLuint GetWindowIndex(const char*  WindowName);

		static GLboolean SetWindowTitleBar(const char* WindowName, const char* NewName);
		static GLboolean SetWindowTitleBar(GLuint WindowIndex, const char* NewName);

		static GLboolean SetWindowIcon(const char* WindowName, const char* Icon, GLuint Width, GLuint Height);
		static GLboolean SetwindowIcon(GLuint WindowIndex, const char* Icon, GLuint Width, GLuint Height);

		//gets and sets window is in focus(Linux only?)
		static GLboolean GetWindowIsInFocus(const char* WindowName);
		static GLboolean GetWindowIsInFocus(GLuint WindowIndex);
		static GLboolean FocusWindow(const char* WindowName, GLboolean NewState);
		static GLboolean FocusWindow(GLuint WindowIndex, GLboolean NewState);

		//gets and sets for restoring the window
		static GLboolean RestoreWindow(const char* WindowName);
		static GLboolean RestoreWindow(GLuint WindowIndex);

		//get window obscurity. I feel like this is completely useless
		//static GLboolean GetWindowIsObscured(const char* WindowName);
		//static GLboolean GetWindowIsObscured(GLuint WindowIndex);

		//enable vertical sync on selected window
		static GLboolean SetWindowSwapInterval(const char* WindowName, GLint EnableSync);
		static GLboolean SetWindowSwapInterval(GLuint WindowIndex, GLint EnableSync);
		
		//initialize the window manager
		static GLboolean Initialize();
		static GLboolean IsInitialized();

		//ask the window to poll for window events
		static GLboolean PollForEvents();
		static GLboolean WaitForEvents();

		//NOTE: moved to TinyClock API
		//static GLdouble GetTotalTime();
		//static GLdouble GetDeltaTime();

		//remove a window from the manager
		static GLboolean RemoveWindow(FWindow* WindowToBeRemoved);

		//set the styleof the given window
		static GLboolean SetWindowStyle(const char* WindowName, GLuint WindowStyle);
		static GLboolean SetWindowStyle(GLuint WindowIndex, GLuint WindowStyle);

		//enable the given decorators of the given window
		static GLboolean EnableWindowDecorator(const char* WindowName, GLbitfield Decorators);
		static GLboolean EnableWindowDecorator(GLuint WindowIndex, GLbitfield Decorators);

		//disable the given decorators of the given window
		static GLboolean DisableWindowDecorator(const char* WindowName, GLbitfield Decorators);
		static GLboolean DisableWindowDecorator(GLuint WindowIndex, GLbitfield Decorators); 

		//set callbacks for the selected window
		static GLboolean SetWindowOnKeyEvent(const char* WindowName, OnKeyEvent OnKey);
		static GLboolean SetWindowOnKeyEvent(GLuint WindowIndex, OnKeyEvent OnKey);

		static GLboolean SetWindowOnMouseButtonEvent(const char* WindowName, OnMouseButtonEvent a_OnMouseButtonEvent);
		static GLboolean SetWindowOnMouseButtonEvent(GLuint WindowIndex, OnMouseButtonEvent a_OnMouseButtonEvent);

		static GLboolean SetWindowOnMouseWheelEvent(const char* WindowName, OnMouseWheelEvent OnMouseWheelEvent);
		static GLboolean SetWindowOnMouseWheelEvent(GLuint WindowIndex, OnMouseWheelEvent OnMouseWheelEvent);

		static GLboolean SetWindowOnDestroyed(const char* WindowName, OnDestroyedEvent OnDestroyed);
		static GLboolean SetWindowOnDestroyed(GLuint WindowIndex, OnDestroyedEvent OnDestroyed);

		static GLboolean SetWindowOnMaximized(const char* WindowName, OnMaximizedEvent OnMaximized);
		static GLboolean SetWindowOnMaximized(GLuint WindowIndex, OnMaximizedEvent OnMaximized);

		static GLboolean SetWindowOnMinimized(const char* WindowName, OnMinimizedEvent a_OnMiniimzed);
		static GLboolean SetWindowOnMinimized(GLuint WindowIndex, OnMinimizedEvent a_OnMiniimzed);

	//	static void SetWindowOnRestored(const char* WindowName, OnRestoredEvent OnRestored);
		//static void SetWindowOnRestored(GLuint WindowIndex, OnRestoredEvent OnRestored);

		static GLboolean SetWindowOnFocus(const char* WindowName, OnFocusEvent OnFocus);
		static GLboolean SetWindowOnFocus(GLuint WindowIndex, OnFocusEvent OnFocus);

		static GLboolean SetWindowOnMoved(const char* WindowName, OnMovedEvent OnMoved);
		static GLboolean SetWindowOnMoved(GLuint WindowIndex, OnMovedEvent OnMoved);

		static GLboolean SetWindowOnResize(const char* WindowName, OnResizeEvent OnResize);
		static GLboolean SetWindowOnResize(GLuint WindowIndex, OnResizeEvent OnResize);

		static GLboolean SetWindowOnMouseMove(const char* WindowName, OnMouseMoveEvent OnMouseMove);
		static GLboolean SetWindowOnMouseMove(GLuint WindowIndex, OnMouseMoveEvent OnMouseMove);

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

		GLboolean Initialized; /**<whether the window manager has been initialized*/

		//NOTE: moved to TinyClock API
		//GLdouble TotalTime; /**< How long the window manager has been running*/ 
		//GLdouble PreviousTime; /**the previous amount of time between cycles. used to calculate Delta time */
		//GLdouble DeltaTime; /** How much time (in ms) between CPU cycles*/

#if defined(_WIN32) || defined(_WIN64)
		LRESULT CALLBACK WindowProcedure(HWND WindowHandle, GLuint Message, WPARAM WordParam, LPARAM LongParam);
	
		static LRESULT CALLBACK StaticWindowProcedure(HWND WindowHandle, UINT Message, WPARAM WordParam, LPARAM LongParam);

		static FWindow* GetWindowByHandle(HWND WindowHandle);

		static GLboolean Windows_PollForEvents();
		static GLboolean Windows_WaitForEvents();
		static GLboolean Windows_Initialize();
		static GLboolean Windows_Shutdown();
		static GLboolean Windows_SetMousePositionInScreen(GLuint X, GLuint Y);

		static void CreateTerminal();
		static GLuint Windows_TranslateKey(WPARAM WordParam, LPARAM LongParam);

		HDC DeviceContextHandle; /**< the device context handle for the window*/
		MSG Message; /**< the Win32 message that contains event information */
#else
		static FWindow* GetWindowByHandle(Window WindowHandle);
		static FWindow* GetWindowByEvent(XEvent Event);

		static GLboolean Linux_Initialize();
		static void Linux_Shutdown();

		static GLboolean Linux_PollForEvents();
		static GLboolean Linux_WaitForEvents();
		static GLvoid Linux_ProcessEvents(XEvent CurrentEvent);
		static GLboolean Linux_SetMousePositionInScreen(GLuint X, GLuint Y);
		static Display* GetDisplay();

		static GLuint Linux_TranslateKey(GLuint KeySym);
		static const char* Linux_GetEventType(XEvent Event);

		Display* m_Display; /**< a reference to the X11 display */
		XEvent m_Event; /**< the current X11 event*/
#endif
};
#endif 
