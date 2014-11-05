#include <limits.h>
#include "Window.h"
#include "WindowManager.h"

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)
	//#include <keysymdef.h>
//#include <glx.h>
#include <cstring>
#endif

Foundation_Window::Foundation_Window(const char*  a_WindowName,
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

	if(m_WindowName == NULL || m_WindowName == nullptr)
	{
		printf("that was not a valid window name");
		exit(0);
	}

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)

	if (a_ShouldCreateTerminal)
	{
		CreateTerminal();
	}
#endif

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)

	m_Attributes = new GLuint[5]{GLX_RGBA, GLX_DEPTH_SIZE, m_DepthBits, GLX_DOUBLEBUFFER, None};
	//Foundation_WindowManager::GetInstance()->m_Display = XOpenDisplay(0);

	if (!Foundation_WindowManager::GetDisplay())
	{
		printf("Cannot Connect to X Server \n");
		exit(0);
	}

	m_VisualInfo = glXChooseVisual(Foundation_WindowManager::GetDisplay(), 0,
		m_Attributes);

	if (!m_VisualInfo)
	{
		printf("No appropriate visual found\n");
		exit(0);
	}

	m_SetAttributes.colormap = XCreateColormap(Foundation_WindowManager::GetDisplay(),
		DefaultRootWindow(Foundation_WindowManager::GetDisplay()),
		m_VisualInfo->visual, AllocNone);

	m_SetAttributes.event_mask = ExposureMask | KeyPressMask 
	   	| KeyReleaseMask | MotionNotify | ButtonPressMask | ButtonReleaseMask
	   	| FocusIn | FocusOut | Button1MotionMask | Button2MotionMask | Button3MotionMask | 
		Button4MotionMask | Button5MotionMask | PointerMotionMask | FocusChangeMask
		| SubstructureNotifyMask | VisibilityChangeMask;

//	m_SetAttributes.override_redirect = True;

	//m_SetAttributes.bit_gravity

	m_Window = XCreateWindow(Foundation_WindowManager::GetInstance()->m_Display,
		XDefaultRootWindow(Foundation_WindowManager::GetInstance()->m_Display), 0, 0,
		m_Resolution[0], m_Resolution[1],
		0, m_VisualInfo->depth, InputOutput,
		m_VisualInfo->visual, CWColormap | CWEventMask /*CWOverrideRedirect*/,
		&m_SetAttributes);

	XMapWindow(Foundation_WindowManager::GetDisplay(), m_Window);
	XStoreName(Foundation_WindowManager::GetDisplay(), m_Window,
		m_WindowName);

	InitializeAtomics();

	XSetWMProtocols(Foundation_WindowManager::GetDisplay(), 
			m_Window, &m_ACloseWindow, 1);

	XSetWMProtocols(Foundation_WindowManager::GetDisplay(),
		m_Window, &m_AFullScreenState, 1);	
#endif
}

Foundation_Window::~Foundation_Window()
{
	Shutdown();


}

void Foundation_Window::Shutdown()
{
	glXDestroyContext(Foundation_WindowManager::GetDisplay(), m_Context);
	XDestroyWindow(Foundation_WindowManager::GetDisplay(), m_Window);
}

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
void Foundation_Window::InitializePixelFormat()
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

