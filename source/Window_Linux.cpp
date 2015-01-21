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
	Attributes = new GLint[12]{GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, DepthBits, GLX_STENCIL_SIZE, StencilBits,
	   GLX_RED_SIZE, ColourBits, GLX_GREEN_SIZE, ColourBits, GLX_BLUE_SIZE, ColourBits, None};

	Decorators = 1;
	CurrentWindowStyle |= LINUX_DECORATOR_CLOSE | LINUX_DECORATOR_MAXIMIZE | LINUX_DECORATOR_MINIMIZE | LINUX_DECORATOR_MOVE;

	if (!WindowManager::GetDisplay())
	{
		PrintErrorMessage(ERROR_LINUX_CANNOTCONNECTXSERVER);
		exit(0);
	}

	VisualInfo = glXGetVisualFromFBConfig(WindowManager::GetDisplay(), GetBestFrameBufferConfig()); 

	//VisualInfo = glXChooseVisual(WindowManager::GetDisplay(), 0, Attributes);

	if (!VisualInfo)
	{
		PrintErrorMessage(ERROR_LINUX_INVALIDVISUALINFO);
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

	if(!WindowHandle)
	{
		PrintErrorMessage(ERROR_LINUX_CANNOTCREATEWINDOW);
		exit(0);
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

	unsigned char* Image32 = (unsigned char*)malloc(Width * Height * 4);

	//XImage* Image = XCreateImage(WindowManager::GetDisplay(), CopyFromParent, DepthBits, ZPixmap, 0, Image32,  
			//Width, Height, ColourBits, 0);


	FILE* l_File = fopen(Icon, "r");

	if(l_File == 0)
	{
		printf("file not found \n");
	}

	fclose(l_File);

	Pixmap pix = XCreatePixmap(WindowManager::GetDisplay(), XDefaultRootWindow(WindowManager::GetDisplay()),
			Width, Height, ColourBits);

	XGCValues Values;
	GC gc = XCreateGC(WindowManager::GetDisplay(), pix, 0, &Values);

	//XPutImage(WindowManager::GetDisplay(), pix, gc, Image, Width, Height, 0, 0, Width, Height);


	GLuint width, height, X_Hot, Y_Hot = 0;

	//XReadBitmapFile(Icon, &width, &height, &Data, &X_Hot, &Y_Hot);

	//printf("%i %i %i %i \n", width, height, X_Hot, Y_Hot);
	//printf("%s\n", Data);

	//XChangeProperty(WindowManager::GetDisplay(), WindowHandle, 
		//AtomIcon, AtomCardinal, 32, PropModeReplace, (unsigned char*)Image, sizeof(Image));	

	system("ls");
	//XEvent Event
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
				PrintWarningMessage(WARNING_NOGLEXTENSIONS);
			}

			else
			{
				InitGLExtensions();
			}
			ContextCreated = GL_TRUE;
			return FOUNDATION_OKAY;
		}
	}

	else
	{
		PrintErrorMessage(ERROR_EXISTINGCONTEXT);
		return FOUNDATION_ERROR;	
	}

	return FOUNDATION_ERROR;
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
	AtomCardinal = XInternAtom(WindowManager::GetDisplay(), "CARDINAL", GL_FALSE);
	AtomIcon = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_ICON", GL_FALSE);
	AtomHints = XInternAtom(WindowManager::GetDisplay(), "_MOTIF_WM_HINTS", GL_TRUE);

	AtomWindowType = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_WINDOW_TYPE", GL_FALSE);
	AtomWindowTypeDesktop = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_WINDOW_TYPE_UTILITY", GL_FALSE);
 	AtomWindowTypeSplash = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_WINDOW_TYPE_SPLASH", GL_FALSE);
	AtomWindowTypeNormal = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_WINDOW_TYPE_NORMAL", GL_FALSE);

	AtomAllowedActions = XInternAtom(WindowManager::GetDisplay(), "_NET_WM_ALLOWED_ACTIONS", GL_FALSE);
	AtomActionResize = XInternAtom(WindowManager::GetDisplay(), "WM_ACTION_RESIZE", GL_FALSE);
	AtomActionMinimize = XInternAtom(WindowManager::GetDisplay(), "_WM_ACTION_MINIMIZE", GL_FALSE);
	AtomActionShade = XInternAtom(WindowManager::GetDisplay(), "WM_ACTION_SHADE", GL_FALSE);
	AtomActionMaximizeHorz = XInternAtom(WindowManager::GetDisplay(), "_WM_ACTION_MAXIMIZE_HORZ", GL_FALSE);
	AtomActionMaximizeVert = XInternAtom(WindowManager::GetDisplay(), "_WM_ACTION_MAXIMIZE_VERT", GL_FALSE);
	AtomActionClose = XInternAtom(WindowManager::GetDisplay(), "_WM_ACTION_CLOSE", GL_FALSE);

	AtomDesktopGeometry = XInternAtom(WindowManager::GetDisplay(), "_NET_DESKTOP_GEOMETRY", GL_FALSE);
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

