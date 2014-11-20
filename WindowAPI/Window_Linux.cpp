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

	XSetWMProtocols(F_WM::GetDisplay(),	m_Window, &m_AtomicCloseWindow, true);	

	Linux_InitializeGL();
}

void F_W::Linux_Shutdown()
{
	if(m_CurrentState == WINDOWSTATE_FULLSCREEN)
	{
		Restore();
	}

	glXDestroyContext(F_WM::GetDisplay(), m_Context);
	XUnmapWindow(F_WM::GetDisplay(), m_Window);
	XDestroyWindow(F_WM::GetDisplay(), m_Window);
	m_Window = 0;
	m_Context = 0;
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

void F_W::Linux_FullScreen(bool a_NewState)
{
	XEvent l_Event;
	memset(&l_Event, 0, sizeof(l_Event));

	l_Event.xany.type = ClientMessage;
	l_Event.xclient.message_type = m_AtomicState;
	l_Event.xclient.format = 32;
	l_Event.xclient.window = m_Window;
	l_Event.xclient.data.l[0] = a_NewState;
	l_Event.xclient.data.l[1] = m_AtomicFullScreenState;

	XSendEvent(F_WM::GetDisplay(),
			XDefaultRootWindow(F_WM::GetDisplay()),
			0, SubstructureNotifyMask, &l_Event);
}

void F_W::Linux_Minimize(bool a_NewState)
{
	if(a_NewState)
	{
		XIconifyWindow(F_WM::GetDisplay(),
				m_Window, 0);
	}

	else
	{
		XMapWindow(F_WM::GetDisplay(), m_Window);
	}
}

void F_W::Linux_Maximize(bool a_NewState)
{	
	XEvent l_Event;
	memset(&l_Event, 0, sizeof(l_Event));

	l_Event.xany.type = ClientMessage;
	l_Event.xclient.message_type = m_AtomicState;
	l_Event.xclient.format = 32;
	l_Event.xclient.window = m_Window;
	l_Event.xclient.data.l[0] = a_NewState;
	l_Event.xclient.data.l[1] = m_AtomicMaximizedVertical;
	l_Event.xclient.data.l[2] = m_AtomicMaximizedHorizontal;

	XSendEvent(F_WM::GetDisplay(),
			XDefaultRootWindow(F_WM::GetDisplay()),
			0, SubstructureNotifyMask, &l_Event);
}

void F_W::Linux_Restore()
{
	XMapWindow(F_WM::GetDisplay(), m_Window);
}

void F_W::Linux_SetName(const char* a_NewName)
{
	m_WindowName = a_NewName;

	XStoreName(F_WM::GetDisplay(),
			m_Window, m_WindowName);
}

void F_W::Linux_Focus(bool a_NewState)
{
	if(a_NewState)
	{
		XMapWindow(F_WM::GetDisplay(), m_Window);
	}

	else
	{
		XUnmapWindow(F_WM::GetDisplay(), m_Window);
	}
}

void F_W::Linux_SetSwapInterval(GLint a_EnableSync)
{
	if(EXTSwapControlSupported)
	{
		SwapIntervalEXT(F_WM::GetDisplay(), m_Window, a_EnableSync);
	}

	if(MESASwapControlSupported)
	{
		SwapIntervalMESA(a_EnableSync);
	}

	if(SGISwapControlSupported)
	{
		if(a_EnableSync < 0)
		{
			a_EnableSync = 0;
		}
		SwapIntervalSGI(a_EnableSync);
	}
}



void F_W::Linux_InitializeGL()
{
	m_Context = glXCreateContext(
			F_WM::GetDisplay(),
			m_VisualInfo, 0, GL_TRUE);

	glXMakeCurrent(F_WM::GetDisplay(),
			m_Window, m_Context);

	XWindowAttributes l_Attributes;

	XGetWindowAttributes(F_WM::GetDisplay(),
			m_Window, &l_Attributes);
	m_Position[0] = l_Attributes.x;
	m_Position[1] = l_Attributes.y;

	const char* l_ExtensionAvailable = 0;

	l_ExtensionAvailable = glXQueryExtensionsString(F_WM::GetDisplay(), 0);

	if(l_ExtensionAvailable)
	{
		printf("%s \n", l_ExtensionAvailable);
	}

	else
	{
		printf("no extensions wtf?\n");
	}

	InitGLExtensions();
}

void F_W::InitializeAtomics()
{
	m_AtomicState = XInternAtom(F_WM::GetDisplay(), "_NET_WM_STATE", False);
	m_AtomicFullScreenState = XInternAtom(F_WM::GetDisplay(), "_NET_WM_STATE_FULLSCREEN", False);
	m_AtomicMaximizedHorizontal = XInternAtom(F_WM::GetDisplay(), "_NET_WM_STATE_MAXIMIZED_HORZ", False);
	m_AtomicMaximizedVertical = XInternAtom(F_WM::GetDisplay(), "_NET_WM_STATE_MAXIMIZED_VERT", False);
	m_AtomicCloseWindow = XInternAtom(F_WM::GetDisplay(), "WM_DELETE_WINDOW", False);
	m_AtomicHidden = XInternAtom(F_WM::GetDisplay(), "_NET_WM_STATE_HIDDEN", False);
	m_AtomActiveWindow = XInternAtom(F_WM::GetDisplay(), "_NET_ACTIVE_WINDOW", False);
	m_AtomDemandsAttention = XInternAtom(F_WM::GetDisplay(), "_NET_WM_STATE_DEMANDS_ATTENTION", False);	
	m_AtomFocused = XInternAtom(F_WM::GetDisplay(), "_NET_WM_STATE_FOCUSED", False);
}

void F_W::Linux_InitGLExtensions()
{
	printf("initializing GL extensions\n");
	SwapIntervalEXT = nullptr;
	SwapIntervalSGI = nullptr;
	SwapIntervalMESA = nullptr;

	SwapIntervalMESA = (PFNGLXSWAPINTERVALMESAPROC)glXGetProcAddress((const GLubyte*)"glXSwapIntervalMESA");
	SwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddress((const GLubyte*)"glXSwapIntervalEXT");
	SwapIntervalSGI = (PFNGLXSWAPINTERVALSGIPROC)glXGetProcAddress((const GLubyte*)"glXSwapIntervalSGI");

	if(SwapIntervalMESA)
	{
		printf("MESA swap interval supported\n");
		MESASwapControlSupported = GL_TRUE;
	}
	
	if(SwapIntervalEXT)
	{
		printf("EXT swap interval supported \n");
		EXTSwapControlSupported = GL_TRUE;	
	}

	if(SwapIntervalSGI)
	{
		printf("SGI swap interval supported \n");
		SGISwapControlSupported = GL_TRUE;
	}
}

Window F_W::GetWindowHandle()
{
	return m_Window;
}
#endif
