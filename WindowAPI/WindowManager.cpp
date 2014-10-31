#include "WindowManager.h"

Foundation_WindowManager::Foundation_WindowManager()
{
#if defined(__linux__)|| defined(__GNUC__) || defined(__GNUG__) || defined(__Clang__)


#endif
}

void Foundation_WindowManager::Initialize()
{
	GetInstance()->m_Display = XOpenDisplay(0);

	if(!GetInstance()->m_Display)
	{
		printf("Cannot Connect to X server\n");
		exit(0);
	}

	GetInstance()->m_ScreenResolution[0] = WidthOfScreen(XScreenOfDisplay(GetInstance()->m_Display, 
				DefaultScreen(GetInstance()->m_Display)));

	GetInstance()->m_ScreenResolution[1] = HeightOfScreen(XScreenOfDisplay(GetInstance()->m_Display,
				DefaultScreen(GetInstance()->m_Display)));
}

Foundation_WindowManager::~Foundation_WindowManager()
{
	if(!m_Windows.empty())
	{
		for(GLuint l_Iter = 0; l_Iter <= m_Windows.size(); l_Iter++)
		{
			delete m_Windows[l_Iter];
		}

		m_Windows.clear();
	}
}

Foundation_Window* Foundation_WindowManager::GetWindowByName(const char* a_WindowName)
{
	if(a_WindowName != nullptr || a_WindowName != 0)
	{
		for (GLuint l_Iter = 0; l_Iter <= GetInstance()->m_Windows.size(); l_Iter++)
		{
			if(GetInstance()->m_Windows[l_Iter]->GetWindowName() == a_WindowName)
			{
				return GetInstance()->m_Windows[l_Iter];
			}
		}

		return nullptr;	
	}

	return nullptr;
}

Foundation_Window* Foundation_WindowManager::GetWindowByIndex(GLuint a_WindowIndex)
{
	if (a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		return GetInstance()->m_Windows[a_WindowIndex];
	}

	return nullptr;
}

Foundation_WindowManager* Foundation_WindowManager::AddWindow(Foundation_Window* a_Window)
{
	GetInstance()->m_Windows.push_back(a_Window);
	a_Window->m_WindowID = GetInstance()->m_Windows.size() - 1;
	a_Window->InitializeGL();
	return m_Instance;
}

Foundation_WindowManager* Foundation_WindowManager::GetInstance()
{
	if(!Foundation_WindowManager::m_Instance)
	{
		Foundation_WindowManager::m_Instance = new Foundation_WindowManager();
		return Foundation_WindowManager::m_Instance;
	}

	else 
	{		
		return Foundation_WindowManager::m_Instance;
	}
}

GLuint Foundation_WindowManager::GetNumWindows()
{
	return GetInstance()->m_Windows.size();
}

void Foundation_WindowManager::ShutDown()
{
#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)
	for(GLuint l_CurrentWindow = 0; l_CurrentWindow < GetInstance()->m_Windows.size(); l_CurrentWindow++)
	{
		delete GetInstance()->m_Windows[l_CurrentWindow];
	}

	XCloseDisplay(GetInstance()->m_Display);

#endif
}

void Foundation_WindowManager::GetMousePositionInScreen(GLuint& a_X, GLuint& a_Y)
{
#if defined(_MSC_VER_) || defined(_WIN32) || defined(_WIN64)
	POINT l_Point;

	if (GetCursorPos(&l_Point))
	{
		a_X = l_Point.x;
		a_Y = l_Point.y;
	}

#endif

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)

	a_X = GetInstance()->m_ScreenMousePosition[0];
	a_Y = GetInstance()->m_ScreenMousePosition[1];

#endif
}

GLuint* Foundation_WindowManager::GetMousePositionInScreen()
{
	return GetInstance()->m_ScreenMousePosition;
}

void Foundation_WindowManager::SetMousePositionInScreen(GLuint a_X, GLuint a_Y)
{
#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)

	XWarpPointer(GetInstance()->m_Display, None, XDefaultRootWindow(GetInstance()->m_Display),
		   	0, 0, GetScreenResolution()[0], GetScreenResolution()[1], a_X, a_Y);


#endif
}

