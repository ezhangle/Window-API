#include "Window.h"
#include "WindowManager.h"

#if defined(CURRENT_OS_WINDOWS)

HWND Window::GetWindowHandle()
{
	return WindowHandle;
}

void Window::Windows_InitializeGL()
{
	DeviceContextHandle = GetDC(WindowHandle);
	InitializePixelFormat();
	GLRenderingContextHandle = wglCreateContext(DeviceContextHandle);
	wglMakeCurrent(DeviceContextHandle, GLRenderingContextHandle);

	Windows_InitGLExtensions();
}

void Window::InitializePixelFormat()
{
	PixelFormatDescriptor = {
		sizeof(PIXELFORMATDESCRIPTOR),  /* size */
		1,                              /* version */
		PFD_SUPPORT_OPENGL |
		PFD_DRAW_TO_WINDOW |
		PFD_DOUBLEBUFFER,               /* support double-buffering */
		PFD_TYPE_RGBA,                  /* color type */
		ColourBits, 0,                             /* prefered color depth */
		0, 0,
		0, 0,
		0, 0,
		0,                /* color bits (ignored) */                              /* no alpha buffer */                              /* alpha bits (ignored) */
		0,                              /* no accumulation buffer */
		0, 0, 0, 0,                     /* accum bits (ignored) */
		DepthBits,                    /* depth buffer */
		StencilBits,                  /* no stencil buffer */
		0,                              /* no auxiliary buffers */
		PFD_MAIN_PLANE,                 /* main layer */
		0,                              /* reserved */
		0, 0, 0,                        /* no layer, visible, damage masks */
	};

	int l_PixelFormat = ChoosePixelFormat(DeviceContextHandle, &PixelFormatDescriptor);

	if (l_PixelFormat)
	{
		SetPixelFormat(DeviceContextHandle, l_PixelFormat, &PixelFormatDescriptor);
		return;
	}
	return;
}

void Window::Windows_Initialize(LPCSTR a_MenuName, 
	UINT a_Style /* = CS_OWNDC | CS_HREDRAW | CS_DROPSHADOW */,
	int a_ClearScreenExtra /* = 0 */, 
	int WindowExtra /* = 0 */,
	HINSTANCE a_Instance /* = GetModuleHandle(0) */,
	HICON a_Icon /* = LoadIcon(0 , IDI_APPLICATION)*/, 
	HCURSOR a_Cursor /* = LoadCursor(0 , IDC_ARROW)*/,
	HBRUSH a_Brush /* = (HBRUSH)BLACK_BRUSH */)
{
	InstanceHandle = a_Instance;
	WindowClass.style = a_Style;
	WindowClass.lpfnWndProc = WindowManager::StaticWindowProcedure;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = InstanceHandle;
	WindowClass.hIcon = a_Icon;
	WindowClass.hCursor = a_Cursor;
	WindowClass.hbrBackground = a_Brush;
	WindowClass.lpszMenuName = a_MenuName;
	WindowClass.lpszClassName = a_MenuName;
	RegisterClass(&WindowClass);

	WindowHandle =
		CreateWindow(a_MenuName, a_MenuName, WS_OVERLAPPEDWINDOW, 0,
		0, Resolution[0],
		Resolution[1],
		0, 0, 0, 0);

	ShowWindow(WindowHandle, GL_TRUE);
	UpdateWindow(WindowHandle);
}

void Window::Windows_Shutdown()
{
	if (GLRenderingContextHandle)
	{
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(GLRenderingContextHandle);
	}

	if (PaletteHandle)
	{
		DeleteObject(PaletteHandle);
	}
	ReleaseDC(WindowHandle, DeviceContextHandle);
	DestroyWindow(WindowHandle);
	UnregisterClass(Name, InstanceHandle);

	DeviceContextHandle = nullptr;
	WindowHandle = nullptr;
	GLRenderingContextHandle = nullptr;
	
	PostQuitMessage(0);
}



