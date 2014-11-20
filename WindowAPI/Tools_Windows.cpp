
#include "Tools.h"
#if defined(CURRENT_OS_WINDOWS)
GLuint Foundation_Tools::Windows_TranslateKey(WPARAM a_WordParam, LPARAM a_LongParam)
{
	switch (a_WordParam)
	{
		case VK_ESCAPE:
		{
			return KEY_ESCAPE;
		}

		case VK_F1:
		{
			return KEY_F1;
		}

		case VK_F2:
		{
			return KEY_F2;
		}

		case VK_F3:
		{
			return KEY_F3;
		}

		case VK_F4:
		{
			return KEY_F4;
		}

		case VK_F5:
		{
			return KEY_F5;
		}

		case VK_F6:
		{
			return KEY_F6;
		}

		case VK_F7:
		{
			return KEY_F7;
		}

		case VK_F8:
		{
			return KEY_F8;
		}

		case VK_F9:
		{
			return KEY_F9;
		}

		case VK_F10:
		{
			return KEY_F10;
		}

		case VK_F11:
		{
			return KEY_F11;
		}

		case VK_F12:
		{
			return KEY_F12;
		}

		case VK_BACK:
		{
			return KEY_BACKSPACE;
		}

		case VK_TAB:
		{
			return KEY_TAB;
		}

		case VK_CAPITAL:
		{
			return KEY_CAPSLOCK;
		}

		case VK_RETURN:
		{
			return KEY_ENTER;
		}

		case VK_PRINT:
		{
			return KEY_PRINTSCREEN;
		}

		case VK_SCROLL:
		{
			return KEY_SCROLLLOCK;
		}

		case VK_PAUSE:
		{
			return KEY_PAUSE;
		}

		case VK_INSERT:
		{
			return KEY_INSERT;
		}

		case VK_HOME:
		{
			return KEY_HOME;
		}

		case VK_DELETE:
		{
			return KEY_DELETE;
		}

		case VK_END:
		{
			return KEY_END;
		}

		case VK_PRIOR:
		{
			return KEY_PAGEUP;
		}

		case VK_NEXT:
		{
			return KEY_PAGEDOWN;
		}

		case VK_DOWN:
		{
			return KEY_ARROW_DOWN;
		}

		case VK_UP:
		{
			return KEY_ARROW_UP;
		}

		case VK_LEFT:
		{
			return KEY_ARROW_LEFT;
		}

		case VK_RIGHT:
		{
			return KEY_ARROW_RIGHT;
		}

		case VK_DIVIDE:
		{
			return KEY_KEYPAD_DIVIDE;
		}

		case VK_MULTIPLY:
		{
			return KEY_KEYPAD_MULTIPLY;
		}

		case VK_SUBTRACT:
		{
			return KEY_KEYPAD_DIVIDE;
		}

		case VK_ADD:
		{
			return KEY_KEYPAD_ADD;
		}

		case VK_DECIMAL:
		{
			return KEY_KEYPAD_PERIOD;
		}

		case VK_NUMPAD0:
		{
			return KEY_KEYPAD_0;
		}

		case VK_NUMPAD1:
		{
			return KEY_KEYPAD_1;
		}

		case VK_NUMPAD2:
		{
			return KEY_KEYPAD_2;
		}

		case VK_NUMPAD3:
		{
			return KEY_KEYPAD_3;
		}

		case VK_NUMPAD4:
		{
			return KEY_KEYPAD_4;
		}

		case VK_NUMPAD5:
		{
			return KEY_KEYPAD_5;
		}

		case VK_NUMPAD6:
		{
			return KEY_KEYPAD_6;
		}

		case VK_NUMPAD7:
		{
			return KEY_KEYPAD_7;
		}

		case VK_NUMPAD8:
		{
			return KEY_KEYPAD_8;
		}

		case VK_NUMPAD9:
		{
			return KEY_KEYPAD_9;
		}

		case VK_LWIN:
		{
			return KEY_WINDOW_LEFT;
		}

		case VK_RWIN:
		{
			return KEY_WINDOW_RIGHT;
		}

		default:
		{
			return a_WordParam;
		}
	}
}

#endif