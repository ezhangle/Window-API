#include "Window.h"

#ifdef _linux_ || __GNUG__ || __GNUC__ ||__clang__
#include <keysymdef.h>
#endif

Foundation_Window::Foundation_Window()
{
	m_SavedScreenState.m_Count = 0;		
}

Foundation_Window* Foundation_Window::GetInstance()
{
	if (!m_Instance)
	{
		m_Instance = new Foundation_Window();
		return m_Instance;
	}

	
	{
		return m_Instance;
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
	XNextEvent(GetInstance()->m_Display, &GetInstance()->m_Event);

	switch (GetInstance()->m_Event.type)
	{
		case Expose:
		{
			XGetWindowAttributes(GetInstance()->m_Display,
				GetInstance()->m_Window,
				&GetInstance()->m_WindowAttributes);
				glViewport(0, 0, GetInstance()->m_Resolution[0],
				GetInstance()->m_Resolution[1]);

				GetInstance()->m_WindowPosition[0] = GetInstance()->m_WindowAttributes.x;
				GetInstance()->m_WindowPosition[1] = GetInstance()->m_WindowAttributes.y;
			break;
		}

		case KeyPress:
		{			
			char* CurrentlyPressedKey;
			KeySym l_KeySym;
			XLookupString(&GetInstance()->m_Event.xkey, CurrentlyPressedKey, sizeof(CurrentlyPressedKey), &l_KeySym, 1);
			//printf("%i\n", CurrentlyPressedKey[0]);

			GLuint l_FunctionKeysym = XKeycodeToKeysym(GetInstance()->m_Display, GetInstance()->m_Event.xkey.keycode, 1);
			
			if(l_FunctionKeysym >= 0 && l_FunctionKeysym <= 255)
			{
				GetInstance()->m_Keys[CurrentlyPressedKey[0]] = KEYSTATE_DOWN;
				printf("%c\n %i\n", CurrentlyPressedKey[0]);		
			}
			
			else
			{
				GetInstance()->XTranslateKey(l_FunctionKeysym, KEYSTATE_DOWN);
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
				XPeekEvent(GetInstance()->m_Display, &l_NextEvent);

				if(l_NextEvent.type == KeyPress && 
						l_NextEvent.xkey.time == GetInstance()->m_Event.xkey.time && 
						l_NextEvent.xkey.keycode == GetInstance()->m_Event.xkey.keycode)
				{
					printf("Key was retriggered\n");
				XNextEvent(GetInstance()->m_Display, 
						&GetInstance()->m_Event);
				l_IsRetriggered = true;
				}
			}

			if(!l_IsRetriggered)
			{
				printf("Key Was released");
			}	



			/*if(XEventsQueued(m_Display, QueuedAfterReading))
			{
			XEvent l_NextEvent;
			XPeekEvent(m_Display, &l_NextEvent);

			if(l_NextEvent.type == KeyPress && l_NextEvent.xkey.time == m_Event.xkey.time && l_NextEvent.xkey.keycode == m_Event.xkey.keycode)
			{
				printf("Key is being repeated");
			}

			else
			{

			char* CurrentlyPressedKey;
			KeySym l_KeySym;
			XLookupString(&GetInstance()->m_Event.xkey, CurrentlyPressedKey, sizeof(CurrentlyPressedKey), &l_KeySym, 1);
			printf("key released");

			GLuint l_FunctionKeysym = XKeycodeToKeysym(GetInstance()->m_Display, GetInstance()->m_Event.xkey.keycode, 1);
			
			if(l_FunctionKeysym >= 0 && l_FunctionKeysym <= 255)
			{
				GetInstance()->m_Keys[CurrentlyPressedKey[0]] = KEYSTATE_UP;
				printf("~%c\n", CurrentlyPressedKey[0]);		
			}
			
			else
			{
				GetInstance()->XTranslateKey(l_FunctionKeysym, KEYSTATE_UP);
			}
			}
			}*/

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
	GetInstance()->m_PFD = {
		sizeof(PIXELFORMATDESCRIPTOR),  /* size */
		1,                              /* version */
		PFD_SUPPORT_OPENGL |
		PFD_DRAW_TO_WINDOW |
		PFD_DOUBLEBUFFER,               /* support double-buffering */
		PFD_TYPE_RGBA,                  /* color type */
		GetInstance()->m_ColourBits, 0,                             /* prefered color depth */
		0, 0,
		0, 0,
		0, 0,
		0,                /* color bits (ignored) */                              /* no alpha buffer */                              /* alpha bits (ignored) */
		0,                              /* no accumulation buffer */
		0, 0, 0, 0,                     /* accum bits (ignored) */
		GetInstance()->m_DepthBits,                    /* depth buffer */
		GetInstance()->m_StencilBits,                  /* no stencil buffer */
		0,                              /* no auxiliary buffers */
		PFD_MAIN_PLANE,                 /* main layer */
		0,                              /* reserved */
		0, 0, 0,                        /* no layer, visible, damage masks */
	};

	int l_PixelFormat = ChoosePixelFormat(GetInstance()->m_DeviceContextHandle, &GetInstance()->m_PFD);

	if (l_PixelFormat)
	{
		SetPixelFormat(GetInstance()->m_DeviceContextHandle, l_PixelFormat, &GetInstance()->m_PFD);
		return;
	}
	return;
}

#endif

void Foundation_Window::Initialize(const char* a_WindowName, GLuint a_Width, GLuint a_Height, GLuint a_ColourBits,
	GLuint a_DepthBits, GLuint a_StencilBits, bool a_ShouldCreateTerminal) 
{
	GetInstance()->m_Resolution[0] = a_Width;
	GetInstance()->m_Resolution[1] = a_Height;
	GetInstance()->m_WindowName = a_WindowName;
	GetInstance()->m_ColourBits = a_ColourBits;
	GetInstance()->m_DepthBits = a_DepthBits;
	GetInstance()->m_StencilBits = a_StencilBits;

	if (a_ShouldCreateTerminal)
	{
		GetInstance()->CreateTerminal();
	}

#ifdef _MSC_VER
	GetInstance()->InitializeWin32();
#endif

#ifdef __linux__ || __GNUG__ || __GNUC__ || __clang__

	GetInstance()->m_Attributes = new GLint[5]{GLX_RGBA, GLX_DEPTH_SIZE, GetInstance()->m_DepthBits, GLX_DOUBLEBUFFER, None};
	
	GetInstance()->m_Display = XOpenDisplay(0);
	
	if(!GetInstance()->m_Display)
	{
		printf("Cannot Connect to X Server \n");
		exit(0);
	}

	for(int i = 0; i < 5; i++)
	{
		printf("%i\n", GetInstance()->m_Attributes[i]);
	}

	GetInstance()->m_VisualInfo = glXChooseVisual(GetInstance()->m_Display, 0,
		GetInstance()->m_Attributes);

	if (!GetInstance()->m_VisualInfo)
	{
		printf("No appropriate visual found\n");
		exit(0);
	}

	GetInstance()->m_SetAttributes.colormap = XCreateColormap(GetInstance()->m_Display,
		DefaultRootWindow(GetInstance()->m_Display), 
		GetInstance()->m_VisualInfo->visual, AllocNone);

	GetInstance()->m_SetAttributes.event_mask = ExposureMask | KeyPressMask;

	GetInstance()->m_Window = XCreateWindow(GetInstance()->m_Display,
		DefaultRootWindow(GetInstance()->m_Display), 0, 0, 
		GetInstance()->m_Resolution[0], GetInstance()->m_Resolution[1],
		0, GetInstance()->m_VisualInfo->depth, InputOutput, 
		GetInstance()->m_VisualInfo->visual, CWColormap | CWEventMask, 
		&GetInstance()->m_SetAttributes);

	XMapWindow(GetInstance()->m_Display, GetInstance()->m_Window);
	XStoreName(GetInstance()->m_Display, GetInstance()->m_Window,
		GetInstance()->m_WindowName);

	GetInstance()->m_Context = glXCreateContext(
		GetInstance()->m_Display, GetInstance()->m_VisualInfo,
		0, GL_TRUE);

	glXMakeCurrent(GetInstance()->m_Display,
		GetInstance()->m_Window, GetInstance()->m_Context);

	XWindowAttributes l_WindowAttributes;

	XGetWindowAttributes(GetInstance()->m_Display, GetInstance()->m_Window, &l_WindowAttributes);
	GetInstance()->m_WindowPosition[0] = l_WindowAttributes.x;
	GetInstance()->m_WindowPosition[1] = l_WindowAttributes.y;
#endif

}
#ifdef _MSC_VER
LRESULT CALLBACK Foundation_Window::WindowProcedure(HWND a_WindowHandle, UINT a_Message, WPARAM a_WordParam, LPARAM a_LongParam)
{
	if(GetInstance()->m_WindowHandle == a_WindowHandle)
	{
		printf("It's AliVe!!!");
	}

	switch (a_Message)
	{
	case WM_CREATE:
	{
		GetInstance()->m_DeviceContextHandle = GetDC(m_WindowHandle);
		InitializePixelFormat();
		GetInstance()->m_GLRenderingcontextHandle = wglCreateContext(GetInstance()->m_DeviceContextHandle);
		wglMakeCurrent(GetInstance()->m_DeviceContextHandle, GetInstance()->m_GLRenderingcontextHandle);
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
		TranslateKey(GetInstance()->m_WordParam, GetInstance()->m_LongParam, KEYSTATE_DOWN);
		break;
	}

	case WM_KEYUP:
	{
		TranslateKey(a_WordParam, a_LongParam, KEYSTATE_UP);
		break;
	}

	case WM_LBUTTONDOWN:
	{
		GetInstance()->m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_LBUTTONUP:
	{
		GetInstance()->m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONUP;
		break;
	}

	case WM_RBUTTONDOWN:
	{
		GetInstance()->m_MouseEvents[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_RBUTTONUP:
	{
		GetInstance()->m_MouseEvents[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_MBUTTONDOWN:
	{
		GetInstance()->m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_MBUTTONUP:
	{
		GetInstance()->m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONUP;
		break;
	}

	default:
	{
		return DefWindowProc(GetInstance()->m_WindowHandle, a_Message, GetInstance()->m_WordParam, GetInstance()->m_LongParam);
	}
	}
	return 0;
}

#endif

#ifdef _MSC_VER
LRESULT CALLBACK Foundation_Window::StaticWindowProcedure(HWND a_WindowHandle, UINT a_Message, WPARAM a_WordParam, LPARAM a_LongParam)
{
	return GetInstance()->WindowProcedure(a_WindowHandle, a_Message, a_WordParam, a_LongParam);
}
#endif

#ifdef _MSC_VER
void Foundation_Window::TranslateKey(WPARAM a_WordParam, LPARAM a_LongParam, bool a_KeyState)
{
	switch (a_WordParam)
	{
		case VK_ESCAPE:
		{
			GetInstance()->m_Keys[KEY_ESCAPE] = a_KeyState;
			break;
		}

		case VK_F1:
		{
			GetInstance()->m_Keys[KEY_F1] = a_KeyState;
			break;
		}

		case VK_F2:
		{
			GetInstance()->m_Keys[KEY_F2] = a_KeyState;
			break;
		}

		case VK_F3:
		{
			GetInstance()->m_Keys[KEY_F3] = a_KeyState;
			break;
		}

		case VK_F4:
		{
			GetInstance()->m_Keys[KEY_F4] = a_KeyState;
			break;
		}

		case VK_F5:
		{
			GetInstance()->m_Keys[KEY_F5] = a_KeyState;
			break;
		}

		case VK_F6:
		{
			GetInstance()->m_Keys[KEY_F6] = a_KeyState;
			break;
		}

		case VK_F7:
		{
			GetInstance()->m_Keys[KEY_F7] = a_KeyState;
			break;
		}

		case VK_F8:
		{
			GetInstance()->m_Keys[KEY_F8] = a_KeyState;
			break;
		}

		case VK_F9:
		{
			GetInstance()->m_Keys[KEY_F9] = a_KeyState;
			break;
		}

		case VK_F10:
		{
			GetInstance()->m_Keys[KEY_F10] = a_KeyState;
			break;
		}

		case VK_F11:
		{
			GetInstance()->m_Keys[KEY_F11] = a_KeyState;
			break;
		}

		case VK_F12:
		{
			GetInstance()->m_Keys[KEY_F12] = a_KeyState;
			break;
		}

		case VK_BACK:
		{
			GetInstance()->m_Keys[KEY_BACKSPACE] = a_KeyState;
			break;
		}

		case VK_TAB:
		{
			GetInstance()->m_Keys[KEY_TAB] = a_KeyState;
			break;
		}

		case VK_CAPITAL:
		{
			GetInstance()->m_Keys[KEY_CAPSLOCK] = a_KeyState;
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
			GetInstance()->m_Keys[KEY_ENTER] = a_KeyState;
			break;
		}

		case VK_PRINT:
		{
			GetInstance()->m_Keys[KEY_PRINTSCREEN] = a_KeyState;
			break;
		}

		case VK_SCROLL:
		{
			GetInstance()->m_Keys[KEY_SCROLLLOCK] = a_KeyState;
			break;
		}

		case VK_PAUSE:
		{
			GetInstance()->m_Keys[KEY_PAUSE] = a_KeyState;
			break;
		}

		case VK_INSERT:
		{
			GetInstance()->m_Keys[KEY_INSERT] = a_KeyState;
			break;
		}

		case VK_HOME:
		{
			GetInstance()->m_Keys[KEY_HOME] = a_KeyState;
			break;
		}

		case VK_DELETE:
		{
			GetInstance()->m_Keys[KEY_DELETE] = a_KeyState;
			break;
		}

		case VK_END:
		{
			GetInstance()->m_Keys[KEY_END] = a_KeyState;
			break;
		}

		case VK_DOWN:
		{
			GetInstance()->m_Keys[KEY_ARROW_DOWN] = a_KeyState;
			break;
		}

		case VK_UP:
		{
			GetInstance()->m_Keys[KEY_ARROW_UP] = a_KeyState;
			break;
		}

		case VK_LEFT:
		{
			GetInstance()->m_Keys[KEY_ARROW_LEFT] = a_KeyState;
			break;
		}

		case VK_RIGHT:
		{
			GetInstance()->m_Keys[KEY_ARROW_RIGHT] = a_KeyState;
			break;
		}

		case VK_DIVIDE:
		{
			GetInstance()->m_Keys[KEY_KEYPAD_DIVIDE] = a_KeyState;
			break;
		}

		case VK_MULTIPLY:
		{
			GetInstance()->m_Keys[KEY_KEYPAD_MULTIPLY] = a_KeyState;
			break;
		}

		case VK_SUBTRACT:
		{
			GetInstance()->m_Keys[KEY_KEYPAD_DIVIDE] = a_KeyState;
			break;
		}

		case VK_ADD:
		{
			GetInstance()->m_Keys[KEY_KEYPAD_ADD] = a_KeyState;
			break;
		}

		case VK_DECIMAL:
		{
			GetInstance()->m_Keys[KEY_KEYPAD_PERIOD] = a_KeyState;
			break;
		}

		case VK_NUMPAD0:
		{
			GetInstance()->m_Keys[KEY_KEYPAD_0] = a_KeyState;
			break;
		}

		case VK_NUMPAD1:
		{
			GetInstance()->m_Keys[KEY_KEYPAD_1] = a_KeyState;
			break;
		}

		case VK_NUMPAD2:
		{
			GetInstance()->m_Keys[KEY_KEYPAD_2] = a_KeyState;
			break;
		}

		case VK_NUMPAD3:
		{
			GetInstance()->m_Keys[KEY_KEYPAD_3] = a_KeyState;
			break;
		}

		case VK_NUMPAD4:
		{
			GetInstance()->m_Keys[KEY_KEYPAD_4] = a_KeyState;
			break;
		}

		case VK_NUMPAD5:
		{
			GetInstance()->m_Keys[KEY_KEYPAD_5] = a_KeyState;
			break;
		}

		case VK_NUMPAD6:
		{
			GetInstance()->m_Keys[KEY_KEYPAD_6] = a_KeyState;
			break;
		}

		case VK_NUMPAD7:
		{
			GetInstance()->m_Keys[KEY_KEYPAD_7] = a_KeyState;
			break;
		}

		case VK_NUMPAD8:
		{
			GetInstance()->m_Keys[KEY_KEYPAD_8] = a_KeyState;
			break;
		}

		case VK_NUMPAD9:
		{
			GetInstance()->m_Keys[KEY_KEYPAD_9] = a_KeyState;
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
	return GetInstance()->m_Keys[a_Key];
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
	if (GetInstance()->m_GLRenderingcontextHandle) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(GetInstance()->m_GLRenderingcontextHandle);
	}
	if (GetInstance()->m_PaletteHandle)
	{
		DeleteObject(GetInstance()->m_PaletteHandle);
	}
	ReleaseDC(GetInstance()->m_WindowHandle, GetInstance()->m_DeviceContextHandle);
	PostQuitMessage(0);
#endif
}

#ifdef _MSC_VER
void Foundation_Window::InitializeWin32(UINT a_Style /* = CS_OWNDC | CS_HREDRAW | CS_DROPSHADOW */, 
	WNDPROC a_WindowProcedure /* = Foundation_Window::StaticWindowProcedure */, 
	int a_ClearScreenExtra /* = 0 */, int a_WindowExtra /* = 0 */, 
	HINSTANCE a_Instance /* = GetModuleHandle(0) */, HICON a_Icon /* = LoadIcon(0 , IDI_APPLICATION)*/,
	HCURSOR a_Cursor /* = LoadCursor(0 , IDC_ARROW)*/,
	int a_ClearScreenExtra /* = 0 */, int a_WindowExtra /* = 0 */, 
	HINSTANCE a_Instance /* = GetModuleHandle(0) */, HICON a_Icon /* = LoadIcon(0 , IDI_APPLICATION)*/,
	HCURSOR a_Cursor /* = LoadCursor(0 , IDC_ARROW)*/,
	HBRUSH a_Brush /* = (HBRUSH)BLACK_BRUSH */, LPCSTR a_MenuName /* = GetInstance()->m_WindowName */, 
	LPCSTR a_ClassName /* = GetInstance()->m_WindowName */)
{
	GetInstance()->m_WindowClass.style = a_Style;
	GetInstance()->m_WindowClass.lpfnWndProc = a_WindowProcedure;
	GetInstance()->m_WindowClass.cbClsExtra = 0;
	GetInstance()->m_WindowClass.cbWndExtra = 0;
	GetInstance()->m_WindowClass.hInstance = a_Instance;
	GetInstance()->m_WindowClass.hIcon = a_Icon;
	GetInstance()->m_WindowClass.hCursor = a_Cursor;
	GetInstance()->m_WindowClass.hbrBackground = a_Brush;
	GetInstance()->m_WindowClass.lpszMenuName = a_MenuName;
	GetInstance()->m_WindowClass.lpszClassName = a_ClassName;
	RegisterClass(&GetInstance()->m_WindowClass);

	GetInstance()->m_WindowHandle =
		CreateWindow(a_ClassName, a_MenuName, a_Style, 0,
		0, Foundation_Window::GetInstance()->m_Resolution.x,
		Foundation_Window::GetInstance()->m_Resolution.y,
		0, 0, 0, 0);

	ShowWindow(GetInstance()->m_WindowHandle, true);
	UpdateWindow(GetInstance()->m_WindowHandle);
}
#endif

void Foundation_Window::Window_SwapBuffers()
{
#ifdef _MSC_VER
	SwapBuffers(GetInstance()->m_DeviceContextHandle);
#endif

#ifdef __linux__ || __GNUG__ || __GNUC__ || __clang__
	glXSwapBuffers(GetInstance()->m_Display, GetInstance()->m_Window);
#endif

}

void Foundation_Window::XTranslateKey(GLuint a_KeySym, bool a_KeyState)
{
	switch(a_KeySym)
	{
		case XK_Home:
			{
				GetInstance()->m_Keys[KEY_HOME] = a_KeyState;
				break;
			}

		case XK_Left:
			{
				GetInstance()->m_Keys[KEY_ARROW_LEFT] = a_KeyState;
				SetFullScreen(false);
				break;
			}

		case XK_Right:
			{
				GetInstance()->m_Keys[KEY_ARROW_RIGHT] = a_KeyState;
				SetFullScreen(true);
				break;
			}

		case XK_Up:
			{
				GetInstance()->m_Keys[KEY_ARROW_UP] = a_KeyState;
				break;
			}

		case XK_Down:
			{
				GetInstance()->m_Keys[KEY_ARROW_DOWN] = a_KeyState;
				break;
			}

		case XK_Page_Up:
			{
				GetInstance()->m_Keys[KEY_PAGEUP] = a_KeyState;
				break;
			}

		case XK_Page_Down:
			{
				GetInstance()->m_Keys[KEY_PAGEDOWN] = a_KeyState;
				break;
			}

		case XK_End:
			{
				GetInstance()->m_Keys[KEY_END] = a_KeyState;
				break;
			}

		case XK_Print:
			{
				GetInstance()->m_Keys[KEY_PRINTSCREEN] = a_KeyState;
				break;
			}

		case XK_Insert:
			{
				GetInstance()->m_Keys[KEY_INSERT] = a_KeyState;
				break;
			}

		case XK_Num_Lock:
			{
				GetInstance()->m_Keys[KEY_NUMLOCK] = a_KeyState;
				break;
			}

		case XK_KP_Multiply:
			{
				GetInstance()->m_Keys[KEY_KEYPAD_MULTIPLY] = a_KeyState;
				break;
			}

		case XK_KP_Add:
			{
				GetInstance()->m_Keys[KEY_KEYPAD_ADD] = a_KeyState;
				break;
			}

		case XK_KP_Subtract:
			{
				GetInstance()->m_Keys[KEY_KEYPAD_SUBTRACT] = a_KeyState;
				break;
			}

		case XK_KP_Decimal:
			{
				GetInstance()->m_Keys[KEY_KEYPAD_PERIOD] = a_KeyState;
				break;
			}

		case XK_KP_Divide:
			{
				GetInstance()->m_Keys[KEY_KEYPAD_DIVIDE] = a_KeyState;
				break;
			}

		case XK_KP_0:
			{
				GetInstance()->m_Keys[KEY_KEYPAD_0] = a_KeyState;
				break;
			}

		case XK_KP_1:
			{
				GetInstance()->m_Keys[KEY_KEYPAD_1] = a_KeyState;
				break;
			}

		case XK_KP_2:
			{
				GetInstance()->m_Keys[KEY_KEYPAD_2] = a_KeyState;
				break;
			}

		case XK_KP_3:
			{
				GetInstance()->m_Keys[KEY_KEYPAD_3] = a_KeyState;
				break;
			}

		case XK_KP_4:
			{
				GetInstance()->m_Keys[KEY_KEYPAD_4] = a_KeyState;
				break;
			}

		case XK_KP_5:
			{
				GetInstance()->m_Keys[KEY_KEYPAD_5] = a_KeyState;
				break;
			}

		case XK_KP_6:
			{
				GetInstance()->m_Keys[KEY_KEYPAD_6] = a_KeyState;
				break;
			}

		case XK_KP_7:
			{
				GetInstance()->m_Keys[KEY_KEYPAD_7] = a_KeyState;
				break;
			}

		case XK_KP_8:
			{
				GetInstance()->m_Keys[KEY_KEYPAD_8] = a_KeyState;
				break;
			}

		case XK_KP_9:
			{
				GetInstance()->m_Keys[KEY_KEYPAD_9] = a_KeyState;
				break;
			}

		case XK_F1:
			{
				GetInstance()->m_Keys[KEY_F1] = a_KeyState;
				break;
			}

		case XK_F2:
			{
				GetInstance()->m_Keys[KEY_F2] = a_KeyState;
				break;
			}

		case XK_F3:
			{
				GetInstance()->m_Keys[KEY_F3] = a_KeyState;
				break;
			}

		case XK_F4:
			{
				GetInstance()->m_Keys[KEY_F4] = a_KeyState;
				break;
			}

		case XK_F5:
			{
				GetInstance()->m_Keys[KEY_F5] = a_KeyState;
				break;
			}

		case XK_F6:
			{
				GetInstance()->m_Keys[KEY_F6] = a_KeyState;
				break;
			}

		case XK_F7:
			{
				GetInstance()->m_Keys[KEY_F7] = a_KeyState;
				break;
			}

		case XK_F8:
			{
				GetInstance()->m_Keys[KEY_F8] = a_KeyState;
				break;
			}

		case XK_F9:
			{
				GetInstance()->m_Keys[KEY_F9] = a_KeyState;
				break;
			}

		case XK_F10:
			{
				GetInstance()->m_Keys[KEY_F10] = a_KeyState;
				break;
			}

		case XK_F11:
			{
				GetInstance()->m_Keys[KEY_F11] = a_KeyState;
				break;
			}

		case XK_F12:
			{
				GetInstance()->m_Keys[KEY_F12] = a_KeyState;
				break;
			}

		case XK_Shift_L:
			{
				GetInstance()->m_Keys[KEY_LEFTSHIFT] = a_KeyState;
				break;
			}

		case XK_Shift_R:
			{
				GetInstance()->m_Keys[KEY_RIGHTSHIFT] = a_KeyState;
				break;
			}

		case XK_Control_R:
			{
				GetInstance()->m_Keys[KEY_RIGHTCONTROL] = a_KeyState;
				break;
			}

		case XK_Control_L:
			{
				GetInstance()->m_Keys[KEY_LEFTCONTROL] = a_KeyState;
				break;
			}

		case XK_Caps_Lock:
			{
				GetInstance()->m_Keys[KEY_CAPSLOCK] = a_KeyState;
				break;
			}

		case XK_Alt_L:
			{
				GetInstance()->m_Keys[KEY_LEFTALT] = a_KeyState;
				break;
			}

		case XK_Alt_R:
			{
				GetInstance()->m_Keys[KEY_RIGHTALT] = a_KeyState;
				break;
			}

		default:
			{
				break;
			}
	}

}

void Foundation_Window::SetFullScreen(bool a_FullScreenState)
{
#ifdef __linux__ || __GNUC__ || __GNUG_ || __clang__

	if(a_FullScreenState)
	{
	
	GetInstance()->m_WindowHints.m_Flags = 2;
	GetInstance()->m_WindowHints.m_Decorations = 0;

	GetInstance()->m_WindowProperty = XInternAtom(GetInstance()->m_Display,"_MOTIF_WM_HINTS", False);

	XChangeProperty(GetInstance()->m_Display,
			GetInstance()->m_Window,
			GetInstance()->m_WindowProperty,
			GetInstance()->m_WindowProperty,
			32,
			PropModeReplace,
			(unsigned char*)&GetInstance()->m_WindowHints, 5);

	XWindowChanges l_WindowChanges;	 
	l_WindowChanges.width = WidthOfScreen(XScreenOfDisplay(GetInstance()->m_Display, DefaultScreen(GetInstance()->m_Display)));
	l_WindowChanges.height = HeightOfScreen(XScreenOfDisplay(GetInstance()->m_Display, DefaultScreen(GetInstance()->m_Display)));
	l_WindowChanges.x = 0;
	l_WindowChanges.y = 0;
	XConfigureWindow(GetInstance()->m_Display, GetInstance()->m_Window, CWWidth | CWHeight | CWX | CWY, &l_WindowChanges); 
	}

	else
	{
		GetInstance()->m_WindowHints.m_Flags = 2;
		GetInstance()->m_WindowHints.m_Decorations = 3;

		GetInstance()->m_WindowProperty = XInternAtom(GetInstance()->m_Display, "_MOTIF_WM_HINTS", False);

		GLuint l_Value = 0;

		XChangeProperty(GetInstance()->m_Display,
				GetInstance()->m_Window,
				GetInstance()->m_WindowProperty,
				GetInstance()->m_WindowProperty,
				32,
				PropModeReplace,
				(unsigned char*)&l_Value, 1);

		XWindowChanges l_WindowChanges;
		l_WindowChanges.width = GetInstance()->m_Resolution[0];
		l_WindowChanges.height = GetInstance()->m_Resolution[1];
		l_WindowChanges.x = GetInstance()->m_WindowPosition[0];
		l_WindowChanges.y = GetInstance()->m_WindowPosition[1];
		XConfigureWindow(GetInstance()->m_Display, GetInstance()->m_Window, CWWidth | CWHeight | CWX | CWY, &l_WindowChanges);
		printf("%i %i \n", GetInstance()->m_WindowPosition[0], GetInstance()->m_WindowPosition[1]);
	}	
#endif  
}

void Foundation_Window::GetResolution(GLuint& a_Width, GLuint& a_Height)
{
	a_Width = GetInstance()->m_Resolution[0];
	a_Height = GetInstance()->m_Resolution[1];
}

void Foundation_Window::SetResolution(GLuint a_Width, GLuint a_Height)
{
	GetInstance()-m_Resolution[0] = a_Width;
	GetInstance()->m_Resolution[1] = a_Height;
}

Foundation_Window* Foundation_Window::m_Instance = 0;

