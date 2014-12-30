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
		Foundation_Tools::PrintErrorMessage(ERROR_INVALIDWINDOWNAME);
		exit(0);
	}

	InitializeEvents();

	CurrentState = WINDOWSTATE_NORMAL;
	ContextCreated = GL_FALSE;
	IsCurrentContext = GL_FALSE;
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

GLboolean FWindow::Shutdown()
{
	if(ContextCreated)
	{
		ContextCreated = GL_FALSE;
#if defined (CURRENT_OS_WINDOWS)
		Windows_Shutdown();
#endif

#if defined(CURRENT_OS_LINUX)
		Linux_Shutdown();
#endif

		return FOUNDATION_OKAY;
	}

	return 	Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
}

/**********************************************************************************************//**
 * @fn	void FWindow::Initialize()
 *
 * @brief	Initializes this object.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

GLboolean FWindow::Initialize()
{
#if defined(CURRENT_OS_WINDOWS)
	return Windows_Initialize(Name);
#endif

#if defined(CURRENT_OS_LINUX)
	return Linux_Initialize();
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

GLboolean FWindow::InitializeGL()
{
#if defined(CURRENT_OS_WINDOWS)
	return Windows_InitializeGL();
#endif

#if defined(CURRENT_OS_LINUX)
	return Linux_InitializeGL();
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

GLboolean FWindow::SwapDrawBuffers()
{
	if(ContextCreated)
	{
#if defined(CURRENT_OS_WINDOWS)
		SwapBuffers(DeviceContextHandle);
#endif

#if defined(CURRENT_OS_LINUX)
		glXSwapBuffers(WindowManager::GetDisplay(), WindowHandle);
#endif

		return FOUNDATION_OKAY;
	}

	return Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
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

GLboolean FWindow::SetSwapInterval(GLint SwapSetting)
{
	if(ContextCreated)
	{
	CurrentSwapInterval = SwapSetting;
#if defined(CURRENT_OS_WINDOWS)
	Windows_VerticalSync(SwapSetting);
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_VerticalSync(SwapSetting);
#endif

	return FOUNDATION_OKAY;
	}

	return Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
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

GLboolean FWindow::SetCurrentState(GLuint NewState)
{
	/**
	* first we restore the window to make moving from state to state as easy as possible
	*/

	if(ContextCreated)
	{

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

	return Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
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
	if(ContextCreated)
	{
		return (CurrentState == WINDOWSTATE_FULLSCREEN);
	}

	return Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
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

GLboolean FWindow::FullScreen(GLboolean ShouldBeFullscreen)
{
	if(ContextCreated)
	{
	if(ShouldBeFullscreen)
	{
		CurrentState = WINDOWSTATE_FULLSCREEN;
	}

	else
	{
		CurrentState = WINDOWSTATE_NORMAL;
	}

#if defined(CURRENT_OS_LINUX)
	Linux_FullScreen(ShouldBeFullscreen);
#endif

#if defined(CURRENT_OS_WINDOWS)
	Windows_Fullscreen();
#endif

	return FOUNDATION_OKAY;
	}

	return Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
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

GLboolean FWindow::Minimize(GLboolean NewState)
{
	if(ContextCreated)
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
	 	Windows_Minimize();
#endif

#if defined(CURRENT_OS_LINUX)
	 	Linux_Minimize(NewState);
#endif

	 	return FOUNDATION_OKAY;
	}	

	return FOUNDATION_ERROR;
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

GLboolean FWindow::Maximize(GLboolean NewState)
{
	if(Context)
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
		 Windows_Maximize();
#endif

#if defined(CURRENT_OS_LINUX)
	 	Linux_Maximize(NewState);
#endif
		 return FOUNDATION_OKAY;
	}
	return Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);	
}

