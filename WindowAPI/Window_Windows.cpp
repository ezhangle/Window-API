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

	GLenum l_Error = glewInit();

	if (l_Error != GLEW_OK)
	{
		printf("%s\n", glewGetErrorString(l_Error));
		exit(0);
	}
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

GLuint F_W::Windows_TranslateKey(WPARAM a_WordParam, LPARAM a_LongParam)
{
	switch (a_WordParam)
	{
		case VK_ESCAPE:
		{
			return KEY_ESCAPE;
		}

		case VK_F1:
		{
			return KEY_F1;
		}

		case VK_F2:
		{
			return KEY_F2;
		}

		case VK_F3:
		{
			return KEY_F3;
		}

		case VK_F4:
		{
			return KEY_F4;
		}

		case VK_F5:
		{
			return KEY_F5;
		}

		case VK_F6:
		{
			return KEY_F6;
		}

		case VK_F7:
		{
			return KEY_F7;
		}

		case VK_F8:
		{
			return KEY_F8;
		}

		case VK_F9:
		{
			return KEY_F9;
		}

		case VK_F10:
		{
			return KEY_F10;
		}

		case VK_F11:
		{
			return KEY_F11;
		}

		case VK_F12:
		{
			return KEY_F12;
		}

		case VK_BACK:
		{
			return KEY_BACKSPACE;
		}

		case VK_TAB:
		{
			return KEY_TAB;
		}

		case VK_CAPITAL:
		{
			return KEY_CAPSLOCK;
		}

		case VK_RETURN:
		{
			return KEY_ENTER;
		}

		case VK_PRINT:
		{
			return KEY_PRINTSCREEN;
		}

		case VK_SCROLL:
		{
			return KEY_SCROLLLOCK;
		}

		case VK_PAUSE:
		{
			return KEY_PAUSE;
		}

		case VK_INSERT:
		{
			return KEY_INSERT;
		}

		case VK_HOME:
		{
			return KEY_HOME;
		}

		case VK_DELETE:
		{
			return KEY_DELETE;
		}

		case VK_END:
		{
			return KEY_END;
		}

		case VK_DOWN:
		{
			return KEY_ARROW_DOWN;
		}

		case VK_UP:
		{
			return KEY_ARROW_UP;
		}

		case VK_LEFT:
		{
			return KEY_ARROW_LEFT;
		}

		case VK_RIGHT:
		{
			return KEY_ARROW_RIGHT;
		}

		case VK_DIVIDE:
		{
			return KEY_KEYPAD_DIVIDE;
		}

		case VK_MULTIPLY:
		{
			return KEY_KEYPAD_MULTIPLY;
		}

		case VK_SUBTRACT:
		{
			return KEY_KEYPAD_DIVIDE;
		}

		case VK_ADD:
		{
			return KEY_KEYPAD_ADD;
		}

		case VK_DECIMAL:
		{
			return KEY_KEYPAD_PERIOD;
		}

		case VK_NUMPAD0:
		{
			return KEY_KEYPAD_0;
		}

		case VK_NUMPAD1:
		{
			return KEY_KEYPAD_1;
		}

		case VK_NUMPAD2:
		{
			return KEY_KEYPAD_2;			
		}

		case VK_NUMPAD3:
		{
			return KEY_KEYPAD_3;			
		}

		case VK_NUMPAD4:
		{
			return KEY_KEYPAD_4;			
		}

		case VK_NUMPAD5:
		{
			return KEY_KEYPAD_5;			
		}

		case VK_NUMPAD6:
		{
			return KEY_KEYPAD_6;			
		}

		case VK_NUMPAD7:
		{
			return KEY_KEYPAD_7;			
		}

		case VK_NUMPAD8:
		{
			return KEY_KEYPAD_8;			
		}

		case VK_NUMPAD9:
		{
			return KEY_KEYPAD_9;			
		}

		default:
		{
			return a_WordParam;
		}
	}
}

void F_W::Windows_FullScreen(bool a_FullScreenState)
{
	if (a_FullScreenState)
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
	}

	else
	{
		RECT l_Rect;
		l_Rect.left = 0;
		l_Rect.top = 0;
		l_Rect.right = m_Resolution[0];
		l_Rect.bottom = m_Resolution[1];

		/*DEVMODE l_ScreenSettings;

		l_ScreenSettings.dmSize = sizeof(l_ScreenSettings);
		l_ScreenSettings.dmPelsWidth = m_Resolution[0];
		l_ScreenSettings.dmPelsHeight = m_Resolution[1];
		l_ScreenSettings.dmBitsPerPel = 32;
		l_ScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&l_ScreenSettings, CDS_RESET) != DISP_CHANGE_SUCCESSFUL)
		{
			printf("could not successfully change back to regular mode. dear god what have i done? \n");
		}*/

		SetWindowLongPtr(m_WindowHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		AdjustWindowRect(&l_Rect, WS_OVERLAPPEDWINDOW, false);
		MoveWindow(m_WindowHandle, m_Position[0], m_Position[1], l_Rect.right, l_Rect.bottom, true);
	}
}

void F_W::Windows_Minimize(bool a_MinimizeState)
{
	if (a_MinimizeState)
	{
		ShowWindow(m_WindowHandle, SW_MINIMIZE);
	}

	else
	{
		ShowWindow(m_WindowHandle, SW_NORMAL);
	}
}

void F_W::Windows_Maximize(bool a_MaximizeState)
{
	if (a_MaximizeState)
	{
		ShowWindow(m_WindowHandle, SW_MAXIMIZE);
	}

	else
	{
		ShowWindow(m_WindowHandle, SW_NORMAL);
	}
}

void F_W::Windows_Restore()
{
	ShowWindow(m_WindowHandle, SW_RESTORE);
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

void F_W::Windows_VerticalSync(bool a_EnableSync)
{
	wglSwapIntervalEXT(a_EnableSync);
}

#endif
