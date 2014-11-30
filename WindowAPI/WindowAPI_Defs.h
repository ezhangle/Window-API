/**********************************************************************************************//**
 * @file	WindowAPI\WindowAPI_Defs.h
 *
 * @brief	Declares the window API definitions.
 **************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <list>

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
#define CURRENT_OS_WINDOWS  
#include <windows.h>
#include <gl/GL.h>
#include "../dependencies/wglext.h"
#endif

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)
#define CURRENT_OS_LINUX	
#include <GL/glx.h>
#include "../dependencies/glxext.h"
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

#define WINDOWSTYLE_BARE 1  /**< the window has no decorators (no minimize, maximize or close button)*/
#define WINDOWSTYLE_DEFAULT 2   /**< the default window style for the respective platform */
#define WINDOWSTYLE_POPUP 3 /**< the window has no decorators as well as no borders and title bar */

#define WINDOWSTATE_NORMAL 0	/**< the window is in its default state */
#define WINDOWSTATE_MAXIMIZED 1 /**< the window is currently maximized */
#define WINDOWSTATE_MINIMIZED 2 /**< the window is currently minimized */
#define WINDOWSTATE_FULLSCREEN 3	/**< the window is currently fullscreen */

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
