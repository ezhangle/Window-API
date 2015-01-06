/**********************************************************************************************//**
 * @file	WindowAPI\Window_Linux.cpp
 *
 * @brief	Implements the window linux class.
 **************************************************************************************************/

#include "Window.h"
#include "WindowManager.h"
#if defined(CURRENT_OS_LINUX)
#include <cstring>

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::Linux_Initialize()
 *
 * @brief	Linux initialize.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	A GLboolean.
 **************************************************************************************************/

GLboolean FWindow::Linux_Initialize()
{
	Attributes = new GLuint[15]{GLX_RGBA, GLX_DEPTH_SIZE, DepthBits, GLX_DOUBLEBUFFER, GLX_STENCIL_SIZE, StencilBits, 
		GLX_RED_SIZE, ColourBits, GLX_GREEN_SIZE, ColourBits, GLX_RED_SIZE, ColourBits, GLX_ALPHA_SIZE, ColourBits, None};

	if (!WindowManager::GetDisplay())
	{
		Foundation_Tools::PrintErrorMessage(ERROR_LINUX_CANNOTCONNECTXSERVER);
		return FOUNDATION_ERROR;
	}

	VisualInfo = glXChooseVisual(WindowManager::GetDisplay(), 0,
		Attributes);

	if (!VisualInfo)
	{
		Foundation_Tools::PrintErrorMessage(ERROR_LINUX_INVALIDVISUALINFO);
		return FOUNDATION_ERROR;
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

	if(!WindowHandle)
	{
		Foundation_Tools::PrintErrorMessage(ERROR_LINUX_CANNOTCREATEWINDOW);
		return FOUNDATION_ERROR;
	}

	XMapWindow(WindowManager::GetDisplay(), WindowHandle);
	XStoreName(WindowManager::GetDisplay(), WindowHandle,
		Name);

	InitializeAtomics();

	XSetWMProtocols(WindowManager::GetDisplay(), WindowHandle, &AtomClose, GL_TRUE);	

	return Linux_InitializeGL();
}

/**********************************************************************************************//**
 * @fn	void FWindow::Linux_Shutdown()
 *
 * @brief	Linux shutdown.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * @fn	void FWindow::Linux_SetResolution(GLuint Width, GLuint Height)
 *
 * @brief	Linux set resolution.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	Width 	The width.
 * @param	Height	The height.
 **************************************************************************************************/

void FWindow::Linux_SetResolution(GLuint Width, GLuint Height)
{
	Resolution[0] = Width;
	Resolution[1] = Height;
	XResizeWindow(WindowManager::GetDisplay(),
		WindowHandle, Resolution[0], Resolution[1]);	
}

/**********************************************************************************************//**
 * @fn	void FWindow::Linux_SetPosition(GLuint X, GLuint Y)
 *
 * @brief	Linux set position.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	X	The new X position of the window in screen coordinates.
 * @param	Y	The new Y position of the window in screen coordinates.
 **************************************************************************************************/

void FWindow::Linux_SetPosition(GLuint X, GLuint Y)
{
	XWindowChanges l_WindowChanges;

	l_WindowChanges.x = X;
	l_WindowChanges.y = Y;

	XConfigureWindow(
			WindowManager::GetDisplay(),
			WindowHandle, CWX | CWY, &l_WindowChanges);
}

/**********************************************************************************************//**
 * @fn	void FWindow::Linux_SetMousePosition(GLuint X, GLuint Y)
 *
 * @brief	Linux set mouse position.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	X	The new X position of the window in screen coordinates.
 * @param	Y	The new Y position of the window in screen coordinates.
 **************************************************************************************************/

void FWindow::Linux_SetMousePosition(GLuint X, GLuint Y)
{
	XWarpPointer(
			WindowManager::GetInstance()->m_Display,
			WindowHandle, WindowHandle,
			Position[0], Position[1],
			Resolution[0], Resolution[1],
			X, Y);
}

/**********************************************************************************************//**
 * @fn	void FWindow::Linux_FullScreen(GLboolean ShouldBeFullscreen)
 *
 * @brief	Linux full screen.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	ShouldBeFullscreen	Whether the window should be in fullscreen mode.
 **************************************************************************************************/

void FWindow::Linux_FullScreen(GLboolean ShouldBeFullscreen)
{
	XEvent l_Event;
	memset(&l_Event, 0, sizeof(l_Event));

	l_Event.xany.type = ClientMessage;
	l_Event.xclient.message_type = AtomState;
	l_Event.xclient.format = 32;
	l_Event.xclient.window = WindowHandle;
	l_Event.xclient.data.l[0] = ShouldBeFullscreen;
	l_Event.xclient.data.l[1] = AtomFullScreen;

	XSendEvent(WindowManager::GetDisplay(),
			XDefaultRootWindow(WindowManager::GetDisplay()),
			0, SubstructureNotifyMask, &l_Event);
}

/**********************************************************************************************//**
 * @fn	void FWindow::Linux_Minimize(GLboolean ShouldBeMinimized)
 *
 * @brief	Linux minimize.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	ShouldBeMinimized	Whether the window should be minimized.
 **************************************************************************************************/

void FWindow::Linux_Minimize(GLboolean ShouldBeMinimized)
{
	if(ShouldBeMinimized)
	{
		XIconifyWindow(WindowManager::GetDisplay(),
				WindowHandle, 0);
	}

	else
	{
		XMapWindow(WindowManager::GetDisplay(), WindowHandle);
	}
}

/**********************************************************************************************//**
 * @fn	void FWindow::Linux_Maximize(GLboolean ShouldBeMaximized)
 *
 * @brief	Linux maximize.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	ShouldBeMaximized	State of the new.
 **************************************************************************************************/

void FWindow::Linux_Maximize(GLboolean ShouldBeMaximized)
{	
	XEvent l_Event;
	memset(&l_Event, 0, sizeof(l_Event));

	l_Event.xany.type = ClientMessage;
	l_Event.xclient.message_type = AtomState;
	l_Event.xclient.format = 32;
	l_Event.xclient.window = WindowHandle;
	l_Event.xclient.data.l[0] = ShouldBeMaximized;
	l_Event.xclient.data.l[1] = AtomMaxVert;
	l_Event.xclient.data.l[2] = AtomMaxHorz;

	XSendEvent(WindowManager::GetDisplay(),
			XDefaultRootWindow(WindowManager::GetDisplay()),
			0, SubstructureNotifyMask, &l_Event);
}

/**********************************************************************************************//**
 * @fn	void FWindow::Linux_Restore()
 *
 * @brief	Linux restore.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

void FWindow::Linux_Restore()
{
	XMapWindow(WindowManager::GetDisplay(), WindowHandle);
}

/**********************************************************************************************//**
 * @fn	void FWindow::Linux_SetTitleBar(const char* NewTitle)
 *
 * @brief	Linux set title bar.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	NewTitle	The new title bar text.
 **************************************************************************************************/

void FWindow::Linux_SetTitleBar(const char* NewTitle)
{
	XStoreName(WindowManager::GetDisplay(),
			WindowHandle, NewTitle);
}

void FWindow::Linux_SetIcon(const char* Icon, GLuint Width, GLuint Height)
{
	//XEvent Event;

	//memset(&Event, 0, sizeof(Event));

	//l_Event.xany.type = ClientMessage;
	//l_Event.xclient.message_type = AtomState;
}

/**********************************************************************************************//**
 * @fn	void FWindow::Linux_Focus(GLboolean ShouldBeInFocus)
 *
 * @brief	Linux focus.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	ShouldBeInFocus	State of the new.
 **************************************************************************************************/

void FWindow::Linux_Focus(GLboolean ShouldBeInFocus)
{
	if(InFocus)
	{
		XMapWindow(WindowManager::GetDisplay(), WindowHandle);
	}

	else
	{
		XUnmapWindow(WindowManager::GetDisplay(), WindowHandle);
	}
}

/**********************************************************************************************//**
 * @fn	void FWindow::Linux_VerticalSync(GLint EnableSync)
 *
 * @brief	Linux vertical synchronise.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	EnableSync	The enable synchronise.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * @fn	GLboolean FWindow::Linux_InitializeGL()
 *
 * @brief	Linux initialize OpenGL.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	A GLboolean.
 **************************************************************************************************/

GLboolean FWindow::Linux_InitializeGL()
{
	if(!Context)
	{
		Context = glXCreateContext(
			WindowManager::GetDisplay(),
			VisualInfo, 0, GL_TRUE);

		if(Context)
		{
			
			glXMakeCurrent(WindowManager::GetDisplay(),
			WindowHandle, Context);

			XWindowAttributes l_Attributes;

			XGetWindowAttributes(WindowManager::GetDisplay(),
			WindowHandle, &l_Attributes);
			Position[0] = l_Attributes.x;
			Position[1] = l_Attributes.y;

			const char* ExtensionsAvailable = 0;

			ExtensionsAvailable = glXQueryExtensionsString(WindowManager::GetDisplay(), 0);

			if(!ExtensionsAvailable)
			{
				Foundation_Tools::PrintWarningMessage(WARNING_NOGLEXTENSIONS);
			}

			else
			{
				InitGLExtensions();
			}
			return FOUNDATION_OKAY;
		}
	}

	else
	{
		Foundation_Tools::PrintErrorMessage(ERROR_EXISTINGCONTEXT);
		return FOUNDATION_ERROR;	
	}
}

/**********************************************************************************************//**
 * @fn	void FWindow::InitializeAtomics()
 *
 * @brief	Initializes the atomics.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

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
	AtomCardinal = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_CARDONAL", GL_FALSE);
	AtomIcon = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_ICON", GL_FALSE);
	AtomHints = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_HINTS", GL_FALSE);

	AtomWindowTypeDesktop = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_WINDOW_TYPE_DESKTOP", GL_FALSE);
 	AtomWindowTypeSplash = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_WINDOW_TYPE_SPLASH", GL_FALSE);
	AtomWindowTypeNormal = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_WINDOW_TYPE_NORMAL", GL_FALSE);

	AtomAllowedActions = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_ALLOWED_ACTIONS", GL_FALSE);

	AtomActionResize = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_ACTION_RESIZE", GL_FALSE);
	AtomActionMinimize = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_ACTION_MINIMIZE", GL_FALSE);
	AtomActionShade = XInternAtom(WindowManager::GetDisplay(), "_NEt_WM_ACTION_SHADE", GL_FALSE);
	AtomActionMaximizeHorz = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_ACTION_MAXIMIZE_HORZ", GL_FALSE);
	AtomActionMaximizeVert = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_ACTION_MAXIMIZE_VERT", GL_FALSE);
	AtomActionClose = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_ACTION_CLOSE", GL_FALSE);
}

/**********************************************************************************************//**
 * @fn	void FWindow::Linux_InitGLExtensions()
 *
 * @brief	Linux initialize OpenGL extensions.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * @fn	Window FWindow::GetWindowHandle()
 *
 * @brief	Gets window handle.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	The window handle.
 **************************************************************************************************/

Window FWindow::GetWindowHandle()
{
	return WindowHandle;
}

GLboolean FWindow::Linux_EnableDecorator(GLbitfield Decorator)
{
	if(ContextCreated)
	{
		//XChangeProperty(WindowManager::GetDisplay(), WindowHandle, AtomAllowedActions, AtomActionMinimize, 32, 
			//PropModeReplace, 	
	}

	return FOUNDATION_ERROR;


}

GLboolean FWindow::Linux_DisableDecorator(GLbitfield Decorator)
{
	return FOUNDATION_ERROR;
}
#endif