void FWindow::Linux_EnableDecorator(GLbitfield Decorator)
{
		if(Decorator & DECORATOR_CLOSEBUTTON)
		{
			CurrentWindowStyle |= LINUX_DECORATOR_CLOSE;
			Decorators = 1;
		}

		if(Decorator & DECORATOR_MINIMIZEBUTTON)
		{
			CurrentWindowStyle |= LINUX_DECORATOR_MINIMIZE;
			Decorators = 1;
		}

		if(Decorator & DECORATOR_MAXIMIZEBUTTON)
		{
			CurrentWindowStyle |= LINUX_DECORATOR_MAXIMIZE;
			Decorators = 1;
		}

		if(Decorator & DECORATOR_ICON)
		{
			//Linux (at least cinammon) doesnt have icons in the window. only in the taskbar icon
		}

		//just need to set it to 1 to enable all decorators that include title bar 
		if(Decorator & DECORATOR_TITLEBAR)
		{
			Decorators = 1;
		}

		if(Decorator & DECORATOR_BORDER)
		{
			Decorators = 1;
		}

		if(Decorator & DECORATOR_SIZEABLEBORDER)
		{
			Decorators = 1;
		}	

		long hints[5] = {LINUX_FUNCTION | LINUX_DECORATOR, CurrentWindowStyle, Decorators, 0, 0};

		XChangeProperty(WindowManager::GetDisplay(), WindowHandle, AtomHints, XA_ATOM, 32, 
			   PropModeReplace,(unsigned char*) hints, 5); 

		XMapWindow(WindowManager::GetDisplay(), WindowHandle);
}

void FWindow::Linux_DisableDecorator(GLbitfield Decorator)
{		
		if(Decorator & DECORATOR_CLOSEBUTTON)
		{
			//I hate doing this but it is neccessary to keep functionality going.
			GLboolean MinimizeEnabled, MaximizeEnabled;

			if(Decorator & DECORATOR_MAXIMIZEBUTTON)
			{
				MaximizeEnabled = GL_TRUE;
			}

			if(Decorator & DECORATOR_MINIMIZEBUTTON)
			{
				MinimizeEnabled = GL_TRUE;
			}

			CurrentWindowStyle &= ~LINUX_DECORATOR_CLOSE;

			if(MaximizeEnabled)
			{
				CurrentWindowStyle |= LINUX_DECORATOR_MAXIMIZE;
			}

			if(MinimizeEnabled)
			{
				CurrentWindowStyle |= LINUX_DECORATOR_MINIMIZE;
			}

			Decorators = 1;
		}

		if(Decorator & DECORATOR_MINIMIZEBUTTON)
		{
			CurrentWindowStyle &= ~LINUX_DECORATOR_MINIMIZE;
			Decorators = 1;
		}

		if(Decorator & DECORATOR_MAXIMIZEBUTTON)
		{
			GLboolean MinimizeEnabled;

			if(Decorator & DECORATOR_MINIMIZEBUTTON)
			{
				MinimizeEnabled = GL_TRUE;
			}

		  	CurrentWindowStyle &= ~LINUX_DECORATOR_MAXIMIZE;

			if(MinimizeEnabled)
			{
				CurrentWindowStyle |= LINUX_DECORATOR_MINIMIZE;
			}

			Decorators = 1;
		}

		if(Decorator & DECORATOR_ICON)
		{
			//Linux (at least cinammon) doesnt have icons in the window. only in the taskbar icon
		}

		//just need to set it to 1 to enable all decorators that include title bar 
		if(Decorator & DECORATOR_TITLEBAR)
		{
			Decorators = LINUX_DECORATOR_BORDER;
		}

		if(Decorator & DECORATOR_BORDER)
		{
			Decorators = 0;
		}

		if(Decorator & DECORATOR_SIZEABLEBORDER)
		{
			Decorators = 0;
		}	

		long hints[5] = {LINUX_FUNCTION | LINUX_DECORATOR, CurrentWindowStyle, Decorators, 0, 0};

		XChangeProperty(WindowManager::GetDisplay(), WindowHandle, AtomHints, XA_ATOM, 32, 
			   PropModeReplace,(unsigned char*) hints, 5); 

		XMapWindow(WindowManager::GetDisplay(), WindowHandle);
}