void Foundation_Window::InitializeWin32(LPCSTR a_MenuName, 
	UINT a_Style /* = CS_OWNDC | CS_HREDRAW | CS_DROPSHADOW */,
	int a_ClearScreenExtra /* = 0 */, 
	int a_WindowExtra /* = 0 */,
	HINSTANCE a_Instance /* = GetModuleHandle(0) */,
	HICON a_Icon /* = LoadIcon(0 , IDI_APPLICATION)*/, 
	HCURSOR a_Cursor /* = LoadCursor(0 , IDC_ARROW)*/,
	HBRUSH a_Brush /* = (HBRUSH)BLACK_BRUSH */)
{
	m_WindowClass.style = a_Style;
	m_WindowClass.lpfnWndProc = Foundation_WindowManager::StaticWindowProcedure;
	m_WindowClass.cbClsExtra = 0;
	m_WindowClass.cbWndExtra = 0;
	m_WindowClass.hInstance = a_Instance;
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

void Foundation_Window::Win32TranslateKey(WPARAM a_WordParam, LPARAM a_LongParam, bool a_KeyState)
{
	switch (a_WordParam)
	{
		case VK_ESCAPE:
		{
			m_Keys[KEY_ESCAPE] = a_KeyState;
			break;
		}

		case VK_F1:
		{
			m_Keys[KEY_F1] = a_KeyState;
			break;
		}

		case VK_F2:
		{
			m_Keys[KEY_F2] = a_KeyState;
			break;
		}

		case VK_F3:
		{
			m_Keys[KEY_F3] = a_KeyState;
			break;
		}

		case VK_F4:
		{
			m_Keys[KEY_F4] = a_KeyState;
			break;
		}

		case VK_F5:
		{
			m_Keys[KEY_F5] = a_KeyState;
			break;
		}

		case VK_F6:
		{
			m_Keys[KEY_F6] = a_KeyState;
			break;
		}

		case VK_F7:
		{
			m_Keys[KEY_F7] = a_KeyState;
			break;
		}

		case VK_F8:
		{
			m_Keys[KEY_F8] = a_KeyState;
			break;
		}

		case VK_F9:
		{
			m_Keys[KEY_F9] = a_KeyState;
			break;
		}

		case VK_F10:
		{
			m_Keys[KEY_F10] = a_KeyState;
			break;
		}

		case VK_F11:
		{
			m_Keys[KEY_F11] = a_KeyState;
			break;
		}

		case VK_F12:
		{
			m_Keys[KEY_F12] = a_KeyState;
			break;
		}

		case VK_BACK:
		{
			m_Keys[KEY_BACKSPACE] = a_KeyState;
			break;
		}

		case VK_TAB:
		{
			m_Keys[KEY_TAB] = a_KeyState;
			break;
		}

		case VK_CAPITAL:
		{
			m_Keys[KEY_CAPSLOCK] = a_KeyState;
			break;
		}

		case VK_SHIFT:
		{
			//m_Keys
			break;
		}

		case VK_CONTROL:
		{
			break;
		}

		case VK_MENU:
		{
			break;
		}

		case VK_RETURN:
		{
			m_Keys[KEY_ENTER] = a_KeyState;
			break;
		}

		case VK_PRINT:
		{
			m_Keys[KEY_PRINTSCREEN] = a_KeyState;
			break;
		}

		case VK_SCROLL:
		{
			m_Keys[KEY_SCROLLLOCK] = a_KeyState;
			break;
		}

		case VK_PAUSE:
		{
			m_Keys[KEY_PAUSE] = a_KeyState;
			break;
		}

		case VK_INSERT:
		{
			m_Keys[KEY_INSERT] = a_KeyState;
			break;
		}

		case VK_HOME:
		{
			m_Keys[KEY_HOME] = a_KeyState;
			break;
		}

		case VK_DELETE:
		{
			m_Keys[KEY_DELETE] = a_KeyState;
			break;
		}

		case VK_END:
		{
			m_Keys[KEY_END] = a_KeyState;
			break;
		}

		case VK_DOWN:
		{
			m_Keys[KEY_ARROW_DOWN] = a_KeyState;
			break;
		}

		case VK_UP:
		{
			m_Keys[KEY_ARROW_UP] = a_KeyState;
			break;
		}

		case VK_LEFT:
		{
			m_Keys[KEY_ARROW_LEFT] = a_KeyState;
			break;
		}

		case VK_RIGHT:
		{
			m_Keys[KEY_ARROW_RIGHT] = a_KeyState;
			break;
		}

		case VK_DIVIDE:
		{
			m_Keys[KEY_KEYPAD_DIVIDE] = a_KeyState;
			break;
		}

		case VK_MULTIPLY:
		{
			m_Keys[KEY_KEYPAD_MULTIPLY] = a_KeyState;
			break;
		}

		case VK_SUBTRACT:
		{
			m_Keys[KEY_KEYPAD_DIVIDE] = a_KeyState;
			break;
		}

		case VK_ADD:
		{
			m_Keys[KEY_KEYPAD_ADD] = a_KeyState;
			break;
		}

		case VK_DECIMAL:
		{
			m_Keys[KEY_KEYPAD_PERIOD] = a_KeyState;
			break;
		}

		case VK_NUMPAD0:
		{
			m_Keys[KEY_KEYPAD_0] = a_KeyState;
			break;
		}

		case VK_NUMPAD1:
		{
			m_Keys[KEY_KEYPAD_1] = a_KeyState;
			break;
		}

		case VK_NUMPAD2:
		{
			m_Keys[KEY_KEYPAD_2] = a_KeyState;
			break;
		}

		case VK_NUMPAD3:
		{
			m_Keys[KEY_KEYPAD_3] = a_KeyState;
			break;
		}

		case VK_NUMPAD4:
		{
			m_Keys[KEY_KEYPAD_4] = a_KeyState;
			break;
		}

		case VK_NUMPAD5:
		{
			m_Keys[KEY_KEYPAD_5] = a_KeyState;
			break;
		}

		case VK_NUMPAD6:
		{
			m_Keys[KEY_KEYPAD_6] = a_KeyState;
			break;
		}

		case VK_NUMPAD7:
		{
			m_Keys[KEY_KEYPAD_7] = a_KeyState;
			break;
		}

		case VK_NUMPAD8:
		{
			m_Keys[KEY_KEYPAD_8] = a_KeyState;
			break;
		}

		case VK_NUMPAD9:
		{
			m_Keys[KEY_KEYPAD_9] = a_KeyState;
			break;
		}
		default:
		{
			return;
		}
	}
}
#endif

bool Foundation_Window::GetKey(GLuint a_Key)
{
	return m_Keys[a_Key];
}

void Foundation_Window::CreateTerminal()
{
	#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
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


bool Foundation_Window::GetShouldClose()
{
	return WindowShouldClose;
}

void Foundation_Window::ShutDownWindow()
{
#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
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

void Foundation_Window::InitializeGL()
{
#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
	InitializeWin32(m_WindowName);
#endif

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)

	Linux_InitializeGL();
#endif
}

void Foundation_Window::SwapBuffers()
{
#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
	SwapBuffers(m_DeviceContextHandle);
#endif

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)
	glXSwapBuffers(Foundation_WindowManager::GetDisplay(), m_Window);
#endif
}

bool Foundation_Window::GetIsFullScreen()
{
	return m_IsFullScreen;
}

void Foundation_Window::FullScreen(bool a_FullScreenState)
{
#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)

	Linux_FullScreen(a_FullScreenState);

#endif

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)

	if (a_FullScreenState)
	{
		SetWindowLong(m_WindowHandle, GWL_USERDATA, WS_POPUPWINDOW);

		SetWindowPos(m_WindowHandle, HWND_TOP, 0, 0,
			Foundation_WindowManager::GetScreenResolution()[0],
			Foundation_WindowManager::GetScreenResolution()[1],
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

bool Foundation_Window::GetIsMinimized()
{
	return m_IsMinimised;
}

void Foundation_Window::Minimize(bool a_MinimizeState)
{
#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)

	Linux_Minimize(a_MinimizeState);
#endif	
}

bool Foundation_Window::GetIsMaximised()
{
	return m_IsMaximised;
}

void Foundation_Window::Maximise(bool a_MaximizeState)
{
#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)
	Linux_Maximize(a_MaximizeState);

#endif
}

void Foundation_Window::GetResolution(GLuint& a_Width, GLuint& a_Height)
{
	a_Width = m_Resolution[0];
	a_Height = m_Resolution[1];
}

GLuint* Foundation_Window::GetResolution()
{
	return m_Resolution;
}

void Foundation_Window::SetResolution(GLuint a_Width, GLuint a_Height)
{
	m_Resolution[0] = a_Width;
	m_Resolution[1] = a_Height;

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
	SetWindowPos(m_WindowHandle, HWND_TOP,
		m_Position[0], m_Position[1],
		a_Width, a_Height,
		SWP_SHOWWINDOW);
#endif

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)

	Linux_SetResolution();	

#endif
	glViewport(0, 0, m_Resolution[0], m_Resolution[1]);
}

void Foundation_Window::GetMousePositionInWindow(GLuint& a_X, GLuint& a_Y)
{
#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
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

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)
a_X = m_MousePosition[0];
a_Y = m_MousePosition[1];
#endif
}

GLuint* Foundation_Window::GetMousePositionInWindow()
{
#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
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

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)
return m_MousePosition;
#endif

}