GLuint* Foundation_WindowManager::GetScreenResolution()
{
#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
	RECT l_Screen;
	HWND m_Desktop = GetDesktopWindow();
	GetWindowRect(m_Desktop, &l_Screen);

	GetInstance()->m_ScreenResolution[0] = l_Screen.right;
	GetInstance()->m_ScreenResolution[1] = l_Screen.bottom;
	return GetInstance()->m_ScreenResolution;

#endif

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)
	GetInstance()->m_ScreenResolution[0] = WidthOfScreen(XDefaultScreenOfDisplay(GetInstance()->m_Display));
	GetInstance()->m_ScreenResolution[1] = HeightOfScreen(XDefaultScreenOfDisplay(GetInstance()->m_Display));
	//

	return GetInstance()->m_ScreenResolution;
#endif
}

void Foundation_WindowManager::PollForEvents()
{
#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)

	GetInstance()->Win32PollForEvents();

#endif


#if defined (__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)

	GetInstance()->X11PollForEvents();
#endif
}

void Foundation_WindowManager::GetScreenResolution(GLuint& a_Width, GLuint& a_Height)
{
#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)

	RECT l_Screen;
	HWND m_Desktop = GetDesktopWindow();
	GetWindowRect(m_Desktop, &l_Screen);
	a_Width = l_Screen.right;
	a_Height = l_Screen.bottom;
#endif

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)

	a_Width = WidthOfScreen(XDefaultScreenOfDisplay(GetInstance()->m_Display));
	a_Height = HeightOfScreen(XDefaultScreenOfDisplay(GetInstance()->m_Display));


	GetInstance()->m_ScreenResolution[0] = a_Width;
	GetInstance()->m_ScreenResolution[1] = a_Height;
#endif
}

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
Foundation_Window* Foundation_WindowManager::GetWindowByHandle(HWND a_WindowHandle)
{
	for (GLuint l_Iter = 0; l_Iter < GetInstance()->m_Windows.size(); l_Iter++)
	{
		if (GetInstance()->m_Windows[l_Iter]->GetWindowHandle() == a_WindowHandle)
		{
			return GetInstance()->m_Windows[l_Iter];
		}
	}

	return nullptr;
}