void FWindow::Linux_SetStyle(GLuint WindowStyle)
{
	switch(WindowStyle)
	{
		case WINDOWSTYLE_DEFAULT:
			{
				Decorators = (1L << 2);
				CurrentWindowStyle = LINUX_DECORATOR_MOVE | LINUX_DECORATOR_CLOSE | 
					LINUX_DECORATOR_MAXIMIZE | LINUX_DECORATOR_MINIMIZE; 
				long Hints[5] = {LINUX_FUNCTION | LINUX_DECORATOR, CurrentWindowStyle, Decorators, 0, 0};

				XChangeProperty(WindowManager::GetDisplay(), WindowHandle, AtomHints, XA_ATOM, 32, PropModeReplace, 
						(unsigned char*)Hints, 5);

				XMapWindow(WindowManager::GetDisplay(), WindowHandle);


				break;
			}

		case WINDOWSTYLE_BARE:
			{
				Decorators = (1L << 2);
				CurrentWindowStyle = (1L << 2);
				long Hints[5] = {LINUX_FUNCTION | LINUX_DECORATOR, CurrentWindowStyle, Decorators, 0, 0};

				XChangeProperty(WindowManager::GetDisplay(), WindowHandle, AtomHints, XA_ATOM, 32, PropModeReplace, 
						(unsigned char*)Hints, 5);

				XMapWindow(WindowManager::GetDisplay(), WindowHandle);


				break;
			}

		case WINDOWSTYLE_POPUP:
			{
				Decorators = 0;
				CurrentWindowStyle = (1L << 2);
				long Hints[5] = {LINUX_FUNCTION | LINUX_DECORATOR, CurrentWindowStyle, Decorators, 0, 0};

				XChangeProperty(WindowManager::GetDisplay(), WindowHandle, AtomHints, XA_ATOM, 32, PropModeReplace, 
						(unsigned char*)Hints, 5);

				XMapWindow(WindowManager::GetDisplay(), WindowHandle);

				break;
			}

			default:
			{
				PrintErrorMessage(ERROR_INVALIDWINDOWSTYLE);
				break;
			}
	}
}	

GLXFBConfig FWindow::GetBestFrameBufferConfig()
{
	const GLint VisualAttributes[]=
	{
		GLX_X_RENDERABLE, GL_TRUE,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
		GLX_RED_SIZE, ColourBits, 
		GLX_GREEN_SIZE, ColourBits,
		GLX_BLUE_SIZE, ColourBits,
		GLX_ALPHA_SIZE, ColourBits,
		GLX_DEPTH_SIZE, DepthBits,
		GLX_STENCIL_SIZE, StencilBits,
		GLX_DOUBLEBUFFER, GL_TRUE,
		None
	};

	GLint FrameBufferCount;
	GLuint BestBufferConfig, BestNumSamples = 0;
	GLXFBConfig* Configs = glXChooseFBConfig(WindowManager::GetDisplay(), 0, VisualAttributes, &FrameBufferCount);

	for(GLuint CurrentConfig = 0; CurrentConfig < FrameBufferCount; CurrentConfig++)
	{
		XVisualInfo* VisInfo = glXGetVisualFromFBConfig(WindowManager::GetDisplay(), Configs[CurrentConfig]);

		if(VisInfo)
		{
			//printf("%i %i %i\n", VisInfo->depth, VisInfo->bits_per_rgb, VisInfo->colormap_size);
			GLint Samples, SampleBuffer;
			glXGetFBConfigAttrib(WindowManager::GetDisplay(), Configs[CurrentConfig], GLX_SAMPLE_BUFFERS, &SampleBuffer);
			glXGetFBConfigAttrib(WindowManager::GetDisplay(), Configs[CurrentConfig], GLX_SAMPLES, &Samples);

			if(SampleBuffer && Samples > -1)
			{
				BestBufferConfig = CurrentConfig;
				BestNumSamples = Samples;
			}
		}

		XFree(VisInfo);
	}

	GLXFBConfig BestConfig = Configs[BestBufferConfig];

	XFree(Configs);	

	return BestConfig;
}
#endif
