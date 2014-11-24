#include "Window.h"
#include "WindowManager.h"
#if defined(CURRENT_OS_LINUX)
#include <cstring>

void FWindow::Linux_Initialize()
{
	Attributes = new GLuint[5]{GLX_RGBA, GLX_DEPTH_SIZE, DepthBits, GLX_DOUBLEBUFFER/*, GLX_STENCIL_SIZE, StencilBits, 
		GLX_RED_SIZE, ColourBits, GLX_GREEN_SIZE, ColourBits, GLX_RED_SIZE, ColourBits, GLX_ALPHA_SIZE, ColourBits/ 4*/, None};

	if (!WindowManager::GetDisplay())
	{
		printf("Cannot Connect to X Server \n");
		exit(0);
	}

	VisualInfo = glXChooseVisual(WindowManager::GetDisplay(), 0,
		Attributes);

	if (!VisualInfo)
	{
		printf("No appropriate visual found\n");
		exit(0);
	}

	SetAttributes.colormap = XCreateColormap(WindowManager::GetDisplay(),
		DefaultRootWindow(WindowManager::GetDisplay()),
		VisualInfo->visual, AllocNone);

	SetAttributes.event_mask = ExposureMask | KeyPressMask 
	   	| KeyReleaseMask | MotionNotify | ButtonPressMask | ButtonReleaseMask
	   	| FocusIn | FocusOut | Button1MotionMask | Button2MotionMask | Button3MotionMask | 
		Button4MotionMask | Button5MotionMask | PointerMotionMask | FocusChangeMask
		| VisibilityChangeMask | PropertyChangeMask | SubstructureNotifyMask;

	WindowHandle = XCreateWindow(WindowManager::GetInstance()->m_Display,
		XDefaultRootWindow(WindowManager::GetInstance()->m_Display), 0, 0,
		Resolution[0], Resolution[1],
		0, VisualInfo->depth, InputOutput,
		VisualInfo->visual, CWColormap | CWEventMask,
		&SetAttributes);

	XMapWindow(WindowManager::GetDisplay(), WindowHandle);
	XStoreName(WindowManager::GetDisplay(), WindowHandle,
		Name);

	InitializeAtomics();

	XSetWMProtocols(WindowManager::GetDisplay(),	WindowHandle, &AtomClose, GL_TRUE);	

	Linux_InitializeGL();
}

void FWindow::Linux_Shutdown()
{
	if(CurrentState == WINDOWSTATE_FULLSCREEN)
	{
		Restore();
	}

	glXDestroyContext(WindowManager::GetDisplay(), Context);
	XUnmapWindow(WindowManager::GetDisplay(), WindowHandle);
	XDestroyWindow(WindowManager::GetDisplay(), WindowHandle);
	WindowHandle = 0;
	Context = 0;
}

void FWindow::Linux_SetResolution(GLuint Width, GLuint Height)
{
	Resolution[0] = Width;
	Resolution[1] = Height;
	XResizeWindow(WindowManager::GetDisplay(),
		WindowHandle, Resolution[0], Resolution[1]);	
}

void FWindow::Linux_SetPosition(GLuint X, GLuint Y)
{
	XWindowChanges l_WindowChanges;

	l_WindowChanges.x = X;
	l_WindowChanges.y = Y;

	XConfigureWindow(
			WindowManager::GetDisplay(),
			WindowHandle, CWX | CWY, &l_WindowChanges);
}

void FWindow::Linux_SetMousePosition(GLuint X, GLuint Y)
{
	XWarpPointer(
			WindowManager::GetInstance()->m_Display,
			WindowHandle, WindowHandle,
			Position[0], Position[1],
			Resolution[0], Resolution[1],
			X, Y);
}

void FWindow::Linux_FullScreen(GLboolean NewState)
{
	XEvent l_Event;
	memset(&l_Event, 0, sizeof(l_Event));

	l_Event.xany.type = ClientMessage;
	l_Event.xclient.message_type = AtomState;
	l_Event.xclient.format = 32;
	l_Event.xclient.window = WindowHandle;
	l_Event.xclient.data.l[0] = NewState;
	l_Event.xclient.data.l[1] = AtomFullScreen;

	XSendEvent(WindowManager::GetDisplay(),
			XDefaultRootWindow(WindowManager::GetDisplay()),
			0, SubstructureNotifyMask, &l_Event);
}

void FWindow::Linux_Minimize(GLboolean NewState)
{
	if(NewState)
	{
		XIconifyWindow(WindowManager::GetDisplay(),
				WindowHandle, 0);
	}

	else
	{
		XMapWindow(WindowManager::GetDisplay(), WindowHandle);
	}
}

