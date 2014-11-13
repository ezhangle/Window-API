#include "WindowManager.h"
#include "Window_Tools.h"
#include "WindowManager_Tools.h"

#if defined(CURRENT_OS_LINUX)
F_W* F_WM::GetWindowByHandle(Window a_WindowHandle)
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

void F_WM::Linux_Initialize()
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

void F_WM::Linux_SetMousePositionInScreen(GLuint a_X, GLuint a_Y)
{
	XWarpPointer(GetInstance()->m_Display, None,
		   	XDefaultRootWindow(GetInstance()->m_Display), 0, 0, 
			GetScreenResolution()[0], 
			GetScreenResolution()[1], 
			a_X, a_Y);
}

void F_WM::Linux_Shutdown()
{
	XCloseDisplay(GetInstance()->m_Display);
}

F_W* F_WM::GetWindowByEvent(XEvent a_Event)
{
	switch(a_Event.type)
	{

		case Expose:
			{
				return GetWindowByHandle(a_Event.xexpose.window);
			}	

		case DestroyNotify:
			{
				return GetWindowByHandle(a_Event.xdestroywindow.window);
			}

		case CreateNotify:
			{
				return GetWindowByHandle(a_Event.xcreatewindow.window);
			}	

		case KeyPress:
			{
				return GetWindowByHandle(a_Event.xkey.window);
			}

		case KeyRelease:
			{
				return GetWindowByHandle(a_Event.xkey.window);
			}

		case ButtonPress:
			{
				return GetWindowByHandle(a_Event.xbutton.window);
			}

		case ButtonRelease:
			{
				return GetWindowByHandle(a_Event.xbutton.window);
			}

		case MotionNotify:
			{
				return GetWindowByHandle(a_Event.xmotion.window);
			}	

		case FocusIn:
			{
				return GetWindowByHandle(a_Event.xfocus.window);
			}

		case FocusOut:
			{
				return GetWindowByHandle(a_Event.xfocus.window);
			}

		case ResizeRequest:
			{
				return GetWindowByHandle(a_Event.xresizerequest.window);
			}

		case ConfigureNotify:
			{
				return GetWindowByHandle(a_Event.xconfigure.window);
			}

		case PropertyNotify:
			{
				return GetWindowByHandle(a_Event.xproperty.window);
			}

		case GravityNotify:
			{
				return GetWindowByHandle(a_Event.xgravity.window);
			}

		case ClientMessage:
			{
				return GetWindowByHandle(a_Event.xclient.window);
			}

		case VisibilityNotify:
			{
				return GetWindowByHandle(a_Event.xvisibility.window);
			}	

	default:
		{
			return NULL;
		}
	}

}

Display* F_WM::GetDisplay()
{
	return GetInstance()->m_Display;
}

