#include <limits.h>
#include "Window.h"
#include "WindowManager.h"
#include "Tools.h"

#if defined(CURRENT_OS_LINUX)
#include <cstring>
#endif

Window::Window(const char*  WindowName,
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

Window::~Window()
{
	Shutdown();
}

void Window::Shutdown()
{
#if defined (CURRENT_OS_WINDOWS)
	Windows_Shutdown();
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_Shutdown();
#endif
}

void Window::Initialize()
{
#if defined(CURRENT_OS_WINDOWS)
	Windows_Initialize(Name);
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_Initialize();
#endif
}

GLboolean Window::GetShouldClose()
{
	return ShouldClose;
}

void Window::InitializeEvents()
{
	KeyEvent = nullptr;
	MouseButtonEvent = nullptr;
	MouseWheelEvent = nullptr;
	DestroyedEvent = nullptr;
	MaximizedEvent = nullptr;
	MinimizedEvent = nullptr;
	RestoredEvent = nullptr;
	MovedEvent = nullptr;
	MouseMoveEvent = nullptr;
}

GLboolean Window::GetKeyState(GLuint Key)
{
	return Keys[Key];
}

void Window::InitializeGL()
{
#if defined(CURRENT_OS_WINDOWS)
	Windows_InitializeGL();
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_InitializeGL();
#endif
}

void Window::SwapDrawBuffers()
{
#if defined(CURRENT_OS_WINDOWS)
	SwapBuffers(DeviceContextHandle);
#endif

#if defined(CURRENT_OS_LINUX)
	glXSwapBuffers(WindowManager::GetDisplay(), WindowHandle);
#endif
}

void Window::SetSwapInterval(GLint SwapSetting)
{
	CurrentSwapInterval = SwapSetting;
#if defined(CURRENT_OS_WINDOWS)
	Windows_VerticalSync(SwapSetting);
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_VerticalSync(SwapSetting);
#endif
}

GLuint Window::GetCurrentState()
{
	return CurrentState;
}

void Window::SetCurrentState(GLuint NewState)
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

GLboolean Window::GetIsFullScreen()
{
	return (CurrentState == WINDOWSTATE_FULLSCREEN);
}

void Window::FullScreen(GLboolean NewState)
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

GLboolean Window::GetIsMinimized()
{
	return (CurrentState == WINDOWSTATE_MINIMIZED);
}

void Window::Minimize(GLboolean NewState)
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

GLboolean Window::GetIsMaximized()
{
	return (CurrentState == WINDOWSTATE_MAXIMIZED) ;
}

void Window::Maximize(GLboolean NewState)
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

void Window::Restore()
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

void Window::GetResolution(GLuint& Width, GLuint& Height)
{
	Width = Resolution[0];
	Height = Resolution[1];
}

GLuint* Window::GetResolution()
{
	return Resolution;
}

void Window::SetResolution(GLuint Width, GLuint Height)
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

void Window::GetMousePosition(GLuint& X, GLuint& Y)
{
	X = MousePosition[0];
	Y = MousePosition[1];
}

GLuint* Window::GetMousePosition()
{
	return MousePosition;
}

void Window::SetMousePosition(GLuint X, GLuint Y)
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

void Window::GetPosition(GLuint& X, GLuint& Y)
{
	X = Position[0];
	Y = Position[1];
}

GLuint* Window::GetPosition()
{
	return Position;
}

void Window::SetPosition(GLuint X, GLuint Y)
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

const char* Window::GetWindowName()
{
	return Name;
}

void Window::SetName(const char* WindowName)
{
	if(WindowName != nullptr)
	{
		WindowName = WindowName;
#if defined(CURRENT_OS_LINUX)
		Linux_SetName(WindowName);
#endif

#if defined(CURRENT_OS_WINDOWS)
		Windows_SetName(WindowName);
#endif
	}
}

void Window::MakeCurrentContext()
{
#if defined(CURRENT_OS_WINDOWS)
	wglMakeCurrent(DeviceContextHandle, GLRenderingContextHandle);
#endif

#if defined(CURRENT_OS_LINUX)
	glXMakeCurrent(WindowManager::GetDisplay(), WindowHandle, Context);
#endif
}

void Window::InitGLExtensions()
{
#if defined(CURRENT_OS_WINDOWS)
	Windows_InitGLExtensions();
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_InitGLExtensions();
#endif

}

GLboolean Window::GetInFocus()
{
	return InFocus;
}

void Window::Focus(GLboolean NewState)
{
	InFocus = NewState;

#if defined(CURRENT_OS_LINUX)
	Linux_Focus(NewState);	
#endif

#if defined(CURRENT_OS_WINDOWS)
	Windows_Focus(NewState);
#endif
}

void Window::SetOnKeyEvent(OnKeyEvent a_OnKeyPressed)
{
	if(Foundation_Tools::IsValid(a_OnKeyPressed))
	{
		KeyEvent = a_OnKeyPressed;
	}
}

void Window::SetOnMouseButtonEvent(OnMouseButtonEvent a_OnMouseButtonEvent)
{
	if(Foundation_Tools::IsValid(a_OnMouseButtonEvent))
	{
		MouseButtonEvent = a_OnMouseButtonEvent;
	}
}

void Window::SetOnMouseWheelEvent(OnMouseWheelEvent OnMouseWheelEvent)
{
	if(Foundation_Tools::IsValid(OnMouseWheelEvent))
	{
		MouseWheelEvent = OnMouseWheelEvent;
	}
}

void Window::SetOnDestroyed(OnDestroyedEvent OnDestroyed)
{
	if(Foundation_Tools::IsValid(OnDestroyed))
	{
		DestroyedEvent = OnDestroyed;
	}
}

void Window::SetOnMaximized(OnMaximizedEvent OnMaximized)
{
	if(Foundation_Tools::IsValid(OnMaximized))
	{
		MaximizedEvent = OnMaximized;
	}
}

void Window::SetOnMinimized(OnMinimizedEvent OnMinimized)
{
	if(Foundation_Tools::IsValid(OnMinimized))
	{
		MinimizedEvent = OnMinimized;
	}
}

void Window::SetOnRestored(OnRestoredEvent OnRestored)
{
	if (Foundation_Tools::IsValid(OnRestored))
	{
		OnRestored = OnRestored;
	}
}

void Window::SetOnFocus(OnFocusEvent OnFocus)
{
	if(Foundation_Tools::IsValid(OnFocus))
	{
		FocusEvent = OnFocus;
	}
}

void Window::SetOnMoved(OnMovedEvent OnMoved)
{
	if(Foundation_Tools::IsValid(OnMoved))
	{
		MovedEvent = OnMoved;
	}
}

void Window::SetOnResize(OnResizeEvent OnResize)
{
	if(Foundation_Tools::IsValid(OnResize))
	{
		ResizeEvent = OnResize;
	}
}

void Window::SetOnMouseMove(OnMouseMoveEvent OnMouseMove)
{
	if(Foundation_Tools::IsValid(OnMouseMove))
	{
		MouseMoveEvent = OnMouseMove;
	}
}