void FWindow::Linux_Maximize(GLboolean NewState)
{	
	XEvent l_Event;
	memset(&l_Event, 0, sizeof(l_Event));

	l_Event.xany.type = ClientMessage;
	l_Event.xclient.message_type = AtomState;
	l_Event.xclient.format = 32;
	l_Event.xclient.window = WindowHandle;
	l_Event.xclient.data.l[0] = NewState;
	l_Event.xclient.data.l[1] = AtomMaxVert;
	l_Event.xclient.data.l[2] = AtomMaxHorz;

	XSendEvent(WindowManager::GetDisplay(),
			XDefaultRootWindow(WindowManager::GetDisplay()),
			0, SubstructureNotifyMask, &l_Event);
}

void FWindow::Linux_Restore()
{
	XMapWindow(WindowManager::GetDisplay(), WindowHandle);
}

void FWindow::Linux_SetTitleBar(const char* NewTitle)
{
	XStoreName(WindowManager::GetDisplay(),
			WindowHandle, NewTitle);
}

void FWindow::Linux_Focus(GLboolean NewState)
{
	if(NewState)
	{
		XMapWindow(WindowManager::GetDisplay(), WindowHandle);
	}

	else
	{
		XUnmapWindow(WindowManager::GetDisplay(), WindowHandle);
	}
}

void FWindow::Linux_VerticalSync(GLint EnableSync)
{
	if(EXTSwapControlSupported)
	{
		SwapIntervalEXT(WindowManager::GetDisplay(), WindowHandle, EnableSync);
	}

	if(MESASwapControlSupported)
	{
		SwapIntervalMESA(EnableSync);
	}

	if(SGISwapControlSupported)
	{
		if(EnableSync < 0)
		{
			EnableSync = 0;
		}
		SwapIntervalSGI(EnableSync);
	}
}

void FWindow::Linux_InitializeGL()
{
	Context = glXCreateContext(
			WindowManager::GetDisplay(),
			VisualInfo, 0, GL_TRUE);

	glXMakeCurrent(WindowManager::GetDisplay(),
			WindowHandle, Context);

	XWindowAttributes l_Attributes;

	XGetWindowAttributes(WindowManager::GetDisplay(),
			WindowHandle, &l_Attributes);
	Position[0] = l_Attributes.x;
	Position[1] = l_Attributes.y;

	const char* l_ExtensionsAvailable = 0;

	l_ExtensionsAvailable = glXQueryExtensionsString(WindowManager::GetDisplay(), 0);

	if(!l_ExtensionsAvailable)
	{
		printf("no extensions available \n");
	}

	else
	{
		InitGLExtensions();
	}
}

void FWindow::InitializeAtomics()
{
	AtomState = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_STATE", GL_FALSE);
	AtomFullScreen = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_STATE_FULLSCREEN", GL_FALSE);
	AtomMaxHorz = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_STATE_MAXIMIZED_HORZ", GL_FALSE);
	AtomMaxVert = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_STATE_MAXIMIZED_VERT", GL_FALSE);
	AtomClose = XInternAtom(WindowManager::GetDisplay(), "WM_DELETE_WINDOW", GL_FALSE);
	AtomHidden = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_STATE_HIDDEN", GL_FALSE);
	AtomActive = XInternAtom(WindowManager::GetDisplay(), "_NET_ACTIVE_WINDOW", GL_FALSE);
	AtomDemandsAttention = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_STATE_DEMANDS_ATTENTION", GL_FALSE);	
	AtomFocused = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_STATE_FOCUSED", GL_FALSE);
}

void FWindow::Linux_InitGLExtensions()
{
	SwapIntervalEXT = nullptr;
	SwapIntervalSGI = nullptr;
	SwapIntervalMESA = nullptr;

	SwapIntervalMESA = (PFNGLXSWAPINTERVALMESAPROC)glXGetProcAddress((const GLubyte*)"glXSwapIntervalMESA");
	SwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddress((const GLubyte*)"glXSwapIntervalEXT");
	SwapIntervalSGI = (PFNGLXSWAPINTERVALSGIPROC)glXGetProcAddress((const GLubyte*)"glXSwapIntervalSGI");

	if(SwapIntervalMESA)
	{
		//printf("MESA swap interval supported\n");
		MESASwapControlSupported = GL_TRUE;
	}
	
	if(SwapIntervalEXT)
	{
		//printf("EXT swap interval supported \n");
		EXTSwapControlSupported = GL_TRUE;	
	}

	if(SwapIntervalSGI)
	{
		//printf("SGI swap interval supported \n");
		SGISwapControlSupported = GL_TRUE;
	}
}

Window FWindow::GetWindowHandle()
{
	return WindowHandle;
}
#endif