void F_WM::Linux_PollForEvents()
{
	XNextEvent(GetInstance()->m_Display, &GetInstance()->m_Event);

	XEvent l_Event = GetInstance()->m_Event;
	F_W* l_Window = GetWindowByEvent(l_Event);

	switch (l_Event.type)
	{
		case Expose:
		{
			break;
		}

		case DestroyNotify:
		{
			if(Foundation_Tools::IsValid(l_Window->m_OnDestroyed))
			{
				l_Window->m_OnDestroyed();
			}
	
			printf("Window was destroyed\n");		
			l_Window->ShutDownWindow();
			
			break;
		}

		case CreateNotify:
		{	
			printf("Window was created\n");
			l_Window->InitializeGL();

			if(Foundation_Tools::IsValid(l_Window->m_OnCreated))
			{
				l_Window->m_OnCreated();
			}

			break;
		}

		case KeyPress:
		{			
			GLuint l_FunctionKeysym = XKeycodeToKeysym(
					GetInstance()->m_Display, l_Event.xkey.keycode, 1);
			
			if(l_FunctionKeysym <= 255)
			{
				l_Window->m_Keys[l_FunctionKeysym] = KEYSTATE_DOWN;	
				if(Foundation_Tools::IsValid(l_Window->m_OnKeyEvent))
				{
					l_Window->m_OnKeyEvent(l_FunctionKeysym, KEYSTATE_DOWN);
				}
			}
			
			else
			{
				l_Window->m_Keys[
					l_Window->Linux_TranslateKey(l_FunctionKeysym)] = KEYSTATE_DOWN;
				
				if(Foundation_Tools::IsValid(l_Window->m_OnKeyEvent))
				{
					l_Window->m_OnKeyEvent(GetWindowByEvent(l_Event)->Linux_TranslateKey(l_FunctionKeysym),  KEYSTATE_DOWN);
				}
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
						l_NextEvent.xkey.time == l_Event.xkey.time && 
						l_NextEvent.xkey.keycode == l_Event.xkey.keycode)
				{
					XNextEvent(GetInstance()->m_Display, &l_Event);
					l_IsRetriggered = true;
				}
			}

			if(!l_IsRetriggered)
			{
				GLuint l_FunctionKeysym = XKeycodeToKeysym(GetInstance()->m_Display,
					   l_Event.xkey.keycode, 1);

				if(l_FunctionKeysym <= 255)
				{
					l_Window->m_Keys[l_FunctionKeysym] = KEYSTATE_UP;
					
					if(Foundation_Tools::IsValid(l_Window->m_OnKeyEvent))
					{
						l_Window->m_OnKeyEvent(l_FunctionKeysym, KEYSTATE_UP);
					}
				}

				else
				{
					l_Window->m_Keys[
					l_Window->Linux_TranslateKey(l_FunctionKeysym)] = KEYSTATE_UP;
					
					if(Foundation_Tools::IsValid(l_Window->m_OnKeyEvent))
					{
						l_Window->m_OnKeyEvent(GetWindowByEvent(l_Event)->
								Linux_TranslateKey(l_FunctionKeysym), KEYSTATE_UP);
					}
				}

				if(Foundation_Tools::IsValid(l_Window->m_OnKeyEvent))
				{
					l_Window->m_OnKeyEvent(GetWindowByEvent(l_Event)->
							Linux_TranslateKey(l_FunctionKeysym), KEYSTATE_UP);
				}
			}

			break;	
		}

		case ButtonPress:
		{			
			switch(l_Event.xbutton.button)
			{
				case 1:
					{
						l_Window->m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONDOWN;	
						
						if(Foundation_Tools::IsValid(l_Window->m_OnMouseButtonEvent))
						{
							l_Window->m_OnMouseButtonEvent(MOUSE_LEFTBUTTON, MOUSE_BUTTONDOWN);
						}
						break;
					}

				case 2:
					{
						l_Window->m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONDOWN;
						
						if(Foundation_Tools::IsValid(l_Window->m_OnMouseButtonEvent))
						{
							l_Window->m_OnMouseButtonEvent(MOUSE_MIDDLEBUTTON, MOUSE_BUTTONDOWN);
						}
						break;
					}

				case 3:
					{
						l_Window->m_MouseEvents[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONDOWN;
						
						if(Foundation_Tools::IsValid(l_Window->m_OnMouseButtonEvent))
						{
							l_Window->m_OnMouseButtonEvent(MOUSE_RIGHTBUTTON, MOUSE_BUTTONDOWN);
						}
						break;
					}

				case 4:
					{
						l_Window->m_MouseEvents[MOUSE_SCROLL_UP] = MOUSE_BUTTONDOWN;
						
						if(Foundation_Tools::IsValid(l_Window->m_OnMouseWheel))
						{
							l_Window->m_OnMouseWheel(MOUSE_SCROLL_UP);
						}
						break;
					}

				case 5:
					{
						l_Window->m_MouseEvents[MOUSE_SCROLL_DOWN] = MOUSE_BUTTONDOWN;
						
						if(Foundation_Tools::IsValid(l_Window->m_OnMouseWheel))
						{
							l_Window->m_OnMouseWheel(MOUSE_SCROLL_DOWN);
						}
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
			switch(l_Event.xbutton.button)
			{
				case 1:
					{
						l_Window->m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONUP;
						
						if(Foundation_Tools::IsValid(l_Window->m_OnMouseButtonEvent))
						{
							l_Window->m_OnMouseButtonEvent(MOUSE_LEFTBUTTON, MOUSE_BUTTONUP);
						}
						break;
					}

				case 2:
					{
						l_Window->m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONUP;

						if(Foundation_Tools::IsValid(l_Window->m_OnMouseButtonEvent))
						{
							l_Window->m_OnMouseButtonEvent(MOUSE_MIDDLEBUTTON, MOUSE_BUTTONUP);
						}
						break;
					}

				case 3:
					{
						l_Window->m_MouseEvents[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONUP;
						
						if(Foundation_Tools::IsValid(l_Window->m_OnMouseButtonEvent))
						{
							l_Window->m_OnMouseButtonEvent(MOUSE_RIGHTBUTTON, MOUSE_BUTTONUP);
						}
						break;
					}

				case 4:
					{
						l_Window->m_MouseEvents[MOUSE_SCROLL_UP] = MOUSE_BUTTONDOWN;
						break;
					}

				case 5:
					{
						l_Window->m_MouseEvents[MOUSE_SCROLL_DOWN] = MOUSE_BUTTONDOWN;
						break;
					}

				default:
					{
						break;
					}
			}
			break;
		}

		//when the mouse/pointer device is moved
		case MotionNotify:
		{ 
			//set the windows mouse position to match the event
			l_Window->m_MousePosition[0] = 
				l_Event.xmotion.x;

			l_Window->m_MousePosition[1] = 
				l_Event.xmotion.y;

			///set the manager screen  ouse position to match the event
			GetInstance()->m_ScreenMousePosition[0] = l_Event.xmotion.x_root;
			GetInstance()->m_ScreenMousePosition[1] = l_Event.xmotion.y_root;
			
			if(Foundation_Tools::IsValid(l_Window->m_OnMouseMove))
			{
				l_Window->m_OnMouseMove(l_Event.xmotion.x, 
						l_Event.xmotion.y);
			}	
			break;
		}

		//when the window goes out of focus
		case FocusOut:
		{
			l_Window->m_InFocus = false;
			if(Foundation_Tools::IsValid(l_Window->m_OnFocus))
			{
				l_Window->m_OnFocus(
						l_Window->m_InFocus);
			}
			break;
		}

		//when the window is back in focus (use to restore?)
		case FocusIn:
		{
			l_Window->m_InFocus = true;
			
			if(Foundation_Tools::IsValid(l_Window->m_OnFocus))
			{
				l_Window->m_OnFocus(
						l_Window->m_InFocus);
			}
			break;
		}

		//when a request to resize the window is made either by 
		//dragging out the window or programmatically
		case ResizeRequest:
		{			
			glViewport(0, 0,
					l_Window->GetResolution()[0],
					l_Window->GetResolution()[1]);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			break;
		}

		//when a request to configure the window is made
		case ConfigureNotify:
		{
			glViewport(0, 0, l_Event.xconfigure.width,
						l_Event.xconfigure.height);

			//check if window was resized
			if((GLuint)l_Event.xconfigure.width != l_Window->m_Resolution[0] 
					|| (GLuint)l_Event.xconfigure.height != l_Window->m_Resolution[1])
			{
				if(Foundation_Tools::IsValid(l_Window->m_OnResize))
				{
					l_Window->m_OnResize(l_Event.xconfigure.width, l_Event.xconfigure.height);
				}

				l_Window->m_Resolution[0] = l_Event.xconfigure.width;
				l_Window->m_Resolution[1] = l_Event.xconfigure.height;
			}

			//check if window was moved
			if((GLuint)l_Event.xconfigure.x != l_Window->m_Position[0]
					|| (GLuint)l_Event.xconfigure.y != l_Window->m_Position[1])
			{
				if(Foundation_Tools::IsValid(l_Window->m_OnMoved))
				{
					l_Window->m_OnMoved(l_Event.xconfigure.x, l_Event.xconfigure.y);
				}

				l_Window->m_Position[0] = l_Event.xconfigure.x;
				l_Window->m_Position[1] = l_Event.xconfigure.y;
			}
			break;
		}

		case PropertyNotify:
		{
			//this is needed in order to read from the windows WM_STATE Atomic
			//to determine if the property notify event was caused by a client
			//iconify event(minimizing the window) or a maximise event
				
			Atom l_Type;
			GLint l_Format;
			ulong l_NumItems, l_BytesAfter;
			unsigned char* l_Properties = 0;

			XGetWindowProperty(F_WM::GetDisplay(), l_Event.xproperty.window, 
						l_Window->m_AtomicState, 
						0, LONG_MAX, false, AnyPropertyType, 
						&l_Type, &l_Format, &l_NumItems, &l_BytesAfter, 
						&l_Properties);

				if(l_Properties && (l_Format == 32))
				{
					for(GLuint l_CurrentItem = 0; l_CurrentItem < l_NumItems; l_CurrentItem++)
					{
						long l_Property = ((long*)(l_Properties))[l_CurrentItem];

						if(l_Property == l_Window->m_AtomicHidden)
						{
							if(Foundation_Tools::IsValid(l_Window->m_OnMinimized))
							{
								l_Window->m_OnMinimized(true);
							}
						}

						if(l_Property == l_Window->m_AtomicMaximizedVertical ||
								l_Property == l_Window->m_AtomicMaximizedVertical)
						{
							if(Foundation_Tools::IsValid(l_Window->m_OnMaximized))
							{
								l_Window->m_OnMaximized(true);
							}
						}
					}
				}

			break;
		}

		case GravityNotify:
		{
			//this is only supposed to pop up when the parent of this window(if any) has something happen
			//to it so that this window can react to said event as well.
			break;
		}

		case ClientMessage:
		{
			if((Atom)l_Event.xclient.data.l[1] == l_Window->m_AtomicCloseWindow)
			{
				printf("window closed\n");
				l_Window->WindowShouldClose = true;
				XDestroyWindow(GetInstance()->m_Display, l_Event.xclient.window);
				break;
			}

			if((Atom)l_Event.xclient.data.l[1] == l_Window->m_AtomicFullScreenState)
			{
				printf("resized window \n");
				break;
			}
			break;
		}

		case VisibilityNotify:
		{
			if(l_Event.xvisibility.state == VisibilityUnobscured)
			{
				//printf("window not obscured \n");
				l_Window->m_IsObscured = false;
			}

			else
			{
				//printf("window obscured\n");
				l_Window->m_IsObscured = true;
			}
		}

		default:
		{
			break;
		}
	}
}

#endif
