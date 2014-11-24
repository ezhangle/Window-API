#include <limits.h>
#include "Window.h"
#include "WindowManager.h"
#include "Tools.h"

#if defined(CURRENT_OS_LINUX)
#include <cstring>
#endif

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

#if defined(CURRENT_OS_LINUX)
	//Linux_Initialize();	
#endif
}

FWindow::~FWindow()
{
	Shutdown();
}

void FWindow::Shutdown()
{
#if defined (CURRENT_OS_WINDOWS)
	Windows_Shutdown();
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_Shutdown();
#endif
}

void FWindow::Initialize()
{
#if defined(CURRENT_OS_WINDOWS)
	Windows_Initialize(Name);
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_Initialize();
#endif
}

GLboolean FWindow::GetShouldClose()
{
	return ShouldClose;
}

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

GLboolean FWindow::GetKeyState(GLuint Key)
{
	return Keys[Key];
}

void FWindow::InitializeGL()
{
#if defined(CURRENT_OS_WINDOWS)
	Windows_InitializeGL();
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_InitializeGL();
#endif
}

void FWindow::SwapDrawBuffers()
{
#if defined(CURRENT_OS_WINDOWS)
	SwapBuffers(DeviceContextHandle);
#endif

#if defined(CURRENT_OS_LINUX)
	glXSwapBuffers(WindowManager::GetDisplay(), WindowHandle);
#endif
}

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

GLuint FWindow::GetCurrentState()
{
	return CurrentState;
}

void FWindow::SetCurrentState(GLuint NewState)
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

GLboolean FWindow::GetIsFullScreen()
{
	return (CurrentState == WINDOWSTATE_FULLSCREEN);
}

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

GLboolean FWindow::GetIsMinimized()
{
	return (CurrentState == WINDOWSTATE_MINIMIZED);
}

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

GLboolean FWindow::GetIsMaximized()
{
	return (CurrentState == WINDOWSTATE_MAXIMIZED) ;
}

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

void FWindow::GetResolution(GLuint& Width, GLuint& Height)
{
	Width = Resolution[0];
	Height = Resolution[1];
}

GLuint* FWindow::GetResolution()
{
	return Resolution;
}

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

void FWindow::GetMousePosition(GLuint& X, GLuint& Y)
{
	X = MousePosition[0];
	Y = MousePosition[1];
}

GLuint* FWindow::GetMousePosition()
{
	return MousePosition;
}

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

void FWindow::GetPosition(GLuint& X, GLuint& Y)
{
	X = Position[0];
	Y = Position[1];
}

GLuint* FWindow::GetPosition()
{
	return Position;
}

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

const char* FWindow::GetWindowName()
{
	return Name;
}

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

void FWindow::MakeCurrentContext()
{
#if defined(CURRENT_OS_WINDOWS)
	wglMakeCurrent(DeviceContextHandle, GLRenderingContextHandle);
#endif

#if defined(CURRENT_OS_LINUX)
	glXMakeCurrent(WindowManager::GetDisplay(), WindowHandle, Context);
#endif
}

void FWindow::InitGLExtensions()
{
#if defined(CURRENT_OS_WINDOWS)
	Windows_InitGLExtensions();
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_InitGLExtensions();
#endif

}

GLboolean FWindow::GetInFocus()
{
	return InFocus;
}

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

void FWindow::SetOnKeyEvent(OnKeyEvent a_OnKeyPressed)
{
	if(Foundation_Tools::IsValid(a_OnKeyPressed))
	{
		KeyEvent = a_OnKeyPressed;
	}
}

void FWindow::SetOnMouseButtonEvent(OnMouseButtonEvent a_OnMouseButtonEvent)
{
	if(Foundation_Tools::IsValid(a_OnMouseButtonEvent))
	{
		MouseButtonEvent = a_OnMouseButtonEvent;
	}
}

void FWindow::SetOnMouseWheelEvent(OnMouseWheelEvent OnMouseWheelEvent)
{
	if(Foundation_Tools::IsValid(OnMouseWheelEvent))
	{
		MouseWheelEvent = OnMouseWheelEvent;
	}
}

void FWindow::SetOnDestroyed(OnDestroyedEvent OnDestroyed)
{
	if(Foundation_Tools::IsValid(OnDestroyed))
	{
		DestroyedEvent = OnDestroyed;
	}
}

void FWindow::SetOnMaximized(OnMaximizedEvent OnMaximized)
{
	if(Foundation_Tools::IsValid(OnMaximized))
	{
		MaximizedEvent = OnMaximized;
	}
}

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

void FWindow::SetOnFocus(OnFocusEvent OnFocus)
{
	if(Foundation_Tools::IsValid(OnFocus))
	{
		FocusEvent = OnFocus;
	}
}

void FWindow::SetOnMoved(OnMovedEvent OnMoved)
{
	if(Foundation_Tools::IsValid(OnMoved))
	{
		MovedEvent = OnMoved;
	}
}

void FWindow::SetOnResize(OnResizeEvent OnResize)
{
	if(Foundation_Tools::IsValid(OnResize))
	{
		ResizeEvent = OnResize;
	}
}

void FWindow::SetOnMouseMove(OnMouseMoveEvent OnMouseMove)
{
	if(Foundation_Tools::IsValid(OnMouseMove))
	{
		MouseMoveEvent = OnMouseMove;
	}
}
