/**********************************************************************************************//**
 * @file	WindowAPI\WindowAPI_Defs.h
 *
 * @brief	Declares the window API definitions.
 **************************************************************************************************/
#ifndef WINDOWAPI_DEFS_H
#define WINDOWAPI_DEFS_H

#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <time.h>

#if defined(_WIN32) || defined(_WIN64)
#ifndef WIN32_LEAN_AND_MEAN
#define  WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>
#include <gl/GL.h>
#include "dependencies/wglext.h"
#endif

#if defined(__linux__) 
#include <GL/glx.h>
#include "dependencies/glxext.h"
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <string>
#endif

#define KEYSTATE_DOWN 1 /**< the key is currently up */
#define KEYSTATE_UP 0   /**< the key is currently down */

#define KEY_ERROR -1	/**< the key pressed is considered invalid */

#define KEY_FIRST 256 + 1   /**< the fist key that is not a char */
#define KEY_F1 KEY_FIRST	/**< the F1 key */
#define KEY_F2 KEY_FIRST + 1	/**< the F2 key */
#define KEY_F3 KEY_FIRST + 2	/**< the F3 key */
#define KEY_F4 KEY_FIRST + 3	/**< the F4 key */
#define KEY_F5 KEY_FIRST + 4	/**< the F5 key */
#define KEY_F6 KEY_FIRST + 5	/**< the F6 key */
#define KEY_F7 KEY_FIRST + 6	/**< the F7 key */
#define KEY_F8 KEY_FIRST + 7	/**< the F8 key */
#define KEY_F9 KEY_FIRST + 8	/**< the F9 key */
#define KEY_F10 KEY_FIRST + 9   /**< the F10 key */
#define KEY_F11 KEY_FIRST + 10  /**< the F11 key */
#define KEY_F12 KEY_FIRST + 11  /**< the F12 key */
#define KEY_CAPSLOCK KEY_FIRST + 12 /**< the CapsLock key */
#define KEY_LEFTSHIFT KEY_FIRST + 13	/**< the left Shift key */
#define KEY_RIGHTSHIFT KEY_FIRST + 14   /**< the right Shift key */
#define KEY_LEFTCONTROL KEY_FIRST + 15  /**< the left Control key */
#define KEY_RIGHTCONTROL KEY_FIRST + 16 /**< the right Control key */
#define KEY_LEFTWINDOW KEY_FIRST + 17   /**< the left Window key */
#define KEY_RIGHTWINDOW KEY_FIRST + 18  /**< the right Window key */
#define KEY_LEFTALT KEY_FIRST + 19  /**< the left Alternate key */
#define KEY_RIGHTALT KEY_FIRST + 20 /**< the right Alternate key*/
#define KEY_ENTER KEY_FIRST + 21	/**< the Enter/Return key */
#define KEY_PRINTSCREEN KEY_FIRST + 22  /**< the PrintScreen key */
#define KEY_SCROLLLOCK KEY_FIRST + 23   /**< the ScrollLock key */
#define KEY_NUMLOCK KEY_FIRST + 24  /**< the NumLock key */
#define KEY_PAUSE KEY_FIRST + 25	/**< the pause/break key */
#define KEY_INSERT KEY_FIRST + 26   /**< the insert key */
#define KEY_HOME KEY_FIRST + 27 /**< the Home key */
#define KEY_END KEY_FIRST + 28  /**< the End key */
#define KEY_PAGEUP KEY_FIRST + 28   /**< the PageUp key */
#define KEY_PAGEDOWN KEY_FIRST + 30 /**< the PageDown key */
#define KEY_ARROW_DOWN KEY_FIRST + 31   /**< the ArrowDown key */
#define KEY_ARROW_UP KEY_FIRST + 32 /**< the ArrowUp key */
#define KEY_ARROW_LEFT KEY_FIRST + 33   /**< the ArrowLeft key */
#define KEY_ARROW_RIGHT KEY_FIRST + 34  /**< the ArrowRight key */
#define KEY_KEYPAD_DIVIDE KEY_FIRST + 35	/**< the KeyPad Divide key */
#define KEY_KEYPAD_MULTIPLY KEY_FIRST + 36  /**< the Keypad Multiply key */
#define KEY_KEYPAD_SUBTRACT KEY_FIRST + 37  /**< the Keypad Subtract key */
#define KEY_KEYPAD_ADD KEY_FIRST + 38   /**< the Keypad  Add key*/
#define KEY_KEYPAD_ENTER KEY_FIRST + 39 /**< the Keypad Enter key */
#define KEY_KEYPAD_PERIOD KEY_FIRST + 40	/**< the Keypad Period/Decimal key */
#define KEY_KEYPAD_0 KEY_FIRST + 41 /**< the Keypad 0 key */
#define KEY_KEYPAD_1 KEY_FIRST + 42 /**< the Keypad 1 key */
#define KEY_KEYPAD_2 KEY_FIRST + 43 /**< the Keypad 2 key */
#define KEY_KEYPAD_3 KEY_FIRST + 44 /**< the Keypad 3 key */
#define KEY_KEYPAD_4 KEY_FIRST + 45 /**< the Keypad 4 key */
#define KEY_KEYPAD_5 KEY_FIRST + 46 /**< the Keypad 5 key */
#define KEY_KEYPAD_6 KEY_FIRST + 47 /**< the Keypad 6 key */
#define KEY_KEYPAD_7 KEY_FIRST + 48 /**< the Keypad 7 key */
#define KEY_KEYPAD_8 KEY_FIRST + 49 /**< the keypad 8 key */
#define KEY_KEYPAD_9 KEY_FIRST + 50 /**< the Keypad 9 key */
#define KEY_BACKSPACE KEY_FIRST + 51	/**< the Backspace key */
#define KEY_TAB KEY_FIRST + 52  /**< the Tab key */
#define KEY_DELETE KEY_FIRST + 53   /**< the Delete key */
#define KEY_ESCAPE KEY_FIRST + 54   /**< the Escape key */
#define KEY_LAST KEY_ESCAPE   /**< the last key to be supported */

