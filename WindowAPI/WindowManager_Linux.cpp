/**********************************************************************************************//**
 * @file	WindowAPI\WindowManager_Linux.cpp
 *
 * @brief	Implements the window manager linux class.
 **************************************************************************************************/

#include "WindowManager.h"
#include "Tools.h"

#include <limits.h>
#if defined(CURRENT_OS_LINUX)

/**********************************************************************************************//**
 * @fn	FWindow* WindowManager::GetWindowByHandle(Window WindowHandle)
 *
 * @brief	Gets window by handle.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowHandle	Handle of the window.
 *
 * @return	null if it fails, else the window by handle.
 **************************************************************************************************/

FWindow* WindowManager::GetWindowByHandle(Window WindowHandle)
{
	if(GetInstance()->IsInitialized())
	{
	for (auto Iter : GetInstance()->Windows)
	{
		if (Iter->GetWindowHandle() == WindowHandle)
		{
			return Iter;
		}
	}

	return nullptr;
	}

	Foundation_Tools::PrintErrorMessage(ERROR_NOTINITIALIZED);
	return nullptr;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::Linux_Initialize()
 *
 * @brief	Linux initialize.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

GLboolean WindowManager::Linux_Initialize()
{
	GetInstance()->m_Display = XOpenDisplay(0);

	if(!GetInstance()->m_Display)
	{
		Foundation_Tools::PrintErrorMessage(ERROR_LINUX_CANNOTCONNECTXSERVER);
		return FOUNDATION_ERROR;
	}

	GetInstance()->ScreenResolution[0] = WidthOfScreen(XScreenOfDisplay(GetInstance()->m_Display, 
				DefaultScreen(GetInstance()->m_Display)));

	GetInstance()->ScreenResolution[1] = HeightOfScreen(XScreenOfDisplay(GetInstance()->m_Display,
				DefaultScreen(GetInstance()->m_Display)));

	return FOUNDATION_OKAY;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::Linux_SetMousePositionInScreen(GLuint X, GLuint Y)
 *
 * @brief	Linux set mouse position in screen.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	X	The GLuint to process.
 * @param	Y	The GLuint to process.
 **************************************************************************************************/

GLboolean WindowManager::Linux_SetMousePositionInScreen(GLuint X, GLuint Y)
{
	if(GetInstance()->IsInitialized())
	{
		XWarpPointer(GetInstance()->m_Display, None,
		   	XDefaultRootWindow(GetInstance()->m_Display), 0, 0, 
			GetScreenResolution()[0], 
			GetScreenResolution()[1], 
			X, Y);
		return FOUNDATION_OKAY;
	}

	Foundation_Tools::PrintErrorMessage(ERROR_NOTINITIALIZED);
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::Linux_Shutdown()
 *
 * @brief	Linux shutdown.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

void WindowManager::Linux_Shutdown()
{
	XCloseDisplay(GetInstance()->m_Display);
}

/**********************************************************************************************//**
 * @fn	FWindow* WindowManager::GetWindowByEvent(XEvent Event)
 *
 * @brief	Gets window by event.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	Event	The event.
 *
 * @return	null if it fails, else the window by event.
 **************************************************************************************************/

FWindow* WindowManager::GetWindowByEvent(XEvent Event)
{
	if(GetInstance()->IsInitialized())
	{
	switch(Event.type)
	{
		case Expose:
			{
				return GetWindowByHandle(Event.xexpose.window);
			}	

		case DestroyNotify:
			{
				return GetWindowByHandle(Event.xdestroywindow.window);
			}

		case CreateNotify:
			{
				return GetWindowByHandle(Event.xcreatewindow.window);
			}	

		case KeyPress:
			{
				return GetWindowByHandle(Event.xkey.window);
			}

		case KeyRelease:
			{
				return GetWindowByHandle(Event.xkey.window);
			}

		case ButtonPress:
			{
				return GetWindowByHandle(Event.xbutton.window);
			}

		case ButtonRelease:
			{
				return GetWindowByHandle(Event.xbutton.window);
			}

		case MotionNotify:
			{
				return GetWindowByHandle(Event.xmotion.window);
			}	

		case FocusIn:
			{
				return GetWindowByHandle(Event.xfocus.window);
			}

		case FocusOut:
			{
				return GetWindowByHandle(Event.xfocus.window);
			}

		case ResizeRequest:
			{
				return GetWindowByHandle(Event.xresizerequest.window);
			}

		case ConfigureNotify:
			{
				return GetWindowByHandle(Event.xconfigure.window);
			}

		case PropertyNotify:
			{
				return GetWindowByHandle(Event.xproperty.window);
			}

		case GravityNotify:
			{
				return GetWindowByHandle(Event.xgravity.window);
			}

		case ClientMessage:
			{
				return GetWindowByHandle(Event.xclient.window);
			}

		case VisibilityNotify:
			{
				return GetWindowByHandle(Event.xvisibility.window);
			}	

	default:
		{
			return nullptr;
		}
	}
	}
	Foundation_Tools::PrintErrorMessage(ERROR_NOTINITIALIZED);
	return nullptr;
}

/**********************************************************************************************//**
 * @fn	Display* WindowManager::GetDisplay()
 *
 * @brief	Gets the display.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	null if it fails, else the display.
 **************************************************************************************************/

Display* WindowManager::GetDisplay()
{
	return GetInstance()->m_Display;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::Linux_PollForEvents()
 *
 * @brief	Linux poll for events.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

GLboolean WindowManager::Linux_PollForEvents()
{
	if(GetInstance()->IsInitialized())
	{
	XNextEvent(GetInstance()->m_Display, &GetInstance()->m_Event);

	XEvent l_Event = GetInstance()->m_Event;
	FWindow* l_Window = GetWindowByEvent(l_Event);

	switch (l_Event.type)
	{
		case Expose:
		{
			break;
		}

		case DestroyNotify:
		{

			if(Foundation_Tools::IsValid(l_Window->DestroyedEvent))
			{
				l_Window->DestroyedEvent();
			}
			
			l_Window->Shutdown();		
			break;
		}

		/*case CreateNotify:
		{	
			printf("FWindow was created\n");
			l_Window->InitializeGL();

			if(Foundation_Tools::IsValid(l_Window->m_OnCreated))
			{
				l_Window->m_OnCreated();
			}

			break;
		}*/

		case KeyPress:
		{			
			GLuint l_FunctionKeysym = XKeycodeToKeysym(
					GetInstance()->m_Display, l_Event.xkey.keycode, 1);
			
			if(l_FunctionKeysym <= 255)
			{
				l_Window->Keys[l_FunctionKeysym] = KEYSTATE_DOWN;	
				if(Foundation_Tools::IsValid(l_Window->KeyEvent))
				{
					l_Window->KeyEvent(l_FunctionKeysym, KEYSTATE_DOWN);
				}
			}
			
			else
			{
				l_Window->Keys[
					Foundation_Tools::Linux_TranslateKey(l_FunctionKeysym)] = KEYSTATE_DOWN;
				
				if(Foundation_Tools::IsValid(l_Window->KeyEvent))
				{
					l_Window->KeyEvent(Foundation_Tools::Linux_TranslateKey(l_FunctionKeysym),  KEYSTATE_DOWN);
				}
			}

			break;
		}

		case KeyRelease:
		{
			GLboolean l_IsRetriggered = GL_FALSE;
			if(XEventsQueued(GetInstance()->m_Display, QueuedAfterReading))
			{
				XEvent l_NextEvent;
				XPeekEvent(GetInstance()->m_Display, &l_NextEvent);

				if(l_NextEvent.type == KeyPress && 
						l_NextEvent.xkey.time == l_Event.xkey.time && 
						l_NextEvent.xkey.keycode == l_Event.xkey.keycode)
				{
					XNextEvent(GetInstance()->m_Display, &l_Event);
					l_IsRetriggered = GL_TRUE;
				}
			}

			if(!l_IsRetriggered)
			{
				GLuint l_FunctionKeysym = XKeycodeToKeysym(GetInstance()->m_Display,
					   l_Event.xkey.keycode, 1);

				if(l_FunctionKeysym <= 255)
				{
					l_Window->Keys[l_FunctionKeysym] = KEYSTATE_UP;
					
					if(Foundation_Tools::IsValid(l_Window->KeyEvent))
					{
						l_Window->KeyEvent(l_FunctionKeysym, KEYSTATE_UP);
					}
				}

				else
				{
					l_Window->Keys[
					Foundation_Tools::Linux_TranslateKey(l_FunctionKeysym)] = KEYSTATE_UP;
					
					if(Foundation_Tools::IsValid(l_Window->KeyEvent))
					{
						l_Window->KeyEvent(Foundation_Tools::
								Linux_TranslateKey(l_FunctionKeysym), KEYSTATE_UP);
					}
				}

				if(Foundation_Tools::IsValid(l_Window->KeyEvent))
				{
					l_Window->KeyEvent(Foundation_Tools::
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
						l_Window->MouseButton[MOUSE_LEFTBUTTON] = MOUSE_BUTTONDOWN;	
						
						if(Foundation_Tools::IsValid(l_Window->MouseButtonEvent))
						{
							l_Window->MouseButtonEvent(MOUSE_LEFTBUTTON, MOUSE_BUTTONDOWN);
						}
						break;
					}

				case 2:
					{
						l_Window->MouseButton[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONDOWN;
						
						if(Foundation_Tools::IsValid(l_Window->MouseButtonEvent))
						{
							l_Window->MouseButtonEvent(MOUSE_MIDDLEBUTTON, MOUSE_BUTTONDOWN);
						}
						break;
					}

				case 3:
					{
						l_Window->MouseButton[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONDOWN;
						
						if(Foundation_Tools::IsValid(l_Window->MouseButtonEvent))
						{
							l_Window->MouseButtonEvent(MOUSE_RIGHTBUTTON, MOUSE_BUTTONDOWN);
						}
						break;
					}

				case 4:
					{
						l_Window->MouseButton[MOUSE_SCROLL_UP] = MOUSE_BUTTONDOWN;
						
						if(Foundation_Tools::IsValid(l_Window->MouseWheelEvent))
						{
							l_Window->MouseWheelEvent(MOUSE_SCROLL_UP);
						}
						break;
					}

				case 5:
					{
						l_Window->MouseButton[MOUSE_SCROLL_DOWN] = MOUSE_BUTTONDOWN;
						
						if(Foundation_Tools::IsValid(l_Window->MouseWheelEvent))
						{
							l_Window->MouseWheelEvent(MOUSE_SCROLL_DOWN);
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
						l_Window->MouseButton[MOUSE_LEFTBUTTON] = MOUSE_BUTTONUP;
						
						if(Foundation_Tools::IsValid(l_Window->MouseButtonEvent))
						{
							l_Window->MouseButtonEvent(MOUSE_LEFTBUTTON, MOUSE_BUTTONUP);
						}
						break;
					}

				case 2:
					{
						l_Window->MouseButton[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONUP;

						if(Foundation_Tools::IsValid(l_Window->MouseButtonEvent))
						{
							l_Window->MouseButtonEvent(MOUSE_MIDDLEBUTTON, MOUSE_BUTTONUP);
						}
						break;
					}

				case 3:
					{
						l_Window->MouseButton[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONUP;
						
						if(Foundation_Tools::IsValid(l_Window->MouseButtonEvent))
						{
							l_Window->MouseButtonEvent(MOUSE_RIGHTBUTTON, MOUSE_BUTTONUP);
						}
						break;
					}

				case 4:
					{
						l_Window->MouseButton[MOUSE_SCROLL_UP] = MOUSE_BUTTONDOWN;
						break;
					}

				case 5:
					{
						l_Window->MouseButton[MOUSE_SCROLL_DOWN] = MOUSE_BUTTONDOWN;
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
			l_Window->MousePosition[0] = 
				l_Event.xmotion.x;

			l_Window->MousePosition[1] = 
				l_Event.xmotion.y;

			///set the manager screen  ouse position to match the event
			GetInstance()->ScreenMousePosition[0] = l_Event.xmotion.x_root;
			GetInstance()->ScreenMousePosition[1] = l_Event.xmotion.y_root;
			
			if(Foundation_Tools::IsValid(l_Window->MouseMoveEvent))
			{
				l_Window->MouseMoveEvent(l_Event.xmotion.x, 
						l_Event.xmotion.y, l_Event.xmotion.x_root, 
						l_Event.xmotion.y_root);
			}	
			break;
		}

		//when the window goes out of focus
		case FocusOut:
		{
			l_Window->InFocus = GL_FALSE;
			if(Foundation_Tools::IsValid(l_Window->FocusEvent))
			{
				l_Window->FocusEvent(
						l_Window->InFocus);
			}
			break;
		}

		//when the window is back in focus (use to restore?)
		case FocusIn:
		{
			l_Window->InFocus = GL_TRUE;
			
			if(Foundation_Tools::IsValid(l_Window->FocusEvent))
			{
				l_Window->FocusEvent(l_Window->InFocus);
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
			if((GLuint)l_Event.xconfigure.width != l_Window->Resolution[0] 
					|| (GLuint)l_Event.xconfigure.height != l_Window->Resolution[1])
			{
				if(Foundation_Tools::IsValid(l_Window->ResizeEvent))
				{
					l_Window->ResizeEvent(l_Event.xconfigure.width, l_Event.xconfigure.height);
				}

				l_Window->Resolution[0] = l_Event.xconfigure.width;
				l_Window->Resolution[1] = l_Event.xconfigure.height;
			}

			//check if window was moved
			if((GLuint)l_Event.xconfigure.x != l_Window->Position[0]
					|| (GLuint)l_Event.xconfigure.y != l_Window->Position[1])
			{
				if(Foundation_Tools::IsValid(l_Window->MovedEvent))
				{
					l_Window->MovedEvent(l_Event.xconfigure.x, l_Event.xconfigure.y);
				}

				l_Window->Position[0] = l_Event.xconfigure.x;
				l_Window->Position[1] = l_Event.xconfigure.y;
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
			unsigned char* l_Properties = nullptr;

			XGetWindowProperty(WindowManager::GetDisplay(), l_Event.xproperty.window, 
						l_Window->AtomState, 
						0, LONG_MAX, GL_FALSE, AnyPropertyType, 
						&l_Type, &l_Format, &l_NumItems, &l_BytesAfter, 
						&l_Properties);

				if(l_Properties && (l_Format == 32))
				{
					for(GLuint l_CurrentItem = 0; l_CurrentItem < l_NumItems; l_CurrentItem++)
					{
						long l_Property = ((long*)(l_Properties))[l_CurrentItem];	

						if(l_Property == l_Window->AtomHidden)
						{
							if(Foundation_Tools::IsValid(l_Window->MinimizedEvent))
							{								
								l_Window->MinimizedEvent();
							}
						}

						if(l_Property == l_Window->AtomMaxVert ||
								l_Property == l_Window->AtomMaxVert)
						{	
							if(Foundation_Tools::IsValid(l_Window->MaximizedEvent))
							{		
								l_Window->MaximizedEvent();
							}
						}

						if(l_Property == l_Window->AtomDemandsAttention)
						{
							//printf("window demands attention \n");
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
			const char* l_AtomName = XGetAtomName(WindowManager::GetDisplay(), l_Event.xclient.message_type);
			if(Foundation_Tools::IsValid(l_AtomName))
			{
				//print the name of the Atom
				//printf("%s\n", l_AtomName);
			}

			if((Atom)l_Event.xclient.data.l[0] == l_Window->AtomClose)
			{
				//printf("window closed\n");
				l_Window->ShouldClose = GL_TRUE;
				l_Window->DestroyedEvent();
				l_Window->Shutdown();
				//XDestroyWindow(GetInstance()->m_Display, l_Event.xclient.window);
				break;
			}

			if((Atom)l_Event.xclient.data.l[1] == l_Window->AtomFullScreen)
			{
				//printf("resized window \n");
				break;
			}
			break;
		}

		/*case VisibilityNotify:
		{
			if(l_Event.xvisibility.state == VisibilityUnobscured)
			{
				//printf("window not obscured \n");
				l_Window->m_IsObscured = GL_FALSE;
			}

			else
			{
				//printf("window obscured\n");
				l_Window->m_IsObscured = GL_TRUE;
			}
		}*/

		default:
		{
			break;
		}
	}
	return FOUNDATION_OKAY;
	}
	Foundation_Tools::PrintErrorMessage(ERROR_NOTINITIALIZED);
return FOUNDATION_ERROR;

}

#endif
