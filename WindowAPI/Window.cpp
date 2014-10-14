#include "Window.h"
#include "WindowManager.h"

#ifdef _linux_ || __GNUG__ || __GNUC__ ||__clang__
#include <keysymdef.h>
#endif

Foundation_Window::Foundation_Window(const char* a_WindowName,
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

	if (a_ShouldCreateTerminal)
	{
		CreateTerminal();
	}

}

void Foundation_Window::PollForEvents()
{
#ifdef _MSC_VER
	GetMessage(&m_Message, 0, 0, 0);
	TranslateMessage(&m_Message);
	DispatchMessage(&m_Message);	
#endif

#ifdef __linux__ || __GNUG__ || __GNUC__ || __clang__
	XNextEvent(m_Display, &m_Event);

	switch (m_Event.type)
	{
		case Expose:
		{
			XGetWindowAttributes(m_Display,
				m_Window,
				&m_WindowAttributes);
				glViewport(0, 0, m_Resolution[0],
				m_Resolution[1]);

				m_WindowPosition[0] = m_WindowAttributes.x;
				m_WindowPosition[1] = m_WindowAttributes.y;
			break;
		}

		case KeyPress:
		{			
			char* CurrentlyPressedKey;
			KeySym l_KeySym;
			XLookupString(&m_Event.xkey, CurrentlyPressedKey, sizeof(CurrentlyPressedKey), &l_KeySym, 1);

			GLuint l_FunctionKeysym = XKeycodeToKeysym(m_Display, m_Event.xkey.keycode, 1);
			
			if(l_FunctionKeysym >= 0 && l_FunctionKeysym <= 255)
			{
				m_Keys[CurrentlyPressedKey[0]] = KEYSTATE_DOWN;
				printf("%c\n", CurrentlyPressedKey[0]);		
			}
			
			else
			{
				XTranslateKey(l_FunctionKeysym, KEYSTATE_DOWN);
			}

			break;
		}

		case KeyRelease:
		{
			printf("Key Release");

			bool l_IsRetriggered = false;
			if(XEventsQueued(m_Display, QueuedAfterReading))
			{
				XEvent l_NextEvent;
				XPeekEvent(m_Display, &l_NextEvent);

				if(l_NextEvent.type == KeyPress && 
						l_NextEvent.xkey.time == m_Event.xkey.time && 
						l_NextEvent.xkey.keycode == m_Event.xkey.keycode)
				{
					printf("Key was retriggered\n");
				XNextEvent(m_Display, 
						&m_Event);
				l_IsRetriggered = true;
				}
			}

			if(!l_IsRetriggered)
			{
				printf("Key Was released");
			}

			break;	
		}

		default:
		{
			break;
		}
	}

#endif
}

void Foundation_Window::InitializeGL()
{
	glMatrixMode(GL_PROJECTION);
	glFrustum(-0.5F, 0.5F, -0.5F, 0.5F, 1.0F, 3.0F);

	/* position viewer */
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0.0F, 0.0F, -2.0F);

	/* position object */
	glRotatef(30.0F, 1.0F, 0.0F, 0.0F);
	glRotatef(30.0F, 0.0F, 1.0F, 0.0F);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void Foundation_Window::Redraw()
{
	/* clear color and depth buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* draw six faces of a cube */
	glBegin(GL_QUADS);
	glNormal3f(0.0F, 0.0F, 1.0F);
	glVertex3f(0.5F, 0.5F, 0.5F); glVertex3f(-0.5F, 0.5F, 0.5F);
	glVertex3f(-0.5F, -0.5F, 0.5F); glVertex3f(0.5F, -0.5F, 0.5F);

	glNormal3f(0.0F, 0.0F, -1.0F);
	glVertex3f(-0.5F, -0.5F, -0.5F); glVertex3f(-0.5F, 0.5F, -0.5F);
	glVertex3f(0.5F, 0.5F, -0.5F); glVertex3f(0.5F, -0.5F, -0.5F);

	glNormal3f(0.0F, 1.0F, 0.0F);
	glVertex3f(0.5F, 0.5F, 0.5F); glVertex3f(0.5F, 0.5F, -0.5F);
	glVertex3f(-0.5F, 0.5F, -0.5F); glVertex3f(-0.5F, 0.5F, 0.5F);

	glNormal3f(0.0F, -1.0F, 0.0F);
	glVertex3f(-0.5F, -0.5F, -0.5F); glVertex3f(0.5F, -0.5F, -0.5F);
	glVertex3f(0.5F, -0.5F, 0.5F); glVertex3f(-0.5F, -0.5F, 0.5F);

	glNormal3f(1.0F, 0.0F, 0.0F);
	glVertex3f(0.5F, 0.5F, 0.5F); glVertex3f(0.5F, -0.5F, 0.5F);
	glVertex3f(0.5F, -0.5F, -0.5F); glVertex3f(0.5F, 0.5F, -0.5F);

	glNormal3f(-1.0F, 0.0F, 0.0F);
	glVertex3f(-0.5F, -0.5F, -0.5F); glVertex3f(-0.5F, -0.5F, 0.5F);
	glVertex3f(-0.5F, 0.5F, 0.5F); glVertex3f(-0.5F, 0.5F, -0.5F);
	glEnd();

	//SwapBuffers(m_DeviceContextHandle);
}