LRESULT CALLBACK Foundation_WindowManager::WindowProcedure(HWND a_WindowHandle, UINT a_Message, WPARAM a_WordParam, LPARAM a_LongParam)
{
	switch (a_Message)
	{
	case WM_CREATE:
	{
		GetWindowByIndex(m_Windows.size() - 1)->m_WindowHandle = a_WindowHandle;
		GetWindowByHandle(a_WindowHandle)->m_DeviceContextHandle = GetDC(GetWindowByHandle(a_WindowHandle)->m_WindowHandle);
		GetWindowByHandle(a_WindowHandle)->InitializePixelFormat();
		GetWindowByHandle(a_WindowHandle)->m_GLRenderingcontextHandle = wglCreateContext(GetWindowByHandle(a_WindowHandle)->m_DeviceContextHandle);
		wglMakeCurrent(GetWindowByHandle(a_WindowHandle)->m_DeviceContextHandle, GetWindowByHandle(a_WindowHandle)->m_GLRenderingcontextHandle);
		//GetWindowByHandle(a_WindowHandle)->InitializeGL();
		break;
	}
	case WM_DESTROY:
	{
		GetWindowByHandle(a_WindowHandle)->WindowShouldClose = true;
		GetWindowByHandle(a_WindowHandle)->ShutDownWindow();
		break;
	}
	case WM_MOVE:
	{
		GetWindowByHandle(a_WindowHandle)->m_Position[0] = LOWORD(a_LongParam);
		GetWindowByHandle(a_WindowHandle)->m_Position[1] = HIWORD(a_LongParam);
		
		break;
	}

	case WM_SIZE:
	{
		if (GetWindowByHandle(a_WindowHandle)->m_GLRenderingcontextHandle)
		{	
			GetWindowByHandle(a_WindowHandle)->m_Resolution[0] = (GLuint)LOWORD(a_LongParam);
			GetWindowByHandle(a_WindowHandle)->m_Resolution[1] =(GLuint)HIWORD(a_LongParam);
			break;
		}
		break;
	}

	case WM_SIZING:
	{
		if (GetWindowByHandle(a_WindowHandle)->m_GLRenderingcontextHandle)
		{
			GetWindowByHandle(a_WindowHandle)->m_Resolution[0] = (GLuint)LOWORD(a_LongParam);
			GetWindowByHandle(a_WindowHandle)->m_Resolution[1] = (GLuint)HIWORD(a_LongParam);
			break;
		}
		break;
	}

	case WM_KEYDOWN:
	{
		GetWindowByHandle(a_WindowHandle)->Win32TranslateKey(a_WordParam, a_LongParam, KEYSTATE_DOWN);

		break;
	}

	case WM_KEYUP:
	{
		GetWindowByHandle(a_WindowHandle)->Win32TranslateKey(a_WordParam, a_LongParam, KEYSTATE_UP);
		break;
	}

	case WM_MOUSEMOVE:
	{
		GetWindowByHandle(a_WindowHandle)->m_MousePosition[0] = (GLuint)LOWORD(a_LongParam);
		GetWindowByHandle(a_WindowHandle)->m_MousePosition[1] = (GLuint)HIWORD(a_LongParam);
		break;
	}

	case WM_LBUTTONDOWN:
	{
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_LBUTTONUP:
	{
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONUP;
		break;
	}

	case WM_RBUTTONDOWN:
	{
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_RBUTTONUP:
	{
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_MBUTTONDOWN:
	{
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_MBUTTONUP:
	{
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONUP;
		break;
	}

	case WM_MOUSEWHEEL:
	{
		printf("%i %i\n", (GLint)HIWORD(a_WordParam), (GLint)HIWORD(a_WordParam) % WHEEL_DELTA);
		break;
	}

	case WM_MOUSELEAVE:
	{
		printf("Mouse had left");
		break;
	}

	default:
	{
		return DefWindowProc(a_WindowHandle, a_Message, a_WordParam, a_LongParam);
	}
	}
	return 0;
}

LRESULT CALLBACK Foundation_WindowManager::StaticWindowProcedure(HWND a_WindowHandle, UINT a_Message, WPARAM a_WordParam, LPARAM a_LongParam)
{
	return Foundation_WindowManager::GetInstance()->WindowProcedure(a_WindowHandle, a_Message, a_WordParam, a_LongParam);
}

void Foundation_WindowManager::Win32PollForEvents()
{
	GetMessage(&m_Message, 0, 0, 0);
	TranslateMessage(&m_Message);
	DispatchMessage(&m_Message);
}

#endif

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)
Foundation_Window* Foundation_WindowManager::GetWindowByHandle(Window a_WindowHandle)
{
	for (GLuint l_Iter = 0; l_Iter < GetInstance()->m_Windows.size(); l_Iter++)
	{
		if (GetInstance()->m_Windows[l_Iter]->GetWindowHandle() == a_WindowHandle)
		{
			return GetInstance()->m_Windows[l_Iter];
		}
	}

	return nullptr;
}

Display* Foundation_WindowManager::GetDisplay()
{
	return GetInstance()->m_Display;
}

void Foundation_WindowManager::X11PollForEvents()
{
	XNextEvent(GetInstance()->m_Display, &GetInstance()->m_Event);

	switch (GetInstance()->m_Event.type)
	{
		case Expose:
		{
			printf("window was exposed\n");
			XGetWindowAttributes(GetInstance()->m_Display,
				GetWindowByHandle(GetInstance()->m_Event.xexpose.window)->m_Window,
				&GetWindowByHandle(GetInstance()->m_Event.xexpose.window)->m_WindowAttributes);
				
			/*if(GetWindowByHandle(GetInstance()->m_Event.xexpose.window)->GetIsFullScreen())
			{
				glViewport(0, 0, 
					GetScreenResolution()[0],
					GetScreenResolution()[1]);	
			}

			else
			{
				glViewport(0, 0, 
						GetWindowByHandle(GetInstance()->m_Event.xexpose.window)->m_Resolution[0],
						GetWindowByHandle(GetInstance()->m_Event.xexpose.window)->m_Resolution[1]);
			}*/

				/*GetWindowByHandle(GetInstance()->m_Event.xexpose.window)->m_Position[0] = GetWindowByHandle(GetInstance()->m_Event.xexpose.window)->m_WindowAttributes.x;

				GetWindowByHandle(GetInstance()->m_Event.xexpose.window)->m_Position[1] = GetWindowByHandle(GetInstance()->m_Event.xexpose.window)->m_WindowAttributes.y;*/
			break;
		}

		case DestroyNotify:
		{
			GetWindowByHandle(GetInstance()->m_Event.xdestroywindow.window)->ShutDownWindow();
			printf("Window was destroyed\n");
			break;
		}

		case CreateNotify:
		{	
			printf("Window was created\n");
			GetWindowByHandle(GetInstance()->m_Event.xcreatewindow.window)->InitializeGL();
			break;
		}

		case KeyPress:
		{			

			GLuint l_FunctionKeysym = XKeycodeToKeysym(
					GetInstance()->m_Display, GetInstance()->m_Event.xkey.keycode, 1);
			
			if(l_FunctionKeysym <= 255)
			{
				GetWindowByHandle(GetInstance()->m_Event.xkey.window)->m_Keys[l_FunctionKeysym] = KEYSTATE_DOWN;
				printf("%c\n", l_FunctionKeysym);		
			}
			
			else
			{
				GetWindowByHandle(GetInstance()->m_Event.xkey.window)->Linux_TranslateKey(l_FunctionKeysym, KEYSTATE_DOWN);
			}

			break;
		}

		case KeyRelease:
		{
			bool l_IsRetriggered = false;
			if(XEventsQueued(GetInstance()->m_Display, QueuedAfterReading))
			{
				XEvent l_NextEvent;
				XPeekEvent(GetInstance()->m_Display, &l_NextEvent);

				if(l_NextEvent.type == KeyPress && 
						l_NextEvent.xkey.time == GetInstance()->m_Event.xkey.time && 
						l_NextEvent.xkey.keycode == GetInstance()->m_Event.xkey.keycode)
				{
					printf("Key was retriggered\n");
					XNextEvent(GetInstance()->m_Display, &GetInstance()->m_Event);
					l_IsRetriggered = true;
				}
			}

			if(!l_IsRetriggered)
			{
				GLuint l_FunctionKeysym = XKeycodeToKeysym(GetInstance()->m_Display,
					   GetInstance()->m_Event.xkey.keycode, 1);

				if(l_FunctionKeysym <= 255)
				{
					printf("Key %c was released", l_FunctionKeysym);
					GetWindowByHandle(GetInstance()->m_Event.xkey.window)->m_Keys[l_FunctionKeysym] = KEYSTATE_UP;
				}

				else
				{
					GetWindowByHandle(GetInstance()->m_Event.xkey.window)->Linux_TranslateKey(l_FunctionKeysym, KEYSTATE_UP);
				}
			}

			break;	
		}

		case ButtonPress:
		{
			printf("%i\n", GetInstance()->m_Event.xbutton.button);
			
			switch(GetInstance()->m_Event.xbutton.button)
			{
				case 1:
					{
						GetWindowByHandle(GetInstance()->m_Event.xbutton.window)->m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONDOWN;
						break;
					}

				case 2:
					{
						GetWindowByHandle(GetInstance()->m_Event.xbutton.window)->m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONDOWN;
						break;
					}

				case 3:
					{
						GetWindowByHandle(GetInstance()->m_Event.xbutton.window)->m_MouseEvents[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONDOWN;
						break;
					}

				case 4:
					{
						GetWindowByHandle(GetInstance()->m_Event.xbutton.window)->m_MouseEvents[MOUSE_SCROLL_UP] = MOUSE_BUTTONDOWN;
						break;
					}

				case 5:
					{
						GetWindowByHandle(GetInstance()->m_Event.xbutton.window)->m_MouseEvents[MOUSE_SCROLL_DOWN] = MOUSE_BUTTONDOWN;
						break;
					}

				default:
					{
						break;
					}
			}
			break;
		}

		case ButtonRelease:
		{
			printf("Button released %i\n", GetInstance()->m_Event.xbutton.button);
			switch(GetInstance()->m_Event.xbutton.button)
			{
				case 1:
					{
						GetWindowByHandle(GetInstance()->m_Event.xbutton.window)->m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONUP;
						break;
					}

				case 2:
					{
						GetWindowByHandle(GetInstance()->m_Event.xbutton.window)->m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONUP;
						break;
					}

				case 3:
					{
						GetWindowByHandle(GetInstance()->m_Event.xbutton.window)->m_MouseEvents[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONUP;
						break;
					}

				case 4:
					{
						GetWindowByHandle(GetInstance()->m_Event.xbutton.window)->m_MouseEvents[MOUSE_SCROLL_UP] = MOUSE_BUTTONDOWN;
						break;
					}

				case 5:
					{
						GetWindowByHandle(GetInstance()->m_Event.xbutton.window)->m_MouseEvents[MOUSE_SCROLL_DOWN] = MOUSE_BUTTONDOWN;
						break;
					}

				default:
					{
						break;
					}
			}
			break;
		}

		case MotionNotify:
		{ 
			GetWindowByHandle(GetInstance()->m_Event.xmotion.window)->m_MousePosition[0] = 
				GetInstance()->m_Event.xmotion.x;

			GetWindowByHandle(GetInstance()->m_Event.xmotion.window)->m_MousePosition[1] = 
				GetInstance()->m_Event.xmotion.y;

			GetInstance()->m_ScreenMousePosition[0] = GetInstance()->m_Event.xmotion.x_root;
			GetInstance()->m_ScreenMousePosition[1] = GetInstance()->m_Event.xmotion.y_root;
			break;
		}

		case FocusOut:
		{
			//printf("window now out of  focus");
			GetWindowByHandle(GetInstance()->m_Event.xfocus.window)->m_InFocus = false;
			break;
		}

		case FocusIn:
		{
			//printf("window now in focus");
			GetWindowByHandle(GetInstance()->m_Event.xfocus.window)->m_InFocus = true;
			break;
		}

		case ResizeRequest:
		{
			printf("window was resized\n");
			/*printf("%i %i\n",
			GetInstance()->m_Event.xresizerequest.width,
			GetInstance()->m_Event.xresizerequest.height);*/

			glViewport(0, 0,
					GetWindowByHandle(GetInstance()->m_Event.xresizerequest.window)->GetResolution()[0],
					GetWindowByHandle(GetInstance()->m_Event.xresizerequest.window)->GetResolution()[1]);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			
			break;
		}

		case ConfigureNotify:
		{
			//GetWindowByIndex(0)->m_Position[0] = GetInstance()->m_Event.xconfigure.x;

			//GetWindowByIndex(0)->m_Position[1] = GetInstance()->m_Event.xconfigure.y;
			//
			//XClearWindow(GetInstance()->m_Display, GetInstance()->m_Event.xconfigure.window);

			glViewport(0, 0, GetInstance()->m_Event.xconfigure.width,
						GetInstance()->m_Event.xconfigure.height);

		printf("%i %i %i %i\n", GetInstance()->m_Event.xconfigure.x, GetInstance()->m_Event.xconfigure.y,
				GetInstance()->m_Event.xconfigure.width, GetInstance()->m_Event.xconfigure.height);
			break;
		}

		case GravityNotify:
		{
			//window has moved
			printf("window has moved");
			break;
		}

		case ClientMessage:
		{
			printf("ClientMessage\n");
			if((Atom)GetInstance()->m_Event.xclient.data.l[1] == Foundation_WindowManager::GetWindowByHandle(GetInstance()->m_Event.xclient.window)->m_ACloseWindow)
			{
				printf("window closed\n");
				GetWindowByHandle(GetInstance()->m_Event.xclient.window)->WindowShouldClose = true;
				XDestroyWindow(GetInstance()->m_Display, GetInstance()->m_Event.xclient.window);
				break;
			}

			if((Atom)GetInstance()->m_Event.xclient.data.l[1] == GetWindowByHandle(GetInstance()->m_Event.xclient.window)->m_AFullScreenState)
			{
				printf("resized window \n");
				break;
			}
			break;
		}

		case VisibilityNotify:
		{
			if(GetInstance()->m_Event.xvisibility.state == VisibilityUnobscured)
			{
				printf("window not obscured \n");
				GetWindowByHandle(GetInstance()->m_Event.xany.window)->m_IsObscured = false;
			}

			else
			{
				printf("window obscured\n");
				GetWindowByHandle(GetInstance()->m_Event.xany.window)->m_IsObscured = true;
			}
		}

		default:
		{
			break;
		}
	}
}

#endif

Foundation_WindowManager* Foundation_WindowManager::m_Instance = 0;