/**********************************************************************************************//**
 * @fn	void FWindow::Restore()
 *
 * @brief	Restores the window to its default setting.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

GLboolean FWindow::Restore()
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

GLboolean FWindow::GetResolution(GLuint& Width, GLuint& Height)
{
	if(Context)
	{
		Width = Resolution[0];
		Height = Resolution[1];
		return FOUNDATION_OKAY;
	}

	return Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
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

GLboolean FWindow::SetResolution(GLuint Width, GLuint Height)
{
	if(Context)
	{
		if(Width > 0 && Height > 0)
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

		else
		{
			return Foundation_Tools::PrintErrorMessage(ERROR_INVALIDRESOLUTION);
		}
	}

		return Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
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

GLboolean FWindow::GetMousePosition(GLuint& X, GLuint& Y)
{
	if(Context)
	{
		X = MousePosition[0];
		Y = MousePosition[1];
		return FOUNDATION_OKAY;
	}

	return Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
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
	if(Context)
	{
		return MousePosition;
	}

	Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
	return nullptr;
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

GLboolean FWindow::SetMousePosition(GLuint X, GLuint Y)
{
	if(Context)
	{ 
		MousePosition[0] = X;
		MousePosition[1] = Y;
#if defined(CURRENT_OS_WINDOWS)
		Windows_SetMousePosition(X, Y);
#endif

#if defined(CURRENT_OS_LINUX)
		Linux_SetMousePosition(X, Y);
#endif

		return FOUNDATION_OKAY;
	}

	return Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);


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

GLboolean FWindow::GetPosition(GLuint& X, GLuint& Y)
{
	if(Context)
	{
		X = Position[0];
		Y = Position[1];

		return FOUNDATION_OKAY;
	}

	return Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
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

GLboolean FWindow::SetPosition(GLuint X, GLuint Y)
{
	if(Context)
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

	return Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
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
	if(Context)
	{
		return Name;
	}

	Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
	return nullptr;
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

GLboolean FWindow::SetTitleBar(const char* NewTitle)
{
	if(Context)
	{
		if(NewTitle != nullptr)
		{
#if defined(CURRENT_OS_LINUX)
			Linux_SetTitleBar(NewTitle);
#endif

#if defined(CURRENT_OS_WINDOWS)
			Windows_SetTitleBar(NewTitle);
#endif
			return FOUNDATION_OKAY;
		}

		else
		{
			Foundation_Tools::PrintErrorMessage(ERROR_INVALIDTITLEBAR);
		}
	}

	return Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
}

/**********************************************************************************************//**
 * @fn	void FWindow::MakeCurrentContext()
 *
 * @brief	Makes the window be the current OpenGL context to be drawn to. NOTE: Does not change the 
 * IsCurrentContext variable foe other windows.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

GLboolean FWindow::MakeCurrentContext()
{
	if(ContextCreated)
	{
		IsCurrentContext = true;
#if defined(CURRENT_OS_WINDOWS)
		wglMakeCurrent(DeviceContextHandle, GLRenderingContextHandle);
#endif

#if defined(CURRENT_OS_LINUX)
		glXMakeCurrent(WindowManager::GetDisplay(), WindowHandle, Context);
#endif
		return FOUNDATION_OKAY;
	}

	return Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
}

GLboolean FWindow::GetIsCurrentContext()
{
	if(ContextCreated)
	{
		return IsCurrentContext;
	}
	Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
	return GL_FALSE;
}

GLboolean FWindow::GetContextHasBeenCreated()
{
	return (Context != GL_FALSE);
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

GLboolean FWindow::PrintOpenGLVersion()
{
	if(ContextCreated)
	{
		printf("%s\n", glGetString(GL_VERSION));
		return FOUNDATION_OKAY;
	}


	return Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
}

const char* FWindow::GetOpenGLVersion()
{
	if(ContextCreated)
	{
		return (const char*)glGetString(GL_VERSION);
	}
	Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
	return nullptr;
}

GLboolean FWindow::PrintOpenGLExtensions()
{
	if(ContextCreated)
	{
		printf("%s \n", (const char*)glGetString(GL_EXTENSIONS));
		return FOUNDATION_OKAY;
	}

		return Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
}

const char* FWindow::GetOpenGLExtensions()
{
	if(ContextCreated)
	{
		return (const char*)glGetString(GL_EXTENSIONS);
	}

	else
	{
		Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
		return nullptr;
	}
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

GLboolean FWindow::Focus(GLboolean ShouldBeInFocus)
{
	if(Context)
	{
		InFocus = ShouldBeInFocus;

#if defined(CURRENT_OS_LINUX)
		Linux_Focus(ShouldBeInFocus);	
#endif

#if defined(CURRENT_OS_WINDOWS)
		Windows_Focus();
#endif

		return FOUNDATION_OKAY;
	}

	return Foundation_Tools::PrintErrorMessage(ERROR_NOCONTEXT);
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