void Foundation_Window::SetMousePositionInWindow(GLuint a_X, GLuint a_Y)
{

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)

	POINT l_Point;
	l_Point.x = a_X;
	l_Point.y = a_Y;
	ClientToScreen(m_WindowHandle, &l_Point);
	SetCursorPos(l_Point.x, l_Point.y);

#endif

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)
	
	Linux_SetMousePositionInWindow(a_X, a_Y);

#endif
}

void Foundation_Window::GetPosition(GLuint& a_X, GLuint& a_Y)
{
	a_X = m_Position[0];
	a_Y = m_Position[1];
}

GLuint* Foundation_Window::GetPosition()
{
	return m_Position;
}

void Foundation_Window::SetPosition(GLuint a_X, GLuint a_Y)
{
	m_Position[0] = a_X;
	m_Position[1] = a_Y;

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
	SetWindowPos(m_WindowHandle, HWND_TOP,
		m_Position[0], m_Position[1],
		m_Resolution[0], m_Resolution[1],
		SWP_SHOWWINDOW);
#endif
	
#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)

	Linux_SetPosition(a_X, a_Y);
#endif
}

const char* Foundation_Window::GetWindowName()
{
	return m_WindowName;
}

void Foundation_Window::SetName(const char* a_WindowName)
{
	if(a_WindowName != NULL || a_WindowName != nullptr)
	{
		m_WindowName = a_WindowName;
#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined (__clang__)

		Linux_SetWindowName();

#endif
	}
}

