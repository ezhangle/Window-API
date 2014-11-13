#include "Window.h"
#include "WindowManager.h"
#if defined(CURRENT_OS_LINUX)
#include <cstring>

void F_W::Linux_Initialize()
{
	m_Attributes = new GLuint[5]{GLX_RGBA, GLX_DEPTH_SIZE, m_DepthBits, GLX_DOUBLEBUFFER, None};

	if (!F_WM::GetDisplay())
	{
		printf("Cannot Connect to X Server \n");
		exit(0);
	}

	m_VisualInfo = glXChooseVisual(F_WM::GetDisplay(), 0,
		m_Attributes);

	if (!m_VisualInfo)
	{
		printf("No appropriate visual found\n");
		exit(0);
	}

	m_SetAttributes.colormap = XCreateColormap(F_WM::GetDisplay(),
		DefaultRootWindow(F_WM::GetDisplay()),
		m_VisualInfo->visual, AllocNone);

	m_SetAttributes.event_mask = ExposureMask | KeyPressMask 
	   	| KeyReleaseMask | MotionNotify | ButtonPressMask | ButtonReleaseMask
	   	| FocusIn | FocusOut | Button1MotionMask | Button2MotionMask | Button3MotionMask | 
		Button4MotionMask | Button5MotionMask | PointerMotionMask | FocusChangeMask
		| VisibilityChangeMask | PropertyChangeMask | SubstructureNotifyMask;

	m_Window = XCreateWindow(F_WM::GetInstance()->m_Display,
		XDefaultRootWindow(F_WM::GetInstance()->m_Display), 0, 0,
		m_Resolution[0], m_Resolution[1],
		0, m_VisualInfo->depth, InputOutput,
		m_VisualInfo->visual, CWColormap | CWEventMask,
		&m_SetAttributes);

	XMapWindow(F_WM::GetDisplay(), m_Window);
	XStoreName(F_WM::GetDisplay(), m_Window,
		m_WindowName);

	InitializeAtomics();

	XSetWMProtocols(F_WM::GetDisplay(), 
			m_Window, &m_AtomicCloseWindow, 1);

	XSetWMProtocols(F_WM::GetDisplay(),
			m_Window, &m_AtomicFullScreenState, 1);
}

void F_W::Linux_Shutdown()
{
	glXDestroyContext(F_WM::GetDisplay(), m_Context);
	XDestroyWindow(F_WM::GetDisplay(), m_Window);
}

void F_W::Linux_SetResolution(GLuint a_Width, GLuint a_Height)
{
	m_Resolution[0] = a_Width;
	m_Resolution[1] = a_Height;
	XResizeWindow(F_WM::GetDisplay(),
		m_Window, m_Resolution[0], m_Resolution[1]);	
}

void F_W::Linux_SetPosition(GLuint a_X, GLuint a_Y)
{
	XWindowChanges l_WindowChanges;

	l_WindowChanges.x = a_X;
	l_WindowChanges.y = a_Y;

	XConfigureWindow(
			F_WM::GetDisplay(),
			m_Window, CWX | CWY, &l_WindowChanges);
}

void F_W::Linux_SetMousePosition(GLuint a_X, GLuint a_Y)
{
	XWarpPointer(
			F_WM::GetInstance()->m_Display,
			m_Window, m_Window,
			m_Position[0], m_Position[1],
			m_Resolution[0], m_Resolution[1],
			a_X, a_Y);
}

void F_W::Linux_FullScreen(bool a_FullScreenState)
{
	m_IsFullScreen = a_FullScreenState;

	XEvent l_Event;
	memset(&l_Event, 0, sizeof(l_Event));

	l_Event.xany.type = ClientMessage;
	l_Event.xclient.message_type = m_AtomicState;
	l_Event.xclient.format = 32;
	l_Event.xclient.window = m_Window;
	l_Event.xclient.data.l[0] = m_IsFullScreen;
	l_Event.xclient.data.l[1] = m_AtomicFullScreenState;

	XSendEvent(F_WM::GetDisplay(),
			XDefaultRootWindow(F_WM::GetDisplay()),
			0, SubstructureNotifyMask, &l_Event);
}

void F_W::Linux_Minimize(bool a_MinimizeState)
{
	m_IsMinimised = a_MinimizeState;

	if(m_IsMinimised)
	{
		XIconifyWindow(F_WM::GetDisplay(),
				m_Window, 0);
	}

	else
	{
		XMapWindow(F_WM::GetDisplay(),
				m_Window);
	}
}

void F_W::Linux_Maximize(bool a_MaximizeState)
{

	m_IsMaximised = a_MaximizeState;
	
	XEvent l_Event;
	memset(&l_Event, 0, sizeof(l_Event));

	l_Event.xany.type = ClientMessage;
	l_Event.xclient.message_type = m_AtomicState;
	l_Event.xclient.format = 32;
	l_Event.xclient.window = m_Window;
	l_Event.xclient.data.l[0] = m_IsMaximised;
	l_Event.xclient.data.l[1] = m_AtomicMaximizedVertical;
	l_Event.xclient.data.l[2] = m_AtomicMaximizedHorizontal;

	XSendEvent(F_WM::GetDisplay(),
			XDefaultRootWindow(F_WM::GetDisplay()),
			0, SubstructureNotifyMask, &l_Event);
	
	if(!m_IsMaximised)
	{
		XMoveWindow(F_WM::GetDisplay(),
				m_Window, m_Position[0], m_Position[1]);
	}
}

void F_W::Linux_SetName(const char* a_NewName)
{
	m_WindowName = a_NewName;

	XStoreName(F_WM::GetDisplay(),
			m_Window, m_WindowName);
}