#define MOUSE_BUTTONUP 0	/**< the mouse button is currently up */
#define MOUSE_BUTTONDOWN 1  /**< the mouse button is currently down */

#define MOUSE_LEFTBUTTON 0  /**<  the left mouse button*/
#define MOUSE_RIGHTBUTTON 1 /**< the right mouse button */
#define MOUSE_MIDDLEBUTTON 2	/**< the middle mouse button / ScrollWheel */
#define MOUSE_LAST MOUSE_MIDDLEBUTTON + 1   /**< the last mouse button to be supported */

#define MOUSE_SCROLL_DOWN 0 /**< the mouse wheel up */
#define MOUSE_SCROLL_UP 1   /**< the mouse wheel down */

#define WINDOWSTYLE_BARE 1  /**< the window has no decorators but the window border and title bar*/
#define WINDOWSTYLE_DEFAULT 2   /**< the default window style for the respective platform */
#define WINDOWSTYLE_POPUP 3 /**< the window has no decorators*/

#define WINDOWSTATE_NORMAL 0	/**< the window is in its default state */
#define WINDOWSTATE_MAXIMIZED 1 /**< the window is currently maximized */
#define WINDOWSTATE_MINIMIZED 2 /**< the window is currently minimized */
#define WINDOWSTATE_FULLSCREEN 3	/**< the window is currently full screen */

#define DECORATOR_TITLEBAR 0x01
#define DECORATOR_ICON 0x02
#define DECORATOR_BORDER 0x04
#define DECORATOR_MINIMIZEBUTTON 0x08
#define DECORATOR_MAXIMIZEBUTTON 0x010
#define DECORATOR_CLOSEBUTTON 0x20
#define DECORATOR_VERTICALSCROLLBAR 0x40
#define DECORATOR_HORIZONTALSCROLLBAR 0x80
#define DECORATOR_SIZEABLEBORDER 0x100