void Foundation_Window::MakeCurrentContext()
{
#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
	wglMakeCurrent(m_DeviceContextHandle, m_GLRenderingcontextHandle);
#endif

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)
	glXMakeCurrent(Foundation_WindowManager::GetDisplay(), m_Window, m_Context);
#endif

}

void Foundation_Window::AddSelfToManager()
{
	//check if this window is already in the manager
	//if not then add else just skip over

	bool l_Successful = false;

	for(GLuint l_Iter = 0; l_Iter < Foundation_WindowManager::GetNumWindows(); l_Iter++)
	{
		if(this == Foundation_WindowManager::GetWindowByIndex(l_Iter))
		{
			l_Successful = true;
		}
	}

	if(!l_Successful)
	{
		Foundation_WindowManager::AddWindow(this);
	}
}

bool Foundation_Window::GetInFocus()
{
	return m_InFocus;
}

bool Foundation_Window::GetIsObscured()
{
	return m_IsObscured;
}

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
HWND Foundation_Window::GetWindowHandle()
{
	return m_WindowHandle;
}
#endif

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)
void Foundation_Window::Linux_SetResolution()
{
	XResizeWindow(Foundation_WindowManager::GetDisplay(),
		m_Window, m_Resolution[0], m_Resolution[1]);	
}

void Foundation_Window::Linux_SetPosition(GLuint a_X, GLuint a_Y)
{

	XWindowChanges l_WindowChanges;

	l_WindowChanges.x = a_X;
	l_WindowChanges.y = a_Y;

	XConfigureWindow(
			Foundation_WindowManager::GetDisplay(),
			m_Window, CWX | CWY, &l_WindowChanges);
}

void Foundation_Window::Linux_SetMousePositionInWindow(GLuint a_X, GLuint a_Y)
{
	XWarpPointer(
			Foundation_WindowManager::GetInstance()->m_Display,
			m_Window, m_Window,
			m_Position[0], m_Position[1],
			m_Resolution[0], m_Resolution[1],
			a_X, a_Y);
}

void Foundation_Window::Linux_FullScreen(bool a_FullScreenState)
{
	m_IsFullScreen = a_FullScreenState;

	XEvent l_Event;

	memset(&l_Event, 0, sizeof(l_Event));

	l_Event.xany.type = ClientMessage;
	l_Event.xclient.message_type = m_AState;
	l_Event.xclient.format = 32;
	l_Event.xclient.window = m_Window;
	l_Event.xclient.data.l[0] = m_IsFullScreen;
	l_Event.xclient.data.l[1] = m_AFullScreenState;

	XSendEvent(Foundation_WindowManager::GetDisplay(),
			XDefaultRootWindow(Foundation_WindowManager::GetDisplay()),
			0, SubstructureNotifyMask, &l_Event);
}

void Foundation_Window::Linux_Minimize(bool a_MinimizeState)
{
	m_IsMinimised = a_MinimizeState;

	if(m_IsMinimised)
	{
		XIconifyWindow(Foundation_WindowManager::GetDisplay(),
				m_Window, 0);
	}

	else
	{
		XMapWindow(Foundation_WindowManager::GetDisplay(),
				m_Window);
	}
}

