/**********************************************************************************************//**
 * @file	WindowAPI\Window.cpp
 *
 * @brief	Implements the FWindow class.
 **************************************************************************************************/

#include <limits.h>
#include "Window.h"
#include "WindowManager.h"

#if defined(__linux__)
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

	if(!IsValidString(WindowName))
	{
		PrintErrorMessage(ERROR_INVALIDWINDOWNAME);
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
 * @fn	GLboolean FWindow::Shutdown()
 *
 * @brief	Shuts down this object and frees any resources it is using.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	A GLboolean.
 **************************************************************************************************/

GLboolean FWindow::Shutdown()
{
	if(ContextCreated)
	{
		
#if defined (_WIN32) || defined(_WIN64)
		Windows_Shutdown();
#else
		Linux_Shutdown();
#endif
		ContextCreated = GL_FALSE;
		return FOUNDATION_OKAY;
	}

		PrintErrorMessage(ERROR_NOCONTEXT);
		return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::Initialize()
 *
 * @brief	Initializes this object.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	A GLboolean.
 **************************************************************************************************/

GLboolean FWindow::Initialize()
{
#if defined(_WIN32) || defined(_WIN64)
	return Windows_Initialize();
#else
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
 * @fn	GLboolean FWindow::InitializeGL()
 *
 * @brief	Initializes OpenGL for this window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	A GLboolean.
 **************************************************************************************************/

GLboolean FWindow::InitializeGL()
{
#if defined(_WIN32) || (_WIN64)
	return Windows_InitializeGL();
#else
	return Linux_InitializeGL();
#endif
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::SwapDrawBuffers()
 *
 * @brief	Swap draw buffers.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	A GLboolean.
 **************************************************************************************************/

GLboolean FWindow::SwapDrawBuffers()
{
	if(ContextCreated)
	{
#if defined(_WIN32) || (_WIN64)
		SwapBuffers(DeviceContextHandle);
#else
		glXSwapBuffers(WindowManager::GetDisplay(), WindowHandle);
#endif
		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_NOCONTEXT);
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::SetSwapInterval(GLint SwapSetting)
 *
 * @brief	Sets swap interval(V-sync).
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	SwapSetting	The swap setting.
 *
 * @return	A GLboolean.
 **************************************************************************************************/

GLboolean FWindow::SetSwapInterval(GLint SwapSetting)
{
	if(ContextCreated)
	{
	CurrentSwapInterval = SwapSetting;
#if defined(_WIN32) || defined(_WIN64)
	Windows_VerticalSync(SwapSetting);
#else
	Linux_VerticalSync(SwapSetting);
#endif

	return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_NOCONTEXT);
	return FOUNDATION_ERROR;
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
 * @fn	GLboolean FWindow::SetCurrentState(GLuint NewState)
 *
 * @brief	Sets the current state of the window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	NewState	new state of the window.
 *
 * @return	A GLboolean.
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

	PrintErrorMessage(ERROR_NOCONTEXT);
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::GetIsFullScreen()
 *
 * @brief	returns whether the window is in full screen mode.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	Whether the window is currently in full screen mode.
 **************************************************************************************************/

GLboolean FWindow::GetIsFullScreen()
{
	if(ContextCreated)
	{
		return (CurrentState == WINDOWSTATE_FULLSCREEN);
	}

	PrintErrorMessage(ERROR_NOCONTEXT);
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::FullScreen(GLboolean ShouldBeFullscreen)
 *
 * @brief	toggle the full screen mode for the window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	ShouldBeFullscreen	whether the window should be in full screen mode.
 *
 * @return	A GLboolean.
 **************************************************************************************************/

GLboolean FWindow::FullScreen(GLboolean ShouldBeFullscreen)
{
	if(ContextCreated)
	{
		if (ShouldBeFullscreen)
		{
			CurrentState = WINDOWSTATE_FULLSCREEN;
		}

		else
		{
			CurrentState = WINDOWSTATE_NORMAL;
		}

#if defined(_WIN32) || defined(_WIN64)
		Windows_FullScreen();
#else
		Linux_FullScreen(ShouldBeFullscreen);
#endif

		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_NOCONTEXT);
	return FOUNDATION_OKAY;
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
 * @fn	GLboolean FWindow::Minimize(GLboolean NewState)
 *
 * @brief	set the window to be minimized depending on NewState.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	NewState	whether the window should be minimized.
 *
 * @return	A GLboolean.
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

#if defined(_WIN32) || defined(_WIN64)
	 	Windows_Minimize();
#else
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
 * @fn	GLboolean FWindow::Maximize(GLboolean NewState)
 *
 * @brief	Maximizes the window depending on New state.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	NewState	Whether to minimize the window.
 *
 * @return	A GLboolean.
 **************************************************************************************************/

GLboolean FWindow::Maximize(GLboolean NewState)
{
	if(ContextCreated)
	{
		if(NewState)
		{
			CurrentState = WINDOWSTATE_MAXIMIZED;
		}

		else
		{
			CurrentState = WINDOWSTATE_NORMAL;
		}

#if defined(_WIN32) || defined(_WIN64)
		 Windows_Maximize();
#else
	 	Linux_Maximize(NewState);
#endif
		 return FOUNDATION_OKAY;
	}
	PrintErrorMessage(ERROR_NOCONTEXT);	
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::Restore()
 *
 * @brief	Restores the window to its default setting.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	A GLboolean.
 **************************************************************************************************/

GLboolean FWindow::Restore()
{
	if (ContextCreated)
	{
		switch (CurrentState)
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
#if defined(_WIN32) || defined(_WIN64)
		Windows_Restore();
#else
		Linux_Restore();
#endif

		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_NOCONTEXT);
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::GetResolution(GLuint& Width, GLuint& Height)
 *
 * @brief	Gets the resolution of the window by setting width and height.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param [in,out]	Width 	The width.
 * @param [in,out]	Height	The height.
 *
 * @return	The resolution.
 **************************************************************************************************/

GLboolean FWindow::GetResolution(GLuint& Width, GLuint& Height)
{
	if (ContextCreated)
	{
		Width = Resolution[0];
		Height = Resolution[1];
		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_NOCONTEXT);
	return FOUNDATION_ERROR;
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
	if (ContextCreated)
	{
		if(Width > 0 && Height > 0)
		{
			Resolution[0] = Width;
			Resolution[1] = Height;

#if defined(_WIN32) || defined(_WIN64)
			Windows_SetResolution(Resolution[0], Resolution[1]);
#else
			Linux_SetResolution(Width, Height);	
#endif
			glViewport(0, 0, Resolution[0], Resolution[1]);

			return FOUNDATION_OKAY;
		}

		else
		{
			PrintErrorMessage(ERROR_INVALIDRESOLUTION);
			return FOUNDATION_ERROR;
		}
	}  

		PrintErrorMessage(ERROR_NOCONTEXT);
		return FOUNDATION_ERROR;
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
	if (ContextCreated)
	{
		X = MousePosition[0];
		Y = MousePosition[1];   /**< . */
		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_NOCONTEXT);
	return FOUNDATION_ERROR;
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
	if (ContextCreated)
	{
		return MousePosition;
	}

	PrintErrorMessage(ERROR_NOCONTEXT);
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
	if (ContextCreated)
	{ 
		MousePosition[0] = X;
		MousePosition[1] = Y;
#if defined(_WIN32) || defined(_WIN64)
		Windows_SetMousePosition(X, Y);
#else
		Linux_SetMousePosition(X, Y);
#endif
		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_NOCONTEXT);
	return FOUNDATION_ERROR;
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
	if (ContextCreated)
	{
		X = Position[0];
		Y = Position[1];

		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_NOCONTEXT);
	return FOUNDATION_ERROR;
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
	if (ContextCreated)
	{
		Position[0] = X;
		Position[1] = Y;
#if defined(_WIN32) || defined(_WIN64)
		Windows_SetPosition(Position[0], Position[1]);
#else
		Linux_SetPosition(X, Y);
#endif
	}

	PrintErrorMessage(ERROR_NOCONTEXT);
	return FOUNDATION_ERROR;
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
	if (ContextCreated)
	{
		return Name;
	}

	PrintErrorMessage(ERROR_NOCONTEXT);
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
	if (ContextCreated)
	{
		if(NewTitle != nullptr)
		{
#if defined(_WIN32) || defined(_WIN64)
			Windows_SetTitleBar(NewTitle);
#else
			Linux_SetTitleBar(NewTitle);
#endif
			return FOUNDATION_OKAY;
		}

		else
		{
			PrintErrorMessage(ERROR_INVALIDTITLEBAR);
			return FOUNDATION_ERROR;
		}
	}

	PrintErrorMessage(ERROR_NOCONTEXT);
	return FOUNDATION_ERROR;
}

GLboolean FWindow::SetIcon(const char* Icon, GLuint Width, GLuint Height)
{
	if (ContextCreated)
	{
#if defined(_WIN32) || defined(_WIN64)
		Windows_SetIcon(Icon, Width, Height);
#else
		Linux_SetIcon(Icon, Width, Height);
#endif
		return FOUNDATION_OKAY;
	}
	return FOUNDATION_ERROR;
}

GLboolean FWindow::SetStyle(GLuint WindowType)
{
	if (ContextCreated)
	{
#if defined(_WIN32) || defined(_WIN64)
		Windows_SetStyle(WindowType);
#else
		Linux_SetStyle(WindowType);
#endif
		PrintErrorMessage(ERROR_NOCONTEXT);
		return FOUNDATION_OKAY;
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::MakeCurrentContext()
 *
 * @brief	Makes the window be the current OpenGL context to be drawn to. NOTE: Does not change
 *  the IsCurrentContext variable foe other windows.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	A GLboolean.
 **************************************************************************************************/

GLboolean FWindow::MakeCurrentContext()
{
	if(ContextCreated)
	{
		IsCurrentContext = true;
#if defined(_WIN32) || defined(_WIN64)
		wglMakeCurrent(DeviceContextHandle, GLRenderingContextHandle);
#else
		glXMakeCurrent(WindowManager::GetDisplay(), WindowHandle, Context);
#endif
		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_NOCONTEXT);
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::GetIsCurrentContext()
 *
 * @brief	Gets is current context.
 *
 * @author	Ziyad
 * @date	3/01/2015
 *
 * @return	The is current context.
 **************************************************************************************************/

GLboolean FWindow::GetIsCurrentContext()
{
	if(ContextCreated)
	{
		return IsCurrentContext;
	}
	PrintErrorMessage(ERROR_NOCONTEXT);
	return GL_FALSE;
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::GetContextHasBeenCreated()
 *
 * @brief	Gets context has been created.
 *
 * @author	Ziyad
 * @date	3/01/2015
 *
 * @return	The context has been created.
 **************************************************************************************************/

GLboolean FWindow::GetContextHasBeenCreated()
{
	return ContextCreated;
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
#if defined(_WIN32) || defined(_WIN64)
	Windows_InitGLExtensions();
#else
	Linux_InitGLExtensions();
#endif
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::PrintOpenGLVersion()
 *
 * @brief	Print open gl version.
 *
 * @author	Ziyad
 * @date	3/01/2015
 *
 * @return	A GLboolean.
 **************************************************************************************************/

GLboolean FWindow::PrintOpenGLVersion()
{
	if(ContextCreated)
	{
		printf("%s\n", glGetString(GL_VERSION));
		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_NOCONTEXT);
	return FOUNDATION_ERROR;
}

const char* FWindow::GetOpenGLVersion()
{
	if(ContextCreated)
	{
		return (const char*)glGetString(GL_VERSION);
	}
	PrintErrorMessage(ERROR_NOCONTEXT);
	return nullptr;
}

GLboolean FWindow::PrintOpenGLExtensions()
{
	if(ContextCreated)
	{
		printf("%s \n", (const char*)glGetString(GL_EXTENSIONS));
		return FOUNDATION_OKAY;
	}

		PrintErrorMessage(ERROR_NOCONTEXT);
		return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	const char* FWindow::GetOpenGLExtensions()
 *
 * @brief	Gets open gl extensions.
 *
 * @author	Ziyad
 * @date	3/01/2015
 *
 * @return	null if it fails, else the open gl extensions.
 **************************************************************************************************/

const char* FWindow::GetOpenGLExtensions()
{
	if(ContextCreated)
	{
		return (const char*)glGetString(GL_EXTENSIONS);
	}

	else
	{
		PrintErrorMessage(ERROR_NOCONTEXT);
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
	if (ContextCreated)
	{
		InFocus = ShouldBeInFocus;

#if defined(_WIN32) || defined(_WIN64)
		Windows_Focus();	
#else
		Linux_Focus(ShouldBeInFocus);
#endif
		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_NOCONTEXT);
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::SetOnKeyEvent(OnKeyEvent OnKey)
 *
 * @brief	Sets on key event.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	OnKey	The on key event.
 *
 * @return	A GLboolean.
 **************************************************************************************************/

GLboolean FWindow::SetOnKeyEvent(OnKeyEvent OnKey)
{
	if (IsValidKeyEvent(OnKey))
	{
		KeyEvent = OnKey;
		return FOUNDATION_OKAY;
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::SetOnMouseButtonEvent(OnMouseButtonEvent OnMouseButtonEvent)
 *
 * @brief	Sets on mouse button event.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	OnMouseButtonEvent	The on mouse button event.
 *
 * @return	A GLboolean.
 **************************************************************************************************/

GLboolean FWindow::SetOnMouseButtonEvent(OnMouseButtonEvent OnMouseButtonEvent)
{
	//we don't really need to check if the context has been created
	if(IsValidKeyEvent(OnMouseButtonEvent))
	{
		MouseButtonEvent = OnMouseButtonEvent;
		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_INVALIDEVENT);
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::SetOnMouseWheelEvent(OnMouseWheelEvent OnMouseWheel)
 *
 * @brief	Sets on mouse wheel event.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	OnMouseWheel	The on mouse wheel event.
 *
 * @return	A GLboolean.
 **************************************************************************************************/

GLboolean FWindow::SetOnMouseWheelEvent(OnMouseWheelEvent OnMouseWheel)
{
	if(IsValidMouseWheelEvent(OnMouseWheel))
	{
		MouseWheelEvent = OnMouseWheel;
		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_INVALIDEVENT);
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::SetOnDestroyed(OnDestroyedEvent OnDestroyed)
 *
 * @brief	Sets on destroyed.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	OnDestroyed	The on destroyed event.
 *
 * @return	A GLboolean.
 **************************************************************************************************/

GLboolean FWindow::SetOnDestroyed(OnDestroyedEvent OnDestroyed)
{
	if(IsValidDestroyedEvent(OnDestroyed))
	{
		DestroyedEvent = OnDestroyed;
		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_INVALIDEVENT);
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::SetOnMaximized(OnMaximizedEvent OnMaximized)
 *
 * @brief	Sets on maximized.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	OnMaximized	The on maximized event.
 *
 * @return	A GLboolean.
 **************************************************************************************************/

GLboolean FWindow::SetOnMaximized(OnMaximizedEvent OnMaximized)
{
	if(IsValidDestroyedEvent(OnMaximized))
	{
		MaximizedEvent = OnMaximized;
		return FOUNDATION_OKAY;
	}
	PrintErrorMessage(ERROR_INVALIDEVENT);
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::SetOnMinimized(OnMinimizedEvent OnMinimized)
 *
 * @brief	Sets on minimized.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	OnMinimized	The on minimized event.
 *
 * @return	A GLboolean.
 **************************************************************************************************/

GLboolean FWindow::SetOnMinimized(OnMinimizedEvent OnMinimized)
{
	if(IsValidDestroyedEvent(OnMinimized))
	{
		MinimizedEvent = OnMinimized;
		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_INVALIDEVENT);
	return FOUNDATION_ERROR;
}

/*void FWindow::SetOnRestored(OnRestoredEvent OnRestored)
{
	if (IsValid(OnRestored))
	{
		RestoredEvent = OnRestored;
	}
}*/

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::SetOnFocus(OnFocusEvent OnFocus)
 *
 * @brief	Sets on focus.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	OnFocus	The on focus event.
 *
 * @return	A GLboolean.
 **************************************************************************************************/

GLboolean FWindow::SetOnFocus(OnFocusEvent OnFocus)
{
	if(IsValidFocusEvent(OnFocus))
	{
		FocusEvent = OnFocus;
		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_INVALIDEVENT);
	return FOUNDATION_ERROR;
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

GLboolean FWindow::SetOnMoved(OnMovedEvent OnMoved)
{
	if(IsValidMovedEvent(OnMoved))
	{
		MovedEvent = OnMoved;
		return FOUNDATION_OKAY;
	}
	PrintErrorMessage(ERROR_INVALIDEVENT);
	return FOUNDATION_ERROR;
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

GLboolean FWindow::SetOnResize(OnResizeEvent OnResize)
{
	if(IsValidMovedEvent(OnResize))
	{
		ResizeEvent = OnResize;
		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_INVALIDEVENT);
	return FOUNDATION_ERROR;
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

GLboolean FWindow::SetOnMouseMove(OnMouseMoveEvent OnMouseMove)
{
	if(IsValidMouseMoveEvent(OnMouseMove))
	{
		MouseMoveEvent = OnMouseMove;
		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_INVALIDEVENT);
	return FOUNDATION_ERROR;
}

GLboolean FWindow::EnableDecorator(GLbitfield Decorator)
{
	if (ContextCreated)
	{
#if defined(_WIN32) || defined(_WIN64)
		Windows_EnableDecorator(Decorator);
#else
		Linux_EnableDecorator(Decorator);
#endif

		return FOUNDATION_OKAY;
	}
	PrintErrorMessage(ERROR_NOCONTEXT);
	return FOUNDATION_ERROR;
}

GLboolean FWindow::DisableDecorator(GLbitfield Decorator)
{
	if (ContextCreated)
	{
#if defined(_WIN32) || defined(_WIN64)
		Windows_DisableDecorator(Decorator);
#else
		Linux_DisableDecorator(Decorator);
#endif
		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_NOCONTEXT);
	return FOUNDATION_ERROR;
}
