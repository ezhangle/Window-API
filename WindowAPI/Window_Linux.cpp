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
		| SubstructureNotifyMask | VisibilityChangeMask;

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
			m_Window, &m_ACloseWindow, 1);

	XSetWMProtocols(F_WM::GetDisplay(),
		m_Window, &m_AFullScreenState, 1);
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
	l_Event.xclient.message_type = m_AState;
	l_Event.xclient.format = 32;
	l_Event.xclient.window = m_Window;
	l_Event.xclient.data.l[0] = m_IsFullScreen;
	l_Event.xclient.data.l[1] = m_AFullScreenState;

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
	l_Event.xclient.message_type = m_AState;
	l_Event.xclient.format = 32;
	l_Event.xclient.window = m_Window;
	l_Event.xclient.data.l[0] = m_IsMaximised;
	l_Event.xclient.data.l[1] = m_AMaximizedVertical;
	l_Event.xclient.data.l[2] = m_AMaximizedHorizontal;

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

void F_W::Linux_TranslateKey(GLuint a_KeySym, bool a_KeyState)
{
	switch(a_KeySym)
	{
		case XK_Escape:
			{
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
				break;
			}

		case XK_Right:
			{
				m_Keys[KEY_ARROW_RIGHT] = a_KeyState;
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
	m_AState = XInternAtom(F_WM::GetDisplay(), "_NET_WM_STATE", False);
	m_AAddState = XInternAtom(F_WM::GetDisplay(), "_NET_WM_STATE_ADD", False);
	m_ARemoveState = XInternAtom(F_WM::GetDisplay(), "_NET_WM_STATE_REMOVE", False);
	m_AFullScreenState = XInternAtom(F_WM::GetDisplay(), "_NET_WM_STATE_FULLSCREEN", False);
	m_AMaximizedHorizontal = XInternAtom(F_WM::GetDisplay(), "_NET_WM_STATE_MAXIMIZED_HORZ", False);
	m_AMaximizedVertical = XInternAtom(F_WM::GetDisplay(), "_NET_WM_STATE_MAXIMIZED_VERT", False);
	m_AWindowMotifs = XInternAtom(F_WM::GetDisplay(), "_MOTIF_WM_MOTIFS", False);
	m_ABypassCompositor = XInternAtom(F_WM::GetDisplay(), "_NET_WM_BYPASS_COMPOSITOR", False);
	m_AActiveWindow = XInternAtom(F_WM::GetDisplay(), "_NET_ACTIVE_WINDOW", False);
	m_ACloseWindow = XInternAtom(F_WM::GetDisplay(), "_NET_CLOSE_WINDOW", False);
}

Window F_W::GetWindowHandle()
{
	return m_Window;
}
#endif