void Foundation_Window::Resize(GLuint a_Width, GLuint a_Height)
{

}
#ifdef _MSC_VER
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

#endif

void Foundation_Window::Initialize(const char* a_WindowName, GLuint a_Width, GLuint a_Height, GLuint a_ColourBits,
	GLuint a_DepthBits, GLuint a_StencilBits, bool a_ShouldCreateTerminal) 
{
	m_Resolution[0] = a_Width;
	m_Resolution[1] = a_Height;
	m_WindowName = a_WindowName;
	m_ColourBits = a_ColourBits;
	m_DepthBits = a_DepthBits;
	m_StencilBits = a_StencilBits;

	if (a_ShouldCreateTerminal)
	{
		CreateTerminal();
	}

#ifdef _MSC_VER
	InitializeWin32();
#endif

#ifdef __linux__ || __GNUG__ || __GNUC__ || __clang__

	m_Attributes = new GLuint[5]{GLX_RGBA, GLX_DEPTH_SIZE, m_DepthBits, GLX_DOUBLEBUFFER, None};
	
	m_Display = XOpenDisplay(0);
	
	if(!m_Display)
	{
		printf("Cannot Connect to X Server \n");
		exit(0);
	}

	for(int i = 0; i < 5; i++)
	{
		printf("%i\n", m_Attributes[i]);
	}

	m_VisualInfo = glXChooseVisual(m_Display, 0,
		m_Attributes);

	if (!m_VisualInfo)
	{
		printf("No appropriate visual found\n");
		exit(0);
	}

	m_SetAttributes.colormap = XCreateColormap(m_Display,
		DefaultRootWindow(m_Display), 
		m_VisualInfo->visual, AllocNone);

	m_SetAttributes.event_mask = ExposureMask | KeyPressMask;

	m_Window = XCreateWindow(m_Display,
		DefaultRootWindow(m_Display), 0, 0, 
		m_Resolution[0], m_Resolution[1],
		0, m_VisualInfo->depth, InputOutput, 
		m_VisualInfo->visual, CWColormap | CWEventMask, 
		&m_SetAttributes);

	XMapWindow(m_Display, m_Window);
	XStoreName(m_Display, m_Window,
		m_WindowName);

	m_Context = glXCreateContext(
		m_Display, m_VisualInfo,
		0, GL_TRUE);

	glXMakeCurrent(m_Display,
		m_Window, m_Context);

	XWindowAttributes l_WindowAttributes;

	XGetWindowAttributes(m_Display, m_Window, &l_WindowAttributes);
	m_WindowPosition[0] = l_WindowAttributes.x;
	m_WindowPosition[1] = l_WindowAttributes.y;
#endif

}
#ifdef _MSC_VER
LRESULT CALLBACK Foundation_Window::WindowProcedure(HWND a_WindowHandle, UINT a_Message, WPARAM a_WordParam, LPARAM a_LongParam)
{
	if(m_WindowHandle == a_WindowHandle)
	{
		printf("It's AliVe!!!");
	}

	switch (a_Message)
	{
	case WM_CREATE:
	{
		m_DeviceContextHandle = GetDC(m_WindowHandle);
		InitializePixelFormat();
		m_GLRenderingcontextHandle = wglCreateContext(m_DeviceContextHandle);
		wglMakeCurrent(m_DeviceContextHandle, m_GLRenderingcontextHandle);
		InitializeGL();
		break;
	}
	case WM_DESTROY:
	{
		/* finish OpenGL rendering */
		ShutDownWindow();
		break;
	}

	case WM_SIZE:
	{
		if (m_GLRenderingcontextHandle)
		{	
			Resize((unsigned int)LOWORD(m_LongParam), (unsigned int)HIWORD(m_LongParam));
			break;
		}
		break;
	}

	case WM_SIZING:
	{
		if (m_GLRenderingcontextHandle)
		{
			Resize((unsigned int)LOWORD(m_LongParam), (unsigned int)HIWORD(m_LongParam));
			break;
		}
		break;
	}

	case WM_KEYDOWN:
	{
		TranslateKey(m_WordParam, m_LongParam, KEYSTATE_DOWN);
		break;
	}

	case WM_KEYUP:
	{
		TranslateKey(a_WordParam, a_LongParam, KEYSTATE_UP);
		break;
	}

	case WM_LBUTTONDOWN:
	{
		m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_LBUTTONUP:
	{
		m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONUP;
		break;
	}

	case WM_RBUTTONDOWN:
	{
		m_MouseEvents[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_RBUTTONUP:
	{
		m_MouseEvents[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_MBUTTONDOWN:
	{
		m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_MBUTTONUP:
	{
		m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONUP;
		break;
	}

	default:
	{
		return DefWindowProc(m_WindowHandle, a_Message, m_WordParam, m_LongParam);
	}
	}
	return 0;
}

#endif

#ifdef _MSC_VER
LRESULT CALLBACK Foundation_Window::StaticWindowProcedure(HWND a_WindowHandle, UINT a_Message, WPARAM a_WordParam, LPARAM a_LongParam)
{
	return WindowProcedure(a_WindowHandle, a_Message, a_WordParam, a_LongParam);
}
#endif

#ifdef _MSC_VER
void Foundation_Window::TranslateKey(WPARAM a_WordParam, LPARAM a_LongParam, bool a_KeyState)
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
	#ifdef _MSC_VER
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


bool Foundation_Window::GetWindowShouldClose()
{
	return WindowShouldClose;
}

void Foundation_Window::ShutDownWindow()
{
#ifdef _MSC_VER
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

#ifdef _MSC_VER
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
<<<<<<< HEAD
	m_WindowClass.lpszClassName = a_MenuName;
	RegisterClass(&m_WindowClass);

	m_WindowHandle =
		CreateWindow(a_MenuName, a_MenuName, a_Style, 0,
		0, m_Resolution[0],
		m_Resolution[1],
		0, 0, 0, 0);

	ShowWindow(m_WindowHandle, true);
	UpdateWindow(m_WindowHandle);
}
#endif

void Foundation_Window::Initialize()
{
#ifdef _MSC_VER
	InitializeWin32(m_WindowName);
#endif

#ifdef _linux_ || __GNUG__ || __GNUC__ ||__clang__
	m_SavedScreenState.m_Count = 0;
	m_Attributes = new GLuint[5]{GLX_RGBA, GLX_DEPTH_SIZE, m_DepthBits, GLX_DOUBLEBUFFER, None};

	m_Display = XOpenDisplay(0);

	if (!m_Display)
	{
		printf("Cannot Connect to X Server \n");
		exit(0);
	}

	for (int i = 0; i < 5; i++)
	{
		printf("%i\n", m_Attributes[i]);
	}

	m_VisualInfo = glXChooseVisual(m_Display, 0,
		m_Attributes);

	if (!m_VisualInfo)
	{
		printf("No appropriate visual found\n");
		exit(0);
	}

	m_SetAttributes.colormap = XCreateColormap(m_Display,
		DefaultRootWindow(m_Display),
		m_VisualInfo->visual, AllocNone);

	m_SetAttributes.event_mask = ExposureMask | KeyPressMask;

	m_Window = XCreateWindow(m_Display,
		DefaultRootWindow(m_Display), 0, 0,
		m_Resolution[0], m_Resolution[1],
		0, m_VisualInfo->depth, InputOutput,
		m_VisualInfo->visual, CWColormap | CWEventMask,
		&m_SetAttributes);

	XMapWindow(m_Display, m_Window);
	XStoreName(m_Display, m_Window,
		m_WindowName);

	m_Context = glXCreateContext(
		m_Display, m_VisualInfo,
		0, GL_TRUE);

	glXMakeCurrent(m_Display,
		m_Window, m_Context);

	XWindowAttributes l_WindowAttributes;

	XGetWindowAttributes(m_Display, m_Window, &l_WindowAttributes);
	m_WindowPosition[0] = l_WindowAttributes.x;
	m_WindowPosition[1] = l_WindowAttributes.y;
#endif

}

void Foundation_Window::Window_SwapBuffers()
{
#ifdef _MSC_VER
	SwapBuffers(m_DeviceContextHandle);
#endif

#ifdef __linux__ || __GNUG__ || __GNUC__ || __clang__
	glXSwapBuffers(m_Display, m_Window);
#endif

}
#ifdef __linux__ || __GNUG__ || __GNUC__ ||__clang__
void Foundation_Window::XTranslateKey(GLuint a_KeySym, bool a_KeyState)
{
	switch(a_KeySym)
	{
		case XK_Home:
			{
				m_Keys[KEY_HOME] = a_KeyState;
				break;
			}

		case XK_Left:
			{
				m_Keys[KEY_ARROW_LEFT] = a_KeyState;
				SetFullScreen(false);
				break;
			}

		case XK_Right:
			{
				m_Keys[KEY_ARROW_RIGHT] = a_KeyState;
				SetFullScreen(true);
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
#endif

void Foundation_Window::SetFullScreen(bool a_FullScreenState)
{
#ifdef __linux__ || __GNUC__ || __GNUG_ || __clang__

	if(a_FullScreenState)
	{	
	m_WindowHints.m_Flags = 2;
	m_WindowHints.m_Decorations = 0;

	m_WindowProperty = XInternAtom(m_Display,"_MOTIF_WM_HINTS", False);

	XChangeProperty(m_Display,
			m_Window,
			m_WindowProperty,
			m_WindowProperty,
			32,
			PropModeReplace,
			(unsigned char*)&m_WindowHints, 5);

	XWindowChanges l_WindowChanges;	 
	l_WindowChanges.width = WidthOfScreen(XScreenOfDisplay(m_Display, DefaultScreen(m_Display)));
	l_WindowChanges.height = HeightOfScreen(XScreenOfDisplay(m_Display, DefaultScreen(m_Display)));
	l_WindowChanges.x = 0;
	l_WindowChanges.y = 0;
	XConfigureWindow(m_Display, m_Window, CWWidth | CWHeight | CWX | CWY, &l_WindowChanges); 
	}

	else
	{
		m_WindowHints.m_Flags = 2;
		m_WindowHints.m_Decorations = 3;

		m_WindowProperty = XInternAtom(m_Display, "_MOTIF_WM_HINTS", False);

		GLuint l_Value = 0;

		XChangeProperty(m_Display,
				m_Window,
				m_WindowProperty,
				m_WindowProperty,
				32,
				PropModeReplace,
				(unsigned char*)&l_Value, 1);

		XWindowChanges l_WindowChanges;
		l_WindowChanges.width = m_Resolution[0];
		l_WindowChanges.height = m_Resolution[1];
		l_WindowChanges.x = m_WindowPosition[0];
		l_WindowChanges.y = m_WindowPosition[1];
		XConfigureWindow(m_Display, m_Window, CWWidth | CWHeight | CWX | CWY, &l_WindowChanges);
		printf("%i %i \n", m_WindowPosition[0], m_WindowPosition[1]);
	}	
#endif  
}

void Foundation_Window::GetResolution(GLuint& a_Width, GLuint& a_Height)
{
	a_Width = m_Resolution[0];
	a_Height = m_Resolution[1];
}

void Foundation_Window::SetResolution(GLuint a_Width, GLuint a_Height)
{
	m_Resolution[0] = a_Width;
	m_Resolution[1] = a_Height;
}

const char* Foundation_Window::GetWindowName()
{
	return m_WindowName;
}

#ifdef _MSC_VER
HWND Foundation_Window::GetWindowHandle()
{
	return m_WindowHandle;
}

#endif