void Foundation_Window::Linux_Maximize(bool a_MaximizeState)
{

	m_IsMaximised = a_MaximizeState;
	XEvent l_Event;

	memset(&l_Event, 0, sizeof(l_Event));

	l_Event.xany.type = ClientMessage;
	l_Event.xclient.message_type = m_AState;
	l_Event.xclient.format = 32;
	l_Event.xclient.window = m_Window;
	l_Event.xclient.data.l[0] = m_IsMaximised;
	l_Event.xclient.data.l[1] = m_AMaximizedVertical;
	l_Event.xclient.data.l[2] = m_AMaximizedHorizontal;

	XSendEvent(Foundation_WindowManager::GetDisplay(),
			XDefaultRootWindow(Foundation_WindowManager::GetDisplay()),
			0, SubstructureNotifyMask, &l_Event);
	
	if(!m_IsMaximised)
	{
		XMoveWindow(Foundation_WindowManager::GetDisplay(),
				m_Window, m_Position[0], m_Position[1]);
	}
}

void Foundation_Window::Linux_SetWindowName()
{
	XStoreName(Foundation_WindowManager::GetDisplay(),
			m_Window, m_WindowName);
}

void Foundation_Window::Linux_FocusWindow(bool a_FocusState)
{

}

void Foundation_Window::Linux_TranslateKey(GLuint a_KeySym, bool a_KeyState)
{
	switch(a_KeySym)
	{
		case XK_Escape:
			{
				//printf("escape was pressed");
				WindowShouldClose = true;
				m_Keys[KEY_ESCAPE] = a_KeyState;
				break;
			}


		case XK_Home:
			{
				m_Keys[KEY_HOME] = a_KeyState;
				break;
			}

		case XK_Left:
			{
				m_Keys[KEY_ARROW_LEFT] = a_KeyState;
				//SetResolution(800, 600);
				break;
			}

		case XK_Right:
			{
				m_Keys[KEY_ARROW_RIGHT] = a_KeyState;
				//SetResolution(1366, 768);
				break;
			}

		case XK_Up:
			{
				m_Keys[KEY_ARROW_UP] = a_KeyState;
				break;
			}

		case XK_Down:
			{
				m_Keys[KEY_ARROW_DOWN] = a_KeyState;
				break;
			}

		case XK_Page_Up:
			{
				m_Keys[KEY_PAGEUP] = a_KeyState;
				break;
			}

		case XK_Page_Down:
			{
				m_Keys[KEY_PAGEDOWN] = a_KeyState;
				break;
			}

		case XK_End:
			{
				m_Keys[KEY_END] = a_KeyState;
				break;
			}

		case XK_Print:
			{
				m_Keys[KEY_PRINTSCREEN] = a_KeyState;
				break;
			}

		case XK_Insert:
			{
				m_Keys[KEY_INSERT] = a_KeyState;
				break;
			}

		case XK_Num_Lock:
			{
				m_Keys[KEY_NUMLOCK] = a_KeyState;
				break;
			}

		case XK_KP_Multiply:
			{
				m_Keys[KEY_KEYPAD_MULTIPLY] = a_KeyState;
				break;
			}

		case XK_KP_Add:
			{
				m_Keys[KEY_KEYPAD_ADD] = a_KeyState;
				break;
			}

		case XK_KP_Subtract:
			{
				m_Keys[KEY_KEYPAD_SUBTRACT] = a_KeyState;
				break;
			}

		case XK_KP_Decimal:
			{
				m_Keys[KEY_KEYPAD_PERIOD] = a_KeyState;
				break;
			}

		case XK_KP_Divide:
			{
				m_Keys[KEY_KEYPAD_DIVIDE] = a_KeyState;
				break;
			}

		case XK_KP_0:
			{
				m_Keys[KEY_KEYPAD_0] = a_KeyState;
				break;
			}

		case XK_KP_1:
			{
				m_Keys[KEY_KEYPAD_1] = a_KeyState;
				break;
			}

		case XK_KP_2:
			{
				m_Keys[KEY_KEYPAD_2] = a_KeyState;
				break;
			}

		case XK_KP_3:
			{
				m_Keys[KEY_KEYPAD_3] = a_KeyState;
				break;
			}

		case XK_KP_4:
			{
				m_Keys[KEY_KEYPAD_4] = a_KeyState;
				break;
			}

		case XK_KP_5:
			{
				m_Keys[KEY_KEYPAD_5] = a_KeyState;
				break;
			}

		case XK_KP_6:
			{
				m_Keys[KEY_KEYPAD_6] = a_KeyState;
				break;
			}

		case XK_KP_7:
			{
				m_Keys[KEY_KEYPAD_7] = a_KeyState;
				break;
			}

		case XK_KP_8:
			{
				m_Keys[KEY_KEYPAD_8] = a_KeyState;
				break;
			}

		case XK_KP_9:
			{
				m_Keys[KEY_KEYPAD_9] = a_KeyState;
				break;
			}

		case XK_F1:
			{
				m_Keys[KEY_F1] = a_KeyState;
				break;
			}

		case XK_F2:
			{
				m_Keys[KEY_F2] = a_KeyState;
				break;
			}

		case XK_F3:
			{
				m_Keys[KEY_F3] = a_KeyState;
				break;
			}

		case XK_F4:
			{
				m_Keys[KEY_F4] = a_KeyState;
				break;
			}

		case XK_F5:
			{
				m_Keys[KEY_F5] = a_KeyState;
				break;
			}

		case XK_F6:
			{
				m_Keys[KEY_F6] = a_KeyState;
				break;
			}

		case XK_F7:
			{
				m_Keys[KEY_F7] = a_KeyState;
				break;
			}

		case XK_F8:
			{
				m_Keys[KEY_F8] = a_KeyState;
				break;
			}

		case XK_F9:
			{
				m_Keys[KEY_F9] = a_KeyState;
				break;
			}

		case XK_F10:
			{
				m_Keys[KEY_F10] = a_KeyState;
				break;
			}

		case XK_F11:
			{
				m_Keys[KEY_F11] = a_KeyState;
				break;
			}

		case XK_F12:
			{
				m_Keys[KEY_F12] = a_KeyState;
				break;
			}

		case XK_Shift_L:
			{
				m_Keys[KEY_LEFTSHIFT] = a_KeyState;
				break;
			}

		case XK_Shift_R:
			{
				m_Keys[KEY_RIGHTSHIFT] = a_KeyState;
				break;
			}

		case XK_Control_R:
			{
				m_Keys[KEY_RIGHTCONTROL] = a_KeyState;
				break;
			}

		case XK_Control_L:
			{
				m_Keys[KEY_LEFTCONTROL] = a_KeyState;
				break;
			}

		case XK_Caps_Lock:
			{
				m_Keys[KEY_CAPSLOCK] = a_KeyState;
				break;
			}

		case XK_Alt_L:
			{
				m_Keys[KEY_LEFTALT] = a_KeyState;
				break;
			}

		case XK_Alt_R:
			{
				m_Keys[KEY_RIGHTALT] = a_KeyState;
				break;
			}

		default:
			{
				break;
			}
	}
}

