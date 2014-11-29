/**********************************************************************************************//**
 * @file	WindowAPI\Tools_Linux.cpp
 *
 * @brief	Implements the tools linux class.
 **************************************************************************************************/

#include "Tools.h"
#if defined(CURRENT_OS_LINUX)

/**********************************************************************************************//**
 * @fn	const char* Foundation_Tools::Linux_GetEventType(XEvent Event)
 *
 * @brief	Linux get event type.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	Event	The event.
 *
 * @return	null if it fails, else a char*.
 **************************************************************************************************/

const char* Foundation_Tools::Linux_GetEventType(XEvent Event)
{
	switch (Event.type)
	{
		case MotionNotify:
		{
			return "Motion Notify Event\n";
		}

		case ButtonPress:
		{
			return "Button Press Event\n";
		}

		case ButtonRelease:
		{
			return "Button Release Event\n";
		}

		case ColormapNotify:
		{
			return "Color Map Notify event \n";
		}

		case EnterNotify:
		{
			return "Enter Notify Event\n";
		}

		case LeaveNotify:
		{
			return "Leave Notify Event\n";
		}

		case Expose:
		{
			return "Expose Event\n";
		}

		case GraphicsExpose:
		{
			return "Graphics expose event\n";
		}

		case NoExpose:
		{
			return "No Expose Event\n";
		}

		case FocusIn:
		{
			return "Focus In Event\n";
		}

		case FocusOut:
		{
			return "Focus Out Event\n";
		}

		case KeymapNotify:
		{
			return "Key Map Notify Event\n";
		}

		case KeyPress:
		{
			return "Key Press Event\n";
		}

		case KeyRelease:
		{
			return "Key Release Event\n";
		}

		case PropertyNotify:
		{
			return "Property Notify Event\n";
		}

		case ResizeRequest:
		{
			return "Resize Property Event\n";
		}

		case CirculateNotify:
		{
			return "Circulate Notify Event\n";
		}

		case ConfigureNotify:
		{
			return "configure Notify Event\n";
		}

		case DestroyNotify:
		{
			return "Destroy Notify Request\n";
		}

		case GravityNotify:
		{
			return "Gravity Notify Event \n";
		}

		case MapNotify:
		{
			return "Map Notify Event\n";
		}

		case ReparentNotify:
		{
			return "Reparent Notify Event\n";
		}

		case UnmapNotify:
		{
			return "Unmap notify event\n";
		}

		case MapRequest:
		{
			return "Map request event\n";
		}

		case ClientMessage:
		{
			return "Client Message Event\n";
		}

		case MappingNotify:
		{
			return "Mapping notify event\n";
		}

		case SelectionClear:
		{
			return "Selection Clear event\n";
		}

		case SelectionNotify:
		{
			return "Selection Notify Event\n";
		}

		case SelectionRequest:
		{
			return "Selection Request event\n";
		}

		case VisibilityNotify:
		{
			return "Visibility Notify Event\n";
		}

		default:
		{
			return 0;
		}
	}
}

/**********************************************************************************************//**
 * @fn	GLuint Foundation_Tools::Linux_TranslateKey(GLuint KeySym)
 *
 * @brief	Linux translate key.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	KeySym	The key symbol.
 *
 * @return	A GLuint.
 **************************************************************************************************/

GLuint Foundation_Tools::Linux_TranslateKey(GLuint KeySym)
{
	switch (KeySym)
	{
		case XK_Escape:
		{
			return KEY_ESCAPE;
		}

		case XK_Home:
		{
			return KEY_HOME;
		}

		case XK_Left:
		{
			return KEY_ARROW_LEFT;
		}

		case XK_Right:
		{
			return KEY_ARROW_RIGHT;
		}

		case XK_Up:
		{
			return KEY_ARROW_UP;
		}

		case XK_Down:
		{
			return KEY_ARROW_DOWN;
		}

		case XK_Page_Up:
		{
			return KEY_PAGEUP;
		}

		case XK_Page_Down:
		{
			return KEY_PAGEDOWN;
		}

		case XK_End:
		{
			return KEY_END;
		}

		case XK_Print:
		{
			return KEY_PRINTSCREEN;
		}

		case XK_Insert:
		{
			return KEY_INSERT;
		}

		case XK_Num_Lock:
		{
			return KEY_NUMLOCK;
		}

		case XK_KP_Multiply:
		{
			return KEY_KEYPAD_MULTIPLY;
		}

		case XK_KP_Add:
		{
			return KEY_KEYPAD_ADD;
		}

		case XK_KP_Subtract:
		{
			return KEY_KEYPAD_SUBTRACT;
		}

		case XK_KP_Decimal:
		{
			return KEY_KEYPAD_PERIOD;
		}

		case XK_KP_Divide:
		{
			return KEY_KEYPAD_DIVIDE;
		}

		case XK_KP_0:
		{
			return KEY_KEYPAD_0;
		}

		case XK_KP_1:
		{
			return KEY_KEYPAD_1;
		}

		case XK_KP_2:
		{
			return KEY_KEYPAD_2;
		}

		case XK_KP_3:
		{
			return KEY_KEYPAD_3;
		}

		case XK_KP_4:
		{
			return KEY_KEYPAD_4;
		}

		case XK_KP_5:
		{
			return KEY_KEYPAD_5;
		}

		case XK_KP_6:
		{
			return KEY_KEYPAD_6;
		}

		case XK_KP_7:
		{
			return KEY_KEYPAD_7;
		}

		case XK_KP_8:
		{
			return KEY_KEYPAD_8;
		}

		case XK_KP_9:
		{
			return KEY_KEYPAD_9;
		}

		case XK_F1:
		{
			return KEY_F1;
		}

		case XK_F2:
		{
			return KEY_F2;
		}

		case XK_F3:
		{
			return KEY_F3;
		}

		case XK_F4:
		{
			return KEY_F4;
		}

		case XK_F5:
		{
			return KEY_F5;
		}

		case XK_F6:
		{
			return KEY_F6;
		}

		case XK_F7:
		{
			return KEY_F7;
		}

		case XK_F8:
		{
			return KEY_F8;
		}

		case XK_F9:
		{
			return KEY_F9;
		}

		case XK_F10:
		{
			return KEY_F10;
		}

		case XK_F11:
		{
			return KEY_F11;
		}

		case XK_F12:
		{
			return KEY_F12;
		}

		case XK_Shift_L:
		{
			return KEY_LEFTSHIFT;
		}

		case XK_Shift_R:
		{
			return KEY_RIGHTSHIFT;
		}

		case XK_Control_R:
		{
			return KEY_RIGHTCONTROL;
		}

		case XK_Control_L:
		{
			return KEY_LEFTCONTROL;
		}

		case XK_Caps_Lock:
		{
			return KEY_CAPSLOCK;
		}

		case XK_Alt_L:
		{
			return KEY_LEFTALT;
		}

		case XK_Alt_R:
		{
			return KEY_RIGHTALT;
		}

		default:
		{
			return 0;
		}
	}
}

#endif