#define LINUX_DECORATOR_BORDER 1L << 1
#define LINUX_DECORATOR_MOVE 1L << 2
#define LINUX_DECORATOR_MINIMIZE 1L << 3
#define LINUX_DECORATOR_MAXIMIZE 1L << 4
#define LINUX_DECORATOR_CLOSE 1L << 5

#define FOUNDATION_ERROR 0
#define FOUNDATION_OKAY 1

#define ERROR_NOCONTEXT 0
#define ERROR_INVALIDWINDOWNAME 1
#define ERROR_INVALIDWINDOWINDEX 2
#define ERROR_INVALIDWINDOWSTATE 3
#define ERROR_INVALIDRESOLUTION 4
#define ERROR_INVALIDCONTEXT 5
#define ERROR_EXISTINGCONTEXT 6
#define ERROR_NOTINITIALIZED 7
#define ERROR_ALREADYINITIALIZED 8
#define ERROR_INVALIDTITLEBAR 9
#define ERROR_INVALIDEVENT 10
#define ERROR_WINDOWNOTFOUND 11
#define ERROR_INVALIDWINDOWSTYLE 12
#define ERROR_INVALIDWINDOW 13
#define ERROR_FUNCTIONNOTIMPLEMENTED 14
#define ERROR_LINUX_CANNOTCONNECTXSERVER 15
#define ERROR_LINUX_INVALIDVISUALINFO 16
#define ERROR_LINUX_CANNOTCREATEWINDOW 17
#define ERROR_LINUX_FUNCTIONNOTIMPLEMENTED 18
#define ERROR_WINDOWS_CANNOTCREATEWINDOW 19
#define ERROR_WINDOWS_CANNOTINITIALIZE 20
#define ERROR_WINDOWS_FUNCTIONNOTIMPLEMENTED 21

#define WARNING_NOTCURRENTCONTEXT 0
#define WARNING_NOGLEXTENSIONS 1

#define LINUX_FUNCTION 1
#define LINUX_DECORATOR 2



typedef void (*OnKeyEvent)(GLuint Key, GLboolean KeyState);	/**<To be called when a key event has occurred*/
typedef void (*OnMouseButtonEvent)(GLuint Button, GLboolean ButtonState);	/**<To be called when a Mouse button event has occurred*/
typedef void (*OnMouseWheelEvent)(GLuint WheelDirection);  /**<To be called when a mouse wheel event has occurred.*/
typedef void (*OnDestroyedEvent)(); /**<To be called when the window is being destroyed*/
typedef void (*OnMaximizedEvent)(); /**<To be called when the window has been maximized*/
typedef void (*OnMinimizedEvent)(); /**<To be called when the window has been minimized*/
//typedef void (*OnRestoredEvent)(); //only really works on windows, Linux doesn't even have an atomic for it. might need to remove
typedef void (*OnFocusEvent)(GLboolean InFocus);	/**<To be called when the window has gained event focus*/
typedef void (*OnMovedEvent)(GLuint X, GLuint Y);   /**<To be called when the window has been moved*/
typedef void (*OnResizeEvent)(GLuint Width, GLuint Height); /**<To be called when the window has been resized*/
typedef void (*OnMouseMoveEvent)(GLuint WindowX, GLuint WindowY, GLuint ScreenX, GLuint ScreenY);   /**<To be called when the mouse has been moved within the window*/ 

//return wether the given string is valid
static inline GLboolean IsValidString(const char* String)
{
	return (String != nullptr);
}

//return whether the given event is valid
static inline GLboolean IsValidKeyEvent(OnKeyEvent OnKeyPressed)
{
	return (OnKeyPressed != nullptr);
}
//return whether the given event is valid
static inline GLboolean IsValidMouseWheelEvent(OnMouseWheelEvent MouseWheelEvent)
{
	return (MouseWheelEvent != nullptr);
}
//return whether the given event is valid
static inline GLboolean IsValidDestroyedEvent(OnMaximizedEvent OnMaximized)
{
	return (OnMaximized != nullptr);
}
//return whether the given event is valid
static inline GLboolean IsValidFocusEvent(OnFocusEvent OnFocus)
{
	return (OnFocus != nullptr);
}
//return whether the given event is valid
static inline GLboolean IsValidMovedEvent(OnMovedEvent OnMoved)
{
	return (OnMoved != nullptr);
}
//return whether the given event is valid
static inline GLboolean IsValidMouseMoveEvent(OnMouseMoveEvent OnMouseMove)
{
	return (OnMouseMove != nullptr);
}

