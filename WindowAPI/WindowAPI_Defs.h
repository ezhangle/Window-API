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
#endif

#define KEYSTATE_DOWN 1
#define KEYSTATE_UP 0
#define KEYSTATE_IDLE 2

#define KEY_ERROR -1

#define KEY_FIRST 256 + 1
#define KEY_F1 KEY_FIRST 
#define KEY_F2 KEY_FIRST + 1
#define KEY_F3 KEY_FIRST + 2
#define KEY_F4 KEY_FIRST + 3
#define KEY_F5 KEY_FIRST + 4
#define KEY_F6 KEY_FIRST + 5
#define KEY_F7 KEY_FIRST + 6
#define KEY_F8 KEY_FIRST + 7
#define KEY_F9 KEY_FIRST + 8
#define KEY_F10 KEY_FIRST + 9
#define KEY_F11 KEY_FIRST + 10
#define KEY_F12 KEY_FIRST + 11
#define KEY_CAPSLOCK KEY_FIRST + 12
#define KEY_LEFTSHIFT KEY_FIRST + 13
#define KEY_RIGHTSHIFT KEY_FIRST + 14
#define KEY_LEFTCONTROL KEY_FIRST + 15
#define KEY_RIGHTCONTROL KEY_FIRST + 16
#define KEY_LEFTWINDOW KEY_FIRST + 17
#define KEY_RIGHTWINDOW KEY_FIRST + 18
#define KEY_LEFTALT KEY_FIRST + 19
#define KEY_RIGHTALT KEY_FIRST + 20
#define KEY_ENTER KEY_FIRST + 21
#define KEY_PRINTSCREEN KEY_FIRST + 22
#define KEY_SCROLLLOCK KEY_FIRST + 23
#define KEY_NUMLOCK KEY_FIRST + 24
#define KEY_PAUSE KEY_FIRST + 25
#define KEY_INSERT KEY_FIRST + 26
#define KEY_HOME KEY_FIRST + 27
#define KEY_END KEY_FIRST + 28
#define KEY_PAGEUP KEY_FIRST + 28 
#define KEY_PAGEDOWN KEY_FIRST + 30 
#define KEY_ARROW_DOWN KEY_FIRST + 31
#define KEY_ARROW_UP KEY_FIRST + 32
#define KEY_ARROW_LEFT KEY_FIRST + 33
#define KEY_ARROW_RIGHT KEY_FIRST + 34
#define KEY_KEYPAD_DIVIDE KEY_FIRST + 35
#define KEY_KEYPAD_MULTIPLY KEY_FIRST + 36
#define KEY_KEYPAD_SUBTRACT KEY_FIRST + 37
#define KEY_KEYPAD_ADD KEY_FIRST + 38
#define KEY_KEYPAD_ENTER KEY_FIRST + 39
#define KEY_KEYPAD_PERIOD KEY_FIRST + 40
#define KEY_KEYPAD_0 KEY_FIRST + 41
#define KEY_KEYPAD_1 KEY_FIRST + 42
#define KEY_KEYPAD_2 KEY_FIRST + 43
#define KEY_KEYPAD_3 KEY_FIRST + 44
#define KEY_KEYPAD_4 KEY_FIRST + 45
#define KEY_KEYPAD_5 KEY_FIRST + 46
#define KEY_KEYPAD_6 KEY_FIRST + 47
#define KEY_KEYPAD_7 KEY_FIRST + 48
#define KEY_KEYPAD_8 KEY_FIRST + 49
#define KEY_KEYPAD_9 KEY_FIRST + 50
#define KEY_BACKSPACE KEY_FIRST + 51
#define KEY_TAB KEY_FIRST + 52
#define KEY_DELETE KEY_FIRST + 53
#define KEY_ESCAPE KEY_FIRST + 54
#define KEY_WINDOW_LEFT KEY_FIRST + 55
#define KEY_WINDOW_RIGHT KEY_FIRST + 56
#define KEY_LAST KEY_WINDOW_RIGHT

#define MOUSE_BUTTONUP 0
#define MOUSE_BUTTONDOWN 1
#define MOUSE_BUTTONIDLE 2

#define MOUSE_LEFTBUTTON 0
#define MOUSE_RIGHTBUTTON 1
#define MOUSE_MIDDLEBUTTON 2
#define MOUSE_LAST MOUSE_MIDDLEBUTTON + 1

#define MOUSE_SCROLL_DOWN 0
#define MOUSE_SCROLL_UP 1

#define WINDOWSTYLE_BARE 1
#define WINDOWSTYLE_DEFAULT 2
#define WINDOWSTYLE_POPUP 3

#define WINDOWSTATE_NORMAL 0
#define WINDOWSTATE_MAXIMIZED 1
#define WINDOWSTATE_MINIMIZED 2
#define WINDOWSTATE_FULLSCREEN 3

typedef void (*OnKeyEvent)(GLuint Key, GLboolean KeyState);
typedef void (*OnMouseButtonEvent)(GLuint Button, GLboolean ButtonState);
typedef void (*OnMouseWheelEvent)(GLuint WheelDirection);
typedef void (*OnDestroyedEvent)();
typedef void (*OnMaximizedEvent)();
typedef void (*OnMinimizedEvent)();
//typedef void (*OnRestoredEvent)(); //only really works on windows, Linux doesn't even have an atomic for it. might need to remove
typedef void (*OnFocusEvent)(GLboolean InFocus);
typedef void (*OnMovedEvent)(GLuint X, GLuint Y);
typedef void (*OnResizeEvent)(GLuint Width, GLuint Height);
typedef void (*OnMouseMoveEvent)(GLuint WindowX, GLuint WindowY, GLuint ScreenX, GLuint ScreenY);
