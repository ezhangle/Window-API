#include <limits.h>
#include "Window.h"
#include "WindowManager.h"
#include "Window_Tools.h"

#if defined(CURRENT_OS_LINUX)
#include <cstring>
#endif

F_W::F_W(const char*  a_WindowName,
	GLuint a_Width /* = 1280 */,
	GLuint a_Height /* = 720 */,
	GLuint a_ColourBits /* = 32 */, 
	GLuint a_DepthBits /* = 8 */,
	GLuint a_StencilBits /* = 8 */, 
	bool a_ShouldCreateTerminal /* = true */) :
	m_WindowName(a_WindowName),
	m_ColourBits(a_ColourBits),
	m_DepthBits(a_DepthBits),
	m_StencilBits(a_StencilBits)
{
	m_Resolution[0] = a_Width;
	m_Resolution[1] = a_Height;
	m_Position[0] = 0;
	m_Position[1] = 0;
	m_ShouldClose = false;

	if(!Foundation_Tools::IsValid(m_WindowName))
	{
		printf("that was not a valid window name");
		exit(0);
	}

	InitializeEvents();

#if defined(CURRENT_OS_WINDOWS)

#endif

#if defined(CURRENT_OS_LINUX)
	Linux_Initialize();	
#endif
}

F_W::~F_W()
{
	Shutdown();
}

void F_W::Shutdown()
{
#if defined (CURRENT_OS_WINDOWS)
	Windows_Shutdown();
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_Shutdown();
#endif
}

void F_W::Initialize()
{
#if defined(CURRENT_OS_WINDOWS)
	Windows_Initialize(m_WindowName);
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_Initialize();
#endif
}

bool F_W::GetShouldClose()
{
	return m_ShouldClose;
}

void F_W::InitializeEvents()
{
	m_OnKeyEvent = nullptr;
	m_OnMouseButtonEvent = nullptr;
	m_OnMouseWheel = nullptr;
	m_OnDestroyed = nullptr;
	m_OnMaximized = nullptr;
	m_OnMinimized = nullptr;
	m_OnRestored = nullptr;
	m_OnMoved = nullptr;
	m_OnMouseMove = nullptr;
}

bool F_W::GetKeyState(GLuint a_Key)
{
	return m_Keys[a_Key];
}

void F_W::InitializeGL()
{
#if defined(CURRENT_OS_WINDOWS)
	Windows_InitializeGL();
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_InitializeGL();
#endif
}

void F_W::SwapDrawBuffers()
{
#if defined(CURRENT_OS_WINDOWS)
	SwapBuffers(m_DeviceContextHandle);
#endif

#if defined(CURRENT_OS_LINUX)
	glXSwapBuffers(F_WM::GetDisplay(), m_Window);
#endif
}

void F_W::SetVerticalSync(bool a_EnableSync)
{
	m_VerticalSyncEnabled = a_EnableSync;
#if defined(CURRENT_OS_WINDOWS)
	Windows_VerticalSync(a_EnableSync);
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_VerticalSync(a_EnableSync);
#endif
}

bool F_W::GetIsFullScreen()
{
	return m_IsFullScreen;
}

void F_W::FullScreen(bool a_FullScreenState)
{
	m_IsFullScreen = a_FullScreenState;

#if defined(CURRENT_OS_LINUX)
	Linux_FullScreen(m_IsFullScreen);
#endif

#if defined(CURRENT_OS_WINDOWS)
	Windows_FullScreen(m_IsFullScreen);
#endif
}

bool F_W::GetIsMinimized()
{
	return m_IsMinimised;
}

void F_W::Minimize(bool a_MinimizeState)
{
	m_IsMinimised = a_MinimizeState;

#if defined(CURRENT_OS_WINDOWS)
	Windows_Minimize(m_IsMinimised);
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_Minimize(a_MinimizeState);
#endif	
}

bool F_W::GetIsMaximised()
{
	return m_IsMaximised;
}

void F_W::Maximise(bool a_MaximizeState)
{
	m_IsMaximised = a_MaximizeState;

#if defined(CURRENT_OS_WINDOWS)
	Windows_Maximize(m_IsMaximised);
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_Maximize(a_MaximizeState);
#endif
}

void F_W::Restore()
{
#if defined(CURRENT_OS_WINDOWS)
	Windows_Restore();
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_Restore();
#endif
}

void F_W::GetResolution(GLuint& a_Width, GLuint& a_Height)
{
	a_Width = m_Resolution[0];
	a_Height = m_Resolution[1];
}

GLuint* F_W::GetResolution()
{
	return m_Resolution;
}

void F_W::SetResolution(GLuint a_Width, GLuint a_Height)
{
	m_Resolution[0] = a_Width;
	m_Resolution[1] = a_Height;

#if defined(CURRENT_OS_WINDOWS)
	Windows_SetResolution(m_Resolution[0], m_Resolution[1]);
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_SetResolution(a_Width, a_Height);	
#endif

	glViewport(0, 0, m_Resolution[0], m_Resolution[1]);
}