//print the warning message assosciated with the given warning number
static inline void PrintWarningMessage(GLuint WarningNumber)
{
	switch(WarningNumber)
	{
		case WARNING_NOGLEXTENSIONS:
			{
				printf("Warning: no OpenGL extensions available \n");
				break;
			}
				
		case WARNING_NOTCURRENTCONTEXT:
			{
				printf("Warning: window not the current OpenGL context being rendered to \n");
				break;
			}

		default:
			{
				printf("Warning: unspecified warning \n");
				break;
			}
	}
}

//print out the error associated with the given error number
static void PrintErrorMessage(GLuint ErrorNumber)
{
	switch(ErrorNumber)
	{
		case ERROR_NOCONTEXT:
		{
			printf("Error: An OpenGL context must first be created(initialize the window) \n");
			break;
		}

		case ERROR_INVALIDWINDOWNAME:
		{
			printf("Error: invald window name \n");
			break;
		}

		case ERROR_INVALIDWINDOWINDEX:
		{
			printf("Error: invalid window index \n");
			break;
		}

		case ERROR_INVALIDWINDOWSTATE:
		{
			printf("Error: invalid window state \n");
			break;
		}

		case ERROR_INVALIDRESOLUTION:
		{
			printf("Error: invalid resolution \n");
			break;
		}

		case ERROR_INVALIDCONTEXT:
		{
			printf("Error: Failed to create OpenGL context \n");
			break;
		}

		case ERROR_EXISTINGCONTEXT:
		{	
			printf("Error: context already created \n");
			break;
		}

		case ERROR_NOTINITIALIZED:
		{
			printf("Error: Window manager not initialized \n");
			break;
		}

		case ERROR_ALREADYINITIALIZED:
		{
			printf("Error: window has already been initialized \n");
			break;
		}

		case ERROR_INVALIDTITLEBAR:
		{
			printf("Error: invalid title bar name (cannot be null or nullptr) \n");
			break;
		}

		case ERROR_INVALIDEVENT:
		{
			printf("Error: invalid event callback given \n");
			break;
		}

		case ERROR_WINDOWNOTFOUND:
		{
			printf("Error: window was not found \n");
			break;
		}

		case ERROR_INVALIDWINDOWSTYLE:
		{
			printf("Error: invalid window style given \n");
			break;
		}

		case ERROR_INVALIDWINDOW:
		{
			printf("Error: invalid window given \n");
			break;
		}

		case ERROR_FUNCTIONNOTIMPLEMENTED:
		{
			printf("Error: I'm sorry but this function has not been implemented yet :( \n");
			break;
		}

		case ERROR_LINUX_CANNOTCONNECTXSERVER:
		{
			printf("Error: cannot connect to X server \n");
			break;
		}

		case ERROR_LINUX_INVALIDVISUALINFO:
		{
			printf("Error: Invalid visual information given \n");
			break;
		}

		case ERROR_LINUX_CANNOTCREATEWINDOW:
		{
			printf("Error: failed to create window \n");
			break;
		}

		case ERROR_LINUX_FUNCTIONNOTIMPLEMENTED:
		{
			printf("Error: function not implemented on linux platform yet. sorry :( \n");
			break;
		}

		case ERROR_WINDOWS_CANNOTCREATEWINDOW:
		{
			printf("Error: failed to create window \n");
			break;
		}

		case ERROR_WINDOWS_FUNCTIONNOTIMPLEMENTED:
		{
			printf("Error: function not implemented on Windows platform yet. sorry ;( \n");
			break;
		}

		default:
		{
			printf("Error: unspecified Error \n");
			break;
		}
	}
}

#endif
