/**********************************************************************************************//**
 * @file	WindowAPI\Window.cpp
 *
 * @brief	Implements the FWindow class.
 **************************************************************************************************/

#include <limits.h>
#include "Window.h"
#include "WindowManager.h"
#include "Tools.h"

#if defined(CURRENT_OS_LINUX)
#include <cstring>
#endif

/**********************************************************************************************//**
 * @fn	FWindow::FWindow(const char* WindowName, GLuint Width , GLuint Height , GLuint ColourBits , GLuint DepthBits , GLuint StencilBits )
 *
 * @brief	Constructor.
 *
 * @author	Ziyad Barakat
 * @date	29/11/2014
 *
 * @param	WindowName 	Name of the window.
 * @param	Width	   	The width.
 * @param	Height	   	The height.
 * @param	ColourBits 	The colour bits.
 * @param	DepthBits  	The depth bits.
 * @param	StencilBits	The stencil bits.
 **************************************************************************************************/

FWindow::FWindow(const char*  WindowName,
	GLuint Width /* = 1280 */,
	GLuint Height /* = 720 */,
	GLuint ColourBits /* = 32 */, 
	GLuint DepthBits /* = 8 */,
	GLuint StencilBits /* = 8 */) :
	Name(WindowName),
	ColourBits(ColourBits),
	DepthBits(DepthBits),
	StencilBits(StencilBits)
{
	Resolution[0] = Width;
	Resolution[1] = Height;
	Position[0] = 0;
	Position[1] = 0;
	ShouldClose = GL_FALSE;
	EXTSwapControlSupported = GL_FALSE;
	SGISwapControlSupported = GL_FALSE;
	MESASwapControlSupported = GL_FALSE;

	if(!Foundation_Tools::IsValid(WindowName))
	{
		printf("that was not a valid window name");
		exit(0);
	}

	InitializeEvents();

	CurrentState = WINDOWSTATE_NORMAL;
}

/**********************************************************************************************//**
 * @fn	FWindow::~FWindow()
 *
 * @brief	FWindow Destructor.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

FWindow::~FWindow()
{
	Shutdown();
}

/**********************************************************************************************//**
 * @fn	void FWindow::Shutdown()
 *
 * @brief	Shuts down this object and frees any resources it is using.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

void FWindow::Shutdown()
{
#if defined (CURRENT_OS_WINDOWS)
	Windows_Shutdown();
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_Shutdown();
#endif
}

/**********************************************************************************************//**
 * @fn	void FWindow::Initialize()
 *
 * @brief	Initializes this object.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

void FWindow::Initialize()
{
#if defined(CURRENT_OS_WINDOWS)
	Windows_Initialize(Name);
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_Initialize();
#endif
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::GetShouldClose()
 *
 * @brief	Gets should close.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	The should close.
 **************************************************************************************************/

GLboolean FWindow::GetShouldClose()
{
	return ShouldClose;
}

/**********************************************************************************************//**
 * @fn	void FWindow::InitializeEvents()
 *
 * @brief	Initializes the events.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

void FWindow::InitializeEvents()
{
	KeyEvent = nullptr;
	MouseButtonEvent = nullptr;
	MouseWheelEvent = nullptr;
	DestroyedEvent = nullptr;
	MaximizedEvent = nullptr;
	MinimizedEvent = nullptr;
//	RestoredEvent = nullptr;
	MovedEvent = nullptr;
	MouseMoveEvent = nullptr;
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::GetKeyState(GLuint Key)
 *
 * @brief	Gets key state.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	Key	The key.
 *
 * @return	The key state.
 **************************************************************************************************/

GLboolean FWindow::GetKeyState(GLuint Key)
{
	return Keys[Key];
}

/**********************************************************************************************//**
 * @fn	void FWindow::InitializeGL()
 *
 * @brief	Initializes OpenGL for this window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

void FWindow::InitializeGL()
{
#if defined(CURRENT_OS_WINDOWS)
	Windows_InitializeGL();
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_InitializeGL();
#endif
}

/**********************************************************************************************//**
 * @fn	void FWindow::SwapDrawBuffers()
 *
 * @brief	Swap draw buffers.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

void FWindow::SwapDrawBuffers()
{
#if defined(CURRENT_OS_WINDOWS)
	SwapBuffers(DeviceContextHandle);
#endif

#if defined(CURRENT_OS_LINUX)
	glXSwapBuffers(WindowManager::GetDisplay(), WindowHandle);
#endif
}

/**********************************************************************************************//**
 * @fn	void FWindow::SetSwapInterval(GLint SwapSetting)
 *
 * @brief	Sets swap interval(V-sync).
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	SwapSetting	The swap setting.
 **************************************************************************************************/