void F_W::GetMousePosition(GLuint& a_X, GLuint& a_Y)
{
	a_X = m_MousePosition[0];
	a_Y = m_MousePosition[1];
}

GLuint* F_W::GetMousePosition()
{
	return m_MousePosition;
}

void F_W::SetMousePosition(GLuint a_X, GLuint a_Y)
{
	m_MousePosition[0] = a_X;
	m_MousePosition[1] = a_Y;
#if defined(CURRENT_OS_WINDOWS)
	Windows_SetMousePosition(a_X, a_Y);
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_SetMousePosition(a_X, a_Y);
#endif
}

void F_W::GetPosition(GLuint& a_X, GLuint& a_Y)
{
	a_X = m_Position[0];
	a_Y = m_Position[1];
}

GLuint* F_W::GetPosition()
{
	return m_Position;
}

void F_W::SetPosition(GLuint a_X, GLuint a_Y)
{
	m_Position[0] = a_X;
	m_Position[1] = a_Y;
#if defined(CURRENT_OS_WINDOWS)
	Windows_SetPosition(m_Position[0], m_Position[1]);
#endif
	
#if defined(CURRENT_OS_LINUX)
	Linux_SetPosition(a_X, a_Y);
#endif
}

const char* F_W::GetWindowName()
{
	return m_WindowName;
}

void F_W::SetName(const char* a_WindowName)
{
	if(a_WindowName != nullptr)
	{
		m_WindowName = a_WindowName;
#if defined(CURRENT_OS_LINUX)
		Linux_SetName(a_WindowName);
#endif

#if defined(CURRENT_OS_WINDOWS)
		Windows_SetName(a_WindowName);
#endif
	}
}

void F_W::MakeCurrentContext()
{
#if defined(CURRENT_OS_WINDOWS)
	wglMakeCurrent(m_DeviceContextHandle, m_GLRenderingContextHandle);
#endif

#if defined(CURRENT_OS_LINUX)
	glXMakeCurrent(F_WM::GetDisplay(), m_Window, m_Context);
#endif
}

bool F_W::GetInFocus()
{
	return m_InFocus;
}

void F_W::Focus(bool a_FocusState)
{
	m_InFocus = a_FocusState;

#if defined(CURRENT_OS_LINUX)
	Linux_Focus(a_FocusState);	
#endif
}

bool F_W::GetIsObscured()
{
	return m_IsObscured;
}

void F_W::SetOnKeyEvent(OnKeyEvent a_OnKeyPressed)
{
	if(Foundation_Tools::IsValid(a_OnKeyPressed))
	{
		m_OnKeyEvent = a_OnKeyPressed;
	}
}

void F_W::SetOnMouseButtonEvent(OnMouseButtonEvent a_OnMouseButtonEvent)
{
	if(Foundation_Tools::IsValid(a_OnMouseButtonEvent))
	{
		m_OnMouseButtonEvent = a_OnMouseButtonEvent;
	}
}

void F_W::SetOnMouseWheelEvent(OnMouseWheelEvent a_OnMouseWheelEvent)
{
	if(Foundation_Tools::IsValid(a_OnMouseWheelEvent))
	{
		m_OnMouseWheel = a_OnMouseWheelEvent;
	}
}

void F_W::SetOnDestroyed(OnDestroyed a_OnDestroyed)
{
	if(Foundation_Tools::IsValid(a_OnDestroyed))
	{
		m_OnDestroyed = a_OnDestroyed;
	}
}

void F_W::SetOnMaximized(OnMaximized a_OnMaximized)
{
	if(Foundation_Tools::IsValid(a_OnMaximized))
	{
		m_OnMaximized = a_OnMaximized;
	}
}

void F_W::SetOnMinimized(OnMinimized a_OnMinimized)
{
	if(Foundation_Tools::IsValid(a_OnMinimized))
	{
		m_OnMinimized = a_OnMinimized;
	}
}

void F_W::SetOnRestored(OnRestored a_OnRestored)
{
	if (Foundation_Tools::IsValid(a_OnRestored))
	{
		m_OnRestored = a_OnRestored;
	}
}

void F_W::SetOnMoved(OnMoved a_OnMoved)
{
	if(Foundation_Tools::IsValid(a_OnMoved))
	{
		m_OnMoved = a_OnMoved;
	}
}

void F_W::SetOnResize(OnResize a_OnResize)
{
	if(Foundation_Tools::IsValid(a_OnResize))
	{
		m_OnResize = a_OnResize;
	}
}

void F_W::SetOnMouseMove(OnMouseMove a_OnMouseMove)
{
	if(Foundation_Tools::IsValid(a_OnMouseMove))
	{
		m_OnMouseMove = a_OnMouseMove;
	}
}