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
	WindowShouldClose = false;

	if(!Foundation_Tools::IsValid(m_WindowName))
	{
		printf("that was not a valid window name");
		exit(0);
	}

	InitializeEvents();

#if defined(CURRENT_OS_WINDOWS)
	if (a_ShouldCreateTerminal)
	{
		CreateTerminal();
	}
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

void F_W::InitializeEvents()
{
	m_OnKeyEvent = NULL;
	m_OnMouseButtonEvent = NULL;
	m_OnMouseWheel = NULL;
	m_OnCreated = NULL;
	m_OnDestroyed = NULL;
	//m_OnFullscreen = NULL;
	m_OnMaximized = NULL;
	m_OnMinimized = NULL;
	//m_OnNameChange = NULL;
	m_OnMoved = NULL;
	m_OnFocus = NULL;
	m_OnMouseMove = NULL;
}

bool F_W::GetKey(GLuint a_Key)
{
	return m_Keys[a_Key];
}

void F_W::CreateTerminal()
{
	#if defined(CURRENT_OS_WINDOWS)
	int hConHandle;
	long lStdHandle;
	FILE *fp;

	// allocate a console for this app
	AllocConsole();

	// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;

	setvbuf(stdout, NULL, _IONBF, 0);
	#endif
}


bool F_W::GetShouldClose()
{
	return WindowShouldClose;
}

void F_W::ShutDownWindow()
{
#if defined(CURRENT_OS_WINDOWS)
	if (m_GLRenderingcontextHandle) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_GLRenderingcontextHandle);
	}
	if (m_PaletteHandle)
	{
		DeleteObject(m_PaletteHandle);
	}
	ReleaseDC(m_WindowHandle, m_DeviceContextHandle);
	PostQuitMessage(0);
#endif
}

void F_W::InitializeGL()
{
#if defined(CURRENT_OS_WINDOWS)
	InitializeWin32(m_WindowName);
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_InitializeGL();
#endif
}

void F_W::SwapBuffers()
{
#if defined(CURRENT_OS_WINDOWS)
	SwapBuffers(m_DeviceContextHandle);
#endif

#if defined(CURRENT_OS_LINUX)
	glXSwapBuffers(F_WM::GetDisplay(), m_Window);
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
	if (a_FullScreenState)
	{
		SetWindowLong(m_WindowHandle, GWL_USERDATA, WS_POPUPWINDOW);

		SetWindowPos(m_WindowHandle, HWND_TOP, 0, 0,
			F_WM::GetScreenResolution()[0],
			F_WM::GetScreenResolution()[1],
			SWP_SHOWWINDOW);
	}

	else
	{
		SetWindowLong(m_WindowHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW);

		SetWindowPos(m_WindowHandle, HWND_TOP, 0, 0,
			m_Resolution[0],
			m_Resolution[1],
			SWP_SHOWWINDOW);
	}
#endif
}

bool F_W::GetIsMinimized()
{
	return m_IsMinimised;
}

void F_W::Minimize(bool a_MinimizeState)
{
	m_IsMinimised = a_MinimizeState;

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
#if defined(CURRENT_OS_LINUX)
	Linux_Maximize(a_MaximizeState);
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
	SetWindowPos(m_WindowHandle, HWND_TOP,
		m_Position[0], m_Position[1],
		a_Width, a_Height,
		SWP_SHOWWINDOW);
#endif

#if defined(CURRENT_OS_LINUX)
	Linux_SetResolution(a_Width, a_Height);	
#endif

	glViewport(0, 0, m_Resolution[0], m_Resolution[1]);
}

void F_W::GetMousePosition(GLuint& a_X, GLuint& a_Y)
{
#if defined(CURRENT_OS_WINDOWS)
	POINT l_Point;

	if (GetCursorPos(&l_Point))
	{
		if (ScreenToClient(m_WindowHandle, &l_Point))
		{
			a_X = l_Point.x;
			a_Y = l_Point.y;
		}
	}
#endif

	a_X = m_MousePosition[0];
	a_Y = m_MousePosition[1];
}

GLuint* F_W::GetMousePosition()
{
#if defined(CURRENT_OS_WINDOWS)
	POINT l_Point;
	GLuint l_MousePositionInWindow[2];

	if (GetCursorPos(&l_Point))
	{
		if (ScreenToClient(m_WindowHandle, &l_Point))
		{
			l_MousePositionInWindow[0] = l_Point.x;
			l_MousePositionInWindow[1] = l_Point.y;
		}
	}

	m_MousePosition[0] = l_MousePositionInWindow[0];
	m_MousePosition[1] = l_MousePositionInWindow[1];

	return m_MousePosition;
#endif

#if defined(CURRENT_OS_LINUX)
return m_MousePosition;
#endif
}

void F_W::SetMousePosition(GLuint a_X, GLuint a_Y)
{
#if defined(CURRENT_OS_WINDOWS)
	POINT l_Point;
	l_Point.x = a_X;
	l_Point.y = a_Y;
	ClientToScreen(m_WindowHandle, &l_Point);
	SetCursorPos(l_Point.x, l_Point.y);
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
	SetWindowPos(m_WindowHandle, HWND_TOP,
		m_Position[0], m_Position[1],
		m_Resolution[0], m_Resolution[1],
		SWP_SHOWWINDOW);
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
	if(a_WindowName != NULL || a_WindowName != nullptr)
	{
		m_WindowName = a_WindowName;
#if defined(CURRENT_OS_LINUX)
		Linux_SetName(a_WindowName);
#endif
	}
}

void F_W::MakeCurrentContext()
{
#if defined(CURRENT_OS_WINDOWS)
	wglMakeCurrent(m_DeviceContextHandle, m_GLRenderingcontextHandle);
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

void F_W::SetOnCreated(OnCreated a_OnCreated)
{
	if(Foundation_Tools::IsValid(a_OnCreated))
	{
		m_OnCreated = a_OnCreated;
	}
}

void F_W::SetOnDestroyed(OnDestroyed a_OnDestroyed)
{
	if(Foundation_Tools::IsValid(a_OnDestroyed))
	{
		m_OnDestroyed = a_OnDestroyed;
	}
}

/*void F_W::SetOnFullScreen(OnFullscreen a_OnFullScreen)
{
	if(Foundation_Tools::IsValid(a_OnFullScreen))
	{
		m_OnFullscreen = a_OnFullScreen;
	}
}*/

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

/*void F_W::SetOnNameChange(OnNameChange a_NameChanged)
{
	if(Foundation_Tools::IsValid(a_NameChanged))
	{
		m_OnNameChange = a_NameChanged;
	}
}*/

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

void F_W::SetOnFocus(OnFocus a_OnFocus)
{
	if(Foundation_Tools::IsValid(a_OnFocus))
	{
		m_OnFocus = a_OnFocus;
	}
}

void F_W::SetOnMouseMove(OnMouseMove a_OnMouseMove)
{
	if(Foundation_Tools::IsValid(a_OnMouseMove))
	{
		m_OnMouseMove = a_OnMouseMove;
	}
}