void F_W::Linux_Focus(bool a_FocusState)
{
	if(a_FocusState)
	{
		XMapWindow(F_WM::GetDisplay(), m_Window);
	}

	else
	{
		XUnmapWindow(F_WM::GetDisplay(), m_Window);
	}
}

GLuint F_W::Linux_TranslateKey(GLuint a_KeySym)
{
	switch(a_KeySym)
	{
		case XK_Escape:
			{
				return KEY_ESCAPE;
			}

		case XK_Home:
			{
				return KEY_HOME;
			}

		case XK_Left:
			{
				return KEY_ARROW_LEFT;
			}

		case XK_Right:
			{
				return KEY_ARROW_RIGHT;
			}

		case XK_Up:
			{
				return KEY_ARROW_UP;
			}

		case XK_Down:
			{
				return KEY_ARROW_DOWN;
			}

		case XK_Page_Up:
			{
				return KEY_PAGEUP;
			}

		case XK_Page_Down:
			{
				return KEY_PAGEDOWN;
			}

		case XK_End:
			{
				return KEY_END;
			}

		case XK_Print:
			{
				return KEY_PRINTSCREEN;
			}

		case XK_Insert:
			{
				return KEY_INSERT;
			}

		case XK_Num_Lock:
			{
				return KEY_NUMLOCK;
			}

		case XK_KP_Multiply:
			{
				return KEY_KEYPAD_MULTIPLY;
			}

		case XK_KP_Add:
			{
				return KEY_KEYPAD_ADD;
			}

		case XK_KP_Subtract:
			{
				return KEY_KEYPAD_SUBTRACT;
			}

		case XK_KP_Decimal:
			{
				return KEY_KEYPAD_PERIOD;
			}

		case XK_KP_Divide:
			{
				return KEY_KEYPAD_DIVIDE;
			}

		case XK_KP_0:
			{
				return KEY_KEYPAD_0;
			}

		case XK_KP_1:
			{
				return KEY_KEYPAD_1;
			}

		case XK_KP_2:
			{
				return KEY_KEYPAD_2;
			}

		case XK_KP_3:
			{
				return KEY_KEYPAD_3;
			}

		case XK_KP_4:
			{
				return KEY_KEYPAD_4;
			}

		case XK_KP_5:
			{
				return KEY_KEYPAD_5;
			}

		case XK_KP_6:
			{
				return KEY_KEYPAD_6;
			}

		case XK_KP_7:
			{
				return KEY_KEYPAD_7;
			}

		case XK_KP_8:
			{
				return KEY_KEYPAD_8;
			}

		case XK_KP_9:
			{
				return KEY_KEYPAD_9;
			}

		case XK_F1:
			{
				return KEY_F1;
			}

		case XK_F2:
			{
				return KEY_F2;
			}

		case XK_F3:
			{
				return KEY_F3;
			}

		case XK_F4:
			{
				return KEY_F4;
			}

		case XK_F5:
			{
				return KEY_F5;
			}

		case XK_F6:
			{
				return KEY_F6;
			}

		case XK_F7:
			{
				return KEY_F7;
			}

		case XK_F8:
			{
				return KEY_F8;
			}

		case XK_F9:
			{
				return KEY_F9;
			}

		case XK_F10:
			{
				return KEY_F10;
			}

		case XK_F11:
			{
				return KEY_F11;
			}

		case XK_F12:
			{
				return KEY_F12;
			}

		case XK_Shift_L:
			{
				return KEY_LEFTSHIFT;
			}

		case XK_Shift_R:
			{
				return KEY_RIGHTSHIFT;
			}

		case XK_Control_R:
			{
				return KEY_RIGHTCONTROL;
			}

		case XK_Control_L:
			{
				return KEY_LEFTCONTROL;
			}

		case XK_Caps_Lock:
			{
				return KEY_CAPSLOCK;
			}

		case XK_Alt_L:
			{
				return KEY_LEFTALT;
			}

		case XK_Alt_R:
			{
				return KEY_RIGHTALT;
			}

		default:
			{
				return 0;
			}
	}
}

void F_W::Linux_InitializeGL()
{
	m_Context = glXCreateContext(
			F_WM::GetDisplay(),
			m_VisualInfo, 0, GL_TRUE);

	glXMakeCurrent(F_WM::GetDisplay(),
			m_Window, m_Context);

	GLenum l_Error = glewInit();

	if(l_Error != GLEW_OK)
	{
		printf("%s\n", glewGetErrorString(l_Error));
		exit(0);
	}

	XWindowAttributes l_Attributes;

	XGetWindowAttributes(F_WM::GetDisplay(),
			m_Window, &l_Attributes);
	m_Position[0] = l_Attributes.x;
	m_Position[1] = l_Attributes.y;
}

void F_W::InitializeAtomics()
{
	m_AtomicState = XInternAtom(F_WM::GetDisplay(), "_NET_WM_STATE", False);
	m_AtomicFullScreenState = XInternAtom(F_WM::GetDisplay(), "_NET_WM_STATE_FULLSCREEN", False);
	m_AtomicMaximizedHorizontal = XInternAtom(F_WM::GetDisplay(), "_NET_WM_STATE_MAXIMIZED_HORZ", False);
	m_AtomicMaximizedVertical = XInternAtom(F_WM::GetDisplay(), "_NET_WM_STATE_MAXIMIZED_VERT", False);
	m_AtomicCloseWindow = XInternAtom(F_WM::GetDisplay(), "_NET_WM_CLOSE_WINDOW", False);
	m_AtomicHidden = XInternAtom(F_WM::GetDisplay(), "_NET_WM_STATE_HIDDEN", False);
}

Window F_W::GetWindowHandle()
{
	return m_Window;
}
#endif