void Foundation_Window::Linux_InitializeGL()
{
	m_Context = glXCreateContext(
			Foundation_WindowManager::GetDisplay(),
			m_VisualInfo, 0, GL_TRUE);

	glXMakeCurrent(
			Foundation_WindowManager::GetDisplay(),
			m_Window, m_Context);

	XWindowAttributes l_Attributes;

	XGetWindowAttributes(Foundation_WindowManager::GetDisplay(),
			m_Window, &l_Attributes);
	m_Position[0] = l_Attributes.x;
	m_Position[1] = l_Attributes.y;
}

void Foundation_Window::InitializeAtomics()
{
	m_AState = XInternAtom(Foundation_WindowManager::GetDisplay(), "_NET_WM_STATE", False);
	m_AAddState = XInternAtom(Foundation_WindowManager::GetDisplay(), "_NET_WM_STATE_ADD", False);
	m_ARemoveState = XInternAtom(Foundation_WindowManager::GetDisplay(), "_NET_WM_STATE_REMOVE", False);
	m_AFullScreenState = XInternAtom(Foundation_WindowManager::GetDisplay(), "_NET_WM_STATE_FULLSCREEN", False);
	m_AMaximizedHorizontal = XInternAtom(Foundation_WindowManager::GetDisplay(), "_NET_WM_STATE_MAXIMIZED_HORZ", False);
	m_AMaximizedVertical = XInternAtom(Foundation_WindowManager::GetDisplay(), "_NET_WM_STATE_MAXIMIZED_VERT", False);
	m_AWindowMotifs = XInternAtom(Foundation_WindowManager::GetDisplay(), "_MOTIF_WM_MOTIFS", False);
	m_ABypassCompositor = XInternAtom(Foundation_WindowManager::GetDisplay(), "_NET_WM_BYPASS_COMPOSITOR", False);
	m_AActiveWindow = XInternAtom(Foundation_WindowManager::GetDisplay(), "_NET_ACTIVE_WINDOW", False);
	m_ACloseWindow = XInternAtom(Foundation_WindowManager::GetDisplay(), "_NET_CLOSE_WINDOW", False);
}

Window Foundation_Window::GetWindowHandle()
{
	return m_Window;
}
#endif
