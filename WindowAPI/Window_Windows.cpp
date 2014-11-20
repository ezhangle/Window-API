#include "Window.h"
#include "WindowManager.h"

#if defined(CURRENT_OS_WINDOWS)

HWND F_W::GetWindowHandle()
{
	return m_WindowHandle;
}

void F_W::Windows_InitializeGL()
{
	m_DeviceContextHandle = GetDC(m_WindowHandle);
	InitializePixelFormat();
	m_GLRenderingContextHandle = wglCreateContext(m_DeviceContextHandle);
	wglMakeCurrent(m_DeviceContextHandle, m_GLRenderingContextHandle);

	Windows_InitGLExtensions();
}

void F_W::InitializePixelFormat()
{
	m_PFD = {
		sizeof(PIXELFORMATDESCRIPTOR),  /* size */
		1,                              /* version */
		PFD_SUPPORT_OPENGL |
		PFD_DRAW_TO_WINDOW |
		PFD_DOUBLEBUFFER,               /* support double-buffering */
		PFD_TYPE_RGBA,                  /* color type */
		m_ColourBits, 0,                             /* prefered color depth */
		0, 0,
		0, 0,
		0, 0,
		0,                /* color bits (ignored) */                              /* no alpha buffer */                              /* alpha bits (ignored) */
		0,                              /* no accumulation buffer */
		0, 0, 0, 0,                     /* accum bits (ignored) */
		m_DepthBits,                    /* depth buffer */
		m_StencilBits,                  /* no stencil buffer */
		0,                              /* no auxiliary buffers */
		PFD_MAIN_PLANE,                 /* main layer */
		0,                              /* reserved */
		0, 0, 0,                        /* no layer, visible, damage masks */
	};

	int l_PixelFormat = ChoosePixelFormat(m_DeviceContextHandle, &m_PFD);

	if (l_PixelFormat)
	{
		SetPixelFormat(m_DeviceContextHandle, l_PixelFormat, &m_PFD);
		return;
	}
	return;
}

void F_W::Windows_Initialize(LPCSTR a_MenuName, 
	UINT a_Style /* = CS_OWNDC | CS_HREDRAW | CS_DROPSHADOW */,
	int a_ClearScreenExtra /* = 0 */, 
	int a_WindowExtra /* = 0 */,
	HINSTANCE a_Instance /* = GetModuleHandle(0) */,
	HICON a_Icon /* = LoadIcon(0 , IDI_APPLICATION)*/, 
	HCURSOR a_Cursor /* = LoadCursor(0 , IDC_ARROW)*/,
	HBRUSH a_Brush /* = (HBRUSH)BLACK_BRUSH */)
{
	m_InstanceHandle = a_Instance;
	m_WindowClass.style = a_Style;
	m_WindowClass.lpfnWndProc = F_WM::StaticWindowProcedure;
	m_WindowClass.cbClsExtra = 0;
	m_WindowClass.cbWndExtra = 0;
	m_WindowClass.hInstance = m_InstanceHandle;
	m_WindowClass.hIcon = a_Icon;
	m_WindowClass.hCursor = a_Cursor;
	m_WindowClass.hbrBackground = a_Brush;
	m_WindowClass.lpszMenuName = a_MenuName;
	m_WindowClass.lpszClassName = a_MenuName;
	RegisterClass(&m_WindowClass);

	m_WindowHandle =
		CreateWindow(a_MenuName, a_MenuName, WS_OVERLAPPEDWINDOW, 0,
		0, m_Resolution[0],
		m_Resolution[1],
		0, 0, 0, 0);

	ShowWindow(m_WindowHandle, true);
	UpdateWindow(m_WindowHandle);
}

void F_W::Windows_Shutdown()
{
	if (m_GLRenderingContextHandle)
	{
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(m_GLRenderingContextHandle);
	}

	if (m_PaletteHandle)
	{
		DeleteObject(m_PaletteHandle);
	}
	ReleaseDC(m_WindowHandle, m_DeviceContextHandle);
	DestroyWindow(m_WindowHandle);
	UnregisterClass(m_WindowName, m_InstanceHandle);

	m_DeviceContextHandle = nullptr;
	m_WindowHandle = nullptr;
	m_GLRenderingContextHandle = nullptr;
	
	PostQuitMessage(0);
}



void F_W::Windows_FullScreen(bool a_NewState)
{
		SetWindowLongPtr(m_WindowHandle, GWL_STYLE,
			WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);

		MoveWindow(m_WindowHandle, 0, 0, F_WM::GetScreenResolution()[0], 
			F_WM::GetScreenResolution()[1], true);
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
		AdjustWindowRect(&l_Rect, WS_OVERLAPPEDWINDOW, false);
		MoveWindow(m_WindowHandle, m_Position[0], m_Position[1], l_Rect.right, l_Rect.bottom, true);*/
}

void F_W::Windows_Minimize(bool a_NewState)
{
	if (a_NewState)
	{
		ShowWindow(m_WindowHandle, SW_MINIMIZE);
	}

	else
	{
		ShowWindow(m_WindowHandle, SW_RESTORE);
	}
}

void F_W::Windows_Maximize(bool a_NewState)
{
	if (a_NewState)
	{
		ShowWindow(m_WindowHandle, SW_MAXIMIZE);
	}

	else
	{
		ShowWindow(m_WindowHandle, SW_RESTORE);
	}
}

void F_W::Windows_Restore()
{
	ShowWindow(m_WindowHandle, SW_RESTORE);
}

void F_W::Windows_Focus(bool a_NewState)
{
	m_InFocus = a_NewState;

	if (a_NewState)
	{
		SetFocus(m_WindowHandle);
	}

	else
	{
		SetFocus(nullptr);
	}
}

void F_W::Windows_SetMousePosition(GLuint a_X, GLuint& a_Y)
{
	POINT l_MousePoint;
	l_MousePoint.x = a_X;
	l_MousePoint.y = a_Y;
	ScreenToClient(m_WindowHandle, &l_MousePoint);
	SetCursorPos(l_MousePoint.x, l_MousePoint.y);
}

void F_W::Windows_SetName(const char* a_NewName)
{
	SetWindowText(m_WindowHandle, m_WindowName);
}

void F_W::Windows_SetPosition(GLuint a_X, GLuint a_Y)
{
	RECT rect = { a_X, a_Y, a_X, a_Y };
	AdjustWindowRect(&rect, GWL_STYLE| WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		false);

	SetWindowPos(m_WindowHandle, HWND_TOP, a_X, a_Y,
		m_Resolution[0], m_Resolution[1], SWP_SHOWWINDOW | SWP_NOSIZE);
}

void F_W::Windows_SetResolution(GLuint a_Width, GLuint a_Height)
{
	SetWindowPos(m_WindowHandle, HWND_TOP, m_Position[0], m_Position[1],
		m_Resolution[0], m_Resolution[1], SWP_SHOWWINDOW | SWP_NOMOVE);
}

void F_W::Windows_VerticalSync(GLint a_EnableSync)
{
	if (EXTSwapControlSupported)
	{
		SwapIntervalEXT(a_EnableSync);
	}
}

void F_W::Windows_InitGLExtensions()
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