void FWindow::SetSwapInterval(GLint SwapSetting)
{
	CurrentSwapInterval = SwapSetting;
#if defined(CURRENT_OS_WINDOWS)
	Windows_VerticalSync(SwapSetting);
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_VerticalSync(SwapSetting);
#endif
}

/**********************************************************************************************//**
 * @fn	GLuint FWindow::GetCurrentState()
 *
 * @brief	Gets current state.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	The current state.
 **************************************************************************************************/

GLuint FWindow::GetCurrentState()
{
	return CurrentState;
}

/**********************************************************************************************//**
 * @fn	void FWindow::SetCurrentState(GLuint NewState)
 *
 * @brief	Sets the current state of the window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	NewState	new state of the window.
 **************************************************************************************************/

void FWindow::SetCurrentState(GLuint NewState)
{
	/**
	* first we restore the window to make moving from state to state as easy as possible
	*/
	Restore();

	switch(NewState)
	{
		case WINDOWSTATE_MAXIMIZED:
			{
				Maximize(GL_TRUE);
				break;
			}

		case WINDOWSTATE_MINIMIZED:
			{
				Minimize(GL_TRUE);
				break;
			}

			case WINDOWSTATE_FULLSCREEN:
			{
				FullScreen(GL_FALSE);
				break;
			}

			default:
			{
				break;
			}
	}
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::GetIsFullScreen()
 *
 * @brief	returns whether the window is in fullscreen mode.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	Whether the window is currently in fullscreen mode.
 **************************************************************************************************/

GLboolean FWindow::GetIsFullScreen()
{
	return (CurrentState == WINDOWSTATE_FULLSCREEN);
}

/**********************************************************************************************//**
 * @fn	void FWindow::FullScreen(GLboolean NewState)
 *
 * @brief	toggle the fullscreen mode for the window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	NewState	whether the window should be in fullscreen mode.
 **************************************************************************************************/

void FWindow::FullScreen(GLboolean NewState)
{
	if(NewState)
	{
		CurrentState = WINDOWSTATE_FULLSCREEN;
	}

	else
	{
		CurrentState = WINDOWSTATE_NORMAL;
	}

#if defined(CURRENT_OS_LINUX)
	Linux_FullScreen(NewState);
#endif

#if defined(CURRENT_OS_WINDOWS)
	Windows_FullScreen(NewState);
#endif
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::GetIsMinimized()
 *
 * @brief	Gets whether the window is minimized.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	whether the window is currently minimized.
 **************************************************************************************************/

GLboolean FWindow::GetIsMinimized()
{
	return (CurrentState == WINDOWSTATE_MINIMIZED);
}

/**********************************************************************************************//**
 * @fn	void FWindow::Minimize(GLboolean NewState)
 *
 * @brief	set the window to be minimized depending on NewState.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	NewState	whether the window should be minimized.
 **************************************************************************************************/

void FWindow::Minimize(GLboolean NewState)
{
	if(NewState)
	{
		CurrentState = WINDOWSTATE_MINIMIZED;
	}

	else
	{
		CurrentState = WINDOWSTATE_NORMAL;
	}

#if defined(CURRENT_OS_WINDOWS)
	Windows_Minimize(NewState);
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_Minimize(NewState);
#endif	
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::GetIsMaximized()
 *
 * @brief	Gets whether the window is maximized.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	whether the window is currently minimized.
 **************************************************************************************************/

GLboolean FWindow::GetIsMaximized()
{
	return (CurrentState == WINDOWSTATE_MAXIMIZED) ;
}

/**********************************************************************************************//**
 * @fn	void FWindow::Maximize(GLboolean NewState)
 *
 * @brief	Maximizes the window depending on New state.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	NewState	Whether to minimize the window.
 **************************************************************************************************/

void FWindow::Maximize(GLboolean NewState)
{
	if(NewState)
	{
		CurrentState = WINDOWSTATE_MAXIMIZED;
	}

	else
	{
		CurrentState = WINDOWSTATE_NORMAL;
	}

#if defined(CURRENT_OS_WINDOWS)
	Windows_Maximize(NewState);
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_Maximize(NewState);
#endif
}

/**********************************************************************************************//**
 * @fn	void FWindow::Restore()
 *
 * @brief	Restores the window to its default setting.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

void FWindow::Restore()
{
	switch(CurrentState)
	{
		case WINDOWSTATE_MAXIMIZED:
			{
				Maximize(GL_FALSE);
				break;
			}

		case WINDOWSTATE_FULLSCREEN:
			{
				FullScreen(GL_FALSE);
				break;
			}
	}

	CurrentState = WINDOWSTATE_NORMAL;
#if defined(CURRENT_OS_WINDOWS)
	Windows_Restore();
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_Restore();
#endif
}

/**********************************************************************************************//**
 * @fn	void FWindow::GetResolution(GLuint& Width, GLuint& Height)
 *
 * @brief	Gets the resolution of the window by setting width and height.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param [in,out]	Width 	The width.
 * @param [in,out]	Height	The height.
 **************************************************************************************************/

void FWindow::GetResolution(GLuint& Width, GLuint& Height)
{
	Width = Resolution[0];
	Height = Resolution[1];
}

/**********************************************************************************************//**
 * @fn	GLuint* FWindow::GetResolution()
 *
 * @brief	Gets the resolution of the window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	null if it fails, else the resolution as an array. Resolution[0] always returns the
 *  width and Resolution[1] always returns the Height.
 **************************************************************************************************/

GLuint* FWindow::GetResolution()
{
	return Resolution;
}

/**********************************************************************************************//**
 * @fn	void FWindow::SetResolution(GLuint Width, GLuint Height)
 *
 * @brief	Sets the resolution of the window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	Width 	The new width of the window.
 * @param	Height	The new height of the window.
 **************************************************************************************************/

void FWindow::SetResolution(GLuint Width, GLuint Height)
{
	Resolution[0] = Width;
	Resolution[1] = Height;

#if defined(CURRENT_OS_WINDOWS)
	Windows_SetResolution(Resolution[0], Resolution[1]);
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_SetResolution(Width, Height);	
#endif

	glViewport(0, 0, Resolution[0], Resolution[1]);
}

/**********************************************************************************************//**
 * @fn	void FWindow::GetMousePosition(GLuint& X, GLuint& Y)
 *
 * @brief	Gets mouse position relative to the window coordinates by setting X and Y.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param [in,out]	X	The X position of the mouse.
 * @param [in,out]	Y	The Y position of the mouse.
 **************************************************************************************************/

void FWindow::GetMousePosition(GLuint& X, GLuint& Y)
{
	X = MousePosition[0];
	Y = MousePosition[1];
}

/**********************************************************************************************//**
 * @fn	GLuint* FWindow::GetMousePosition()
 *
 * @brief	Gets mouse position relative to the window as an array.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	null if it fails, else the mouse position. MousePosition[0] always returns the X and
 *  MousePosition[1] always returns the Y.
 **************************************************************************************************/

GLuint* FWindow::GetMousePosition()
{
	return MousePosition;
}

/**********************************************************************************************//**
 * @fn	void FWindow::SetMousePosition(GLuint X, GLuint Y)
 *
 * @brief	Sets mouse position.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	X	The new X position of the mouse relative to the window coordinates.
 * @param	Y	The new Y position of the mouse relative to the window coordinates.
 **************************************************************************************************/

void FWindow::SetMousePosition(GLuint X, GLuint Y)
{
	MousePosition[0] = X;
	MousePosition[1] = Y;
#if defined(CURRENT_OS_WINDOWS)
	Windows_SetMousePosition(X, Y);
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_SetMousePosition(X, Y);
#endif
}

/**********************************************************************************************//**
 * @fn	void FWindow::GetPosition(GLuint& X, GLuint& Y)
 *
 * @brief	Gets a position.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param [in,out]	X	The X coordinate of the window position relative to screen coordinates.
 * @param [in,out]	Y	The Y coordinates of the window position relative to screen coordinates.
 **************************************************************************************************/

void FWindow::GetPosition(GLuint& X, GLuint& Y)
{
	X = Position[0];
	Y = Position[1];
}

/**********************************************************************************************//**
 * @fn	GLuint* FWindow::GetPosition()
 *
 * @brief	Gets the position.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	null if it fails, else the position. Position[0] always returns the X coordinate of
 *  the window relative to screen coordinates and Position[1] always returns the Y coordinates of
 *  the window.
 **************************************************************************************************/

GLuint* FWindow::GetPosition()
{
	return Position;
}

/**********************************************************************************************//**
 * @fn	void FWindow::SetPosition(GLuint X, GLuint Y)
 *
 * @brief	Sets a position.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	X	The new X coordinate of the window position relative to screen coordinates.
 * @param	Y	The new Y coordinate of the window position relative to screen coordinates.
 **************************************************************************************************/

void FWindow::SetPosition(GLuint X, GLuint Y)
{
	Position[0] = X;
	Position[1] = Y;
#if defined(CURRENT_OS_WINDOWS)
	Windows_SetPosition(Position[0], Position[1]);
#endif
	
#if defined(CURRENT_OS_LINUX)
	Linux_SetPosition(X, Y);
#endif
}

/**********************************************************************************************//**
 * @fn	const char* FWindow::GetWindowName()
 *
 * @brief	Gets window name.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	null if it fails, else the window name.
 **************************************************************************************************/

const char* FWindow::GetWindowName()
{
	return Name;
}

/**********************************************************************************************//**
 * @fn	void FWindow::SetTitleBar(const char* NewTitle)
 *
 * @brief	Sets title bar.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	NewTitle	The new title bar of the window.
 **************************************************************************************************/

void FWindow::SetTitleBar(const char* NewTitle)
{
	if(NewTitle != nullptr)
	{
#if defined(CURRENT_OS_LINUX)
		Linux_SetTitleBar(NewTitle);
#endif

#if defined(CURRENT_OS_WINDOWS)
		Windows_SetTitleBar(NewTitle);
#endif
	}
}

/**********************************************************************************************//**
 * @fn	void FWindow::MakeCurrentContext()
 *
 * @brief	Makes the window be the current OpenGL context to be drawn to.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

void FWindow::MakeCurrentContext()
{
#if defined(CURRENT_OS_WINDOWS)
	wglMakeCurrent(DeviceContextHandle, GLRenderingContextHandle);
#endif

#if defined(CURRENT_OS_LINUX)
	glXMakeCurrent(WindowManager::GetDisplay(), WindowHandle, Context);
#endif
}

/**********************************************************************************************//**
 * @fn	void FWindow::InitGLExtensions()
 *
 * @brief	Initializes the OpenGL extensions for this window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

void FWindow::InitGLExtensions()
{
#if defined(CURRENT_OS_WINDOWS)
	Windows_InitGLExtensions();
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_InitGLExtensions();
#endif

}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::GetInFocus()
 *
 * @brief	Gets whether the window is in event focus.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	The in focus.
 **************************************************************************************************/

GLboolean FWindow::GetInFocus()
{
	return InFocus;
}

/**********************************************************************************************//**
 * @fn	void FWindow::Focus(GLboolean NewState)
 *
 * @brief	put the window into event focus.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	NewState	whether to put the window into event focus.
 **************************************************************************************************/

void FWindow::Focus(GLboolean NewState)
{
	InFocus = NewState;

#if defined(CURRENT_OS_LINUX)
	Linux_Focus(NewState);	
#endif

#if defined(CURRENT_OS_WINDOWS)
	Windows_Focus(NewState);
#endif
}

/**********************************************************************************************//**
 * @fn	void FWindow::SetOnKeyEvent(OnKeyEvent OnKey)
 *
 * @brief	Sets on key event.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	OnKey	The on key event.
 **************************************************************************************************/

void FWindow::SetOnKeyEvent(OnKeyEvent OnKey)
{
	if(Foundation_Tools::IsValid(OnKey))
	{
		KeyEvent = OnKey;
	}
}

/**********************************************************************************************//**
 * @fn	void FWindow::SetOnMouseButtonEvent(OnMouseButtonEvent OnMouseButtonEvent)
 *
 * @brief	Sets on mouse button event.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	OnMouseButtonEvent	The on mouse button event.
 **************************************************************************************************/

void FWindow::SetOnMouseButtonEvent(OnMouseButtonEvent OnMouseButtonEvent)
{
	if(Foundation_Tools::IsValid(OnMouseButtonEvent))
	{
		MouseButtonEvent = OnMouseButtonEvent;
	}
}

/**********************************************************************************************//**
 * @fn	void FWindow::SetOnMouseWheelEvent(OnMouseWheelEvent OnMouseWheel)
 *
 * @brief	Sets on mouse wheel event.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	OnMouseWheel	The on mouse wheel event.
 **************************************************************************************************/

void FWindow::SetOnMouseWheelEvent(OnMouseWheelEvent OnMouseWheel)
{
	if(Foundation_Tools::IsValid(OnMouseWheel))
	{
		MouseWheelEvent = OnMouseWheel;
	}
}

/**********************************************************************************************//**
 * @fn	void FWindow::SetOnDestroyed(OnDestroyedEvent OnDestroyed)
 *
 * @brief	Sets on destroyed.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	OnDestroyed	The on destroyed event.
 **************************************************************************************************/

void FWindow::SetOnDestroyed(OnDestroyedEvent OnDestroyed)
{
	if(Foundation_Tools::IsValid(OnDestroyed))
	{
		DestroyedEvent = OnDestroyed;
	}
}

/**********************************************************************************************//**
 * @fn	void FWindow::SetOnMaximized(OnMaximizedEvent OnMaximized)
 *
 * @brief	Sets on maximized.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	OnMaximized	The on maximized event.
 **************************************************************************************************/

void FWindow::SetOnMaximized(OnMaximizedEvent OnMaximized)
{
	if(Foundation_Tools::IsValid(OnMaximized))
	{
		MaximizedEvent = OnMaximized;
	}
}

/**********************************************************************************************//**
 * @fn	void FWindow::SetOnMinimized(OnMinimizedEvent OnMinimized)
 *
 * @brief	Sets on minimized.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	OnMinimized	The on minimized event.
 **************************************************************************************************/

void FWindow::SetOnMinimized(OnMinimizedEvent OnMinimized)
{
	if(Foundation_Tools::IsValid(OnMinimized))
	{
		MinimizedEvent = OnMinimized;
	}
}

/*void FWindow::SetOnRestored(OnRestoredEvent OnRestored)
{
	if (Foundation_Tools::IsValid(OnRestored))
	{
		RestoredEvent = OnRestored;
	}
}*/

/**********************************************************************************************//**
 * @fn	void FWindow::SetOnFocus(OnFocusEvent OnFocus)
 *
 * @brief	Sets on focus.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	OnFocus	The on focus event.
 **************************************************************************************************/

void FWindow::SetOnFocus(OnFocusEvent OnFocus)
{
	if(Foundation_Tools::IsValid(OnFocus))
	{
		FocusEvent = OnFocus;
	}
}

/**********************************************************************************************//**
 * @fn	void FWindow::SetOnMoved(OnMovedEvent OnMoved)
 *
 * @brief	Sets on moved.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	OnMoved	The on moved event.
 **************************************************************************************************/

void FWindow::SetOnMoved(OnMovedEvent OnMoved)
{
	if(Foundation_Tools::IsValid(OnMoved))
	{
		MovedEvent = OnMoved;
	}
}

/**********************************************************************************************//**
 * @fn	void FWindow::SetOnResize(OnResizeEvent OnResize)
 *
 * @brief	Sets on resize.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	OnResize	The on resize event.
 **************************************************************************************************/

void FWindow::SetOnResize(OnResizeEvent OnResize)
{
	if(Foundation_Tools::IsValid(OnResize))
	{
		ResizeEvent = OnResize;
	}
}

/**********************************************************************************************//**
 * @fn	void FWindow::SetOnMouseMove(OnMouseMoveEvent OnMouseMove)
 *
 * @brief	Sets on mouse move.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	OnMouseMove	The on mouse move event.
 **************************************************************************************************/

void FWindow::SetOnMouseMove(OnMouseMoveEvent OnMouseMove)
{
	if(Foundation_Tools::IsValid(OnMouseMove))
	{
		MouseMoveEvent = OnMouseMove;
	}
}