void Window::Windows_FullScreen(GLboolean NewState)
{
		SetWindowLongPtr(WindowHandle, GWL_STYLE,
			WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);

		MoveWindow(WindowHandle, 0, 0, WindowManager::GetScreenResolution()[0], 
			WindowManager::GetScreenResolution()[1], GL_TRUE);
/*

		DEVMODE l_ScreenSettings;
		memset(&l_ScreenSettings, 0, sizeof(l_ScreenSettings));
		l_ScreenSettings.dmSize = sizeof(l_ScreenSettings);
		l_ScreenSettings.dmPelsWidth = F_WM::GetScreenResolution()[0];
		l_ScreenSettings.dmPelsHeight = F_WM::GetScreenResolution()[1];
		l_ScreenSettings.dmBitsPerPel = m_ColourBits;
		l_ScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		if (ChangeDisplaySettings(&l_ScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			printf("could not successfully change to full screen mode \n");
		}*/


		/*RECT l_Rect;
		l_Rect.left = 0;
		l_Rect.top = 0;
		l_Rect.right = m_Resolution[0];
		l_Rect.bottom = m_Resolution[1];

		DEVMODE l_ScreenSettings;

		l_ScreenSettings.dmSize = sizeof(l_ScreenSettings);
		l_ScreenSettings.dmPelsWidth = m_Resolution[0];
		l_ScreenSettings.dmPelsHeight = m_Resolution[1];
		l_ScreenSettings.dmBitsPerPel = 32;
		l_ScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&l_ScreenSettings, CDS_RESET) != DISP_CHANGE_SUCCESSFUL)
		{
			printf("could not successfully change back to regular mode. dear god what have i done? \n");
		}

		SetWindowLongPtr(m_WindowHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		AdjustWindowRect(&l_Rect, WS_OVERLAPPEDWINDOW, GL_FALSE);
		MoveWindow(m_WindowHandle, m_Position[0], m_Position[1], l_Rect.right, l_Rect.bottom, GL_TRUE);*/
}

void Window::Windows_Minimize(GLboolean NewState)
{
	if (NewState)
	{
		ShowWindow(WindowHandle, SW_MINIMIZE);
	}

	else
	{
		ShowWindow(WindowHandle, SW_RESTORE);
	}
}

void Window::Windows_Maximize(GLboolean NewState)
{
	if (NewState)
	{
		ShowWindow(WindowHandle, SW_MAXIMIZE);
	}

	else
	{
		ShowWindow(WindowHandle, SW_RESTORE);
	}
}

void Window::Windows_Restore()
{
	ShowWindow(WindowHandle, SW_RESTORE);
}

void Window::Windows_Focus(GLboolean NewState)
{
	InFocus = NewState;

	if (NewState)
	{
		SetFocus(WindowHandle);
	}

	else
	{
		SetFocus(nullptr);
	}
}

void Window::Windows_SetMousePosition(GLuint X, GLuint& Y)
{
	POINT l_MousePoint;
	l_MousePoint.x = X;
	l_MousePoint.y = Y;
	ScreenToClient(WindowHandle, &l_MousePoint);
	SetCursorPos(l_MousePoint.x, l_MousePoint.y);
}

void Window::Windows_SetName(const char* NewName)
{
	SetWindowText(WindowHandle, Name);
}

void Window::Windows_SetPosition(GLuint X, GLuint Y)
{
	RECT rect = { X, Y, X, Y };
	AdjustWindowRect(&rect, GWL_STYLE| WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		GL_FALSE);

	SetWindowPos(WindowHandle, HWND_TOP, X, Y,
		Resolution[0], Resolution[1], SWP_SHOWWINDOW | SWP_NOSIZE);
}

void Window::Windows_SetResolution(GLuint Width, GLuint Height)
{
	SetWindowPos(WindowHandle, HWND_TOP, Position[0], Position[1],
		Resolution[0], Resolution[1], SWP_SHOWWINDOW | SWP_NOMOVE);
}

void Window::Windows_VerticalSync(GLint EnableSync)
{
	if (EXTSwapControlSupported)
	{
		SwapIntervalEXT(EnableSync);
	}
}

void Window::Windows_InitGLExtensions()
{
	SwapIntervalEXT = nullptr;
	GetExtensionsStringEXT = nullptr;

	GetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)
		wglGetProcAddress("wglGetExtensionsStringEXT");

	SwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)
		wglGetProcAddress("wglSwapIntervalEXT");

	if (SwapIntervalEXT)
	{
		EXTSwapControlSupported = GL_TRUE;
	}
}

#endif
