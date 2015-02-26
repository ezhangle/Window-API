/**********************************************************************************************//**
 * @file	WindowAPI\WindowManager_Windows.cpp
 *
 * @brief	Implements the window manager windows class.
 **************************************************************************************************/

#include "WindowManager.h"

#if defined(_WIN32) || defined(_WIN64)

/**********************************************************************************************//**
 * @fn	void WindowManager::Windows_Initialize()
 *
 * @brief	Windows initialize.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

GLboolean WindowManager::Windows_Initialize()
{
	CreateTerminal();
	RECT l_Desktop;

	HWND l_DesktopHandle = GetDesktopWindow();


	if (l_DesktopHandle)
	{
		GetWindowRect(l_DesktopHandle, &l_Desktop);

		GetInstance()->ScreenResolution[0] = l_Desktop.right;
		GetInstance()->ScreenResolution[1] = l_Desktop.bottom;

		GetInstance()->Initialized = GL_TRUE;
		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_WINDOWS_CANNOTINITIALIZE);
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	FWindow* WindowManager::GetWindowByHandle(HWND WindowHandle)
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

FWindow* WindowManager::GetWindowByHandle(HWND WindowHandle)
{
	if(GetInstance()->IsInitialized())
	{
#if defined(_MSC_VER)
	for each(auto CurrentWindow in GetInstance()->Windows)
#else
	for (auto CurrentWindow : GetInstance()->Windows)
#endif
	{
		if (CurrentWindow->WindowHandle == WindowHandle)
		{
			return CurrentWindow;
		}
	}

	return nullptr;
	}

	PrintErrorMessage(ERROR_NOTINITIALIZED);
	return nullptr;
}

LRESULT CALLBACK WindowManager::WindowProcedure(HWND WindowHandle, UINT Message, WPARAM WordParam, LPARAM LongParam)
{
	FWindow* l_Window = GetWindowByHandle(WindowHandle);
	switch (Message)
	{
	case WM_CREATE:
	{
		GetWindowByIndex(GetInstance()->Windows.size() - 1)->WindowHandle = WindowHandle;
		l_Window = GetWindowByHandle(WindowHandle);

		l_Window->InitializeGL();
		break;
	}

	case WM_DESTROY:
	{
		l_Window->ShouldClose = GL_TRUE;

		if (IsValidDestroyedEvent(l_Window->DestroyedEvent))
		{
			l_Window->DestroyedEvent();
		}

		l_Window->Shutdown();
		return 0;
	}
	case WM_MOVE:
	{
		l_Window->Position[0] = LOWORD(LongParam);
		l_Window->Position[1] = HIWORD(LongParam);
		
		if (IsValidMovedEvent(l_Window->MovedEvent))
		{
			l_Window->MovedEvent(l_Window->Position[0], l_Window->Position[1]);
		}
		
		break;
	}

	case WM_MOVING:
	{
		l_Window->Position[0] = LOWORD(LongParam);
		l_Window->Position[1] = HIWORD(LongParam);

		if (IsValidMovedEvent(l_Window->MovedEvent))
		{
			l_Window->MovedEvent(l_Window->Position[0], l_Window->Position[1]);
		}
		break;
	}

	case WM_SIZE:
	{
		l_Window->Resolution[0] = (GLuint)LOWORD(LongParam);
		l_Window->Resolution[1] = (GLuint)HIWORD(LongParam);

		switch (WordParam)
		{
			case SIZE_MAXIMIZED:
			{
				if (IsValidDestroyedEvent(l_Window->MaximizedEvent))
				{
					l_Window->MaximizedEvent();
				}

				break;
			}

			case SIZE_MINIMIZED:
			{
				if (IsValidDestroyedEvent(l_Window->MinimizedEvent))
				{
					l_Window->MinimizedEvent();
				}
				break;
			}

			/*case SIZE_RESTORED:
			{
				if (IsValid(l_Window->MaximizedEvent))
				{
					l_Window->RestoredEvent();
				}
				break;
			}*/

			default:
			{
				if (IsValidMovedEvent(l_Window->ResizeEvent))
				{
					l_Window->ResizeEvent(l_Window->Resolution[0],
						l_Window->Resolution[1]);
				}

				break;
			}
		}
		break;
	}

	case WM_SIZING:
	{

		l_Window->Resolution[0] = (GLuint)LOWORD(LongParam);
		l_Window->Resolution[1] = (GLuint)HIWORD(LongParam);

		if (IsValidMovedEvent(l_Window->ResizeEvent))
		{
			l_Window->ResizeEvent(l_Window->Resolution[0],
				l_Window->Resolution[1]);
		}


		break;
	}

	case WM_KEYDOWN:
	{
 		GLuint l_TranslatedKey = 0;

		switch (HIWORD(LongParam))
		{
			case 29:
			{
				l_Window->Keys[KEY_LEFTCONTROL] = KEYSTATE_DOWN;
				l_TranslatedKey = KEY_LEFTCONTROL;
				break;
			}

			case 285:
			{
				l_Window->Keys[KEY_RIGHTCONTROL] = KEYSTATE_DOWN;
				l_TranslatedKey = KEY_RIGHTCONTROL;
				break;
			}

			case 42:
			{
				l_Window->Keys[KEY_LEFTSHIFT] = KEYSTATE_DOWN;
				l_TranslatedKey = KEY_LEFTSHIFT;
				break;
			}

			case 54:
			{
				l_Window->Keys[KEY_RIGHTSHIFT] = KEYSTATE_DOWN;
				l_TranslatedKey = KEY_RIGHTSHIFT;
				break;
			}

			default:
			{
				l_TranslatedKey = Windows_TranslateKey(WordParam, LongParam);
				l_Window->Keys[l_TranslatedKey] = KEYSTATE_DOWN;
				break;
			}
		}

		if (IsValidKeyEvent(l_Window->KeyEvent))
		{
			l_Window->KeyEvent(l_TranslatedKey, KEYSTATE_DOWN);
		}
		break;
	}

	case WM_KEYUP:
	{
		GLuint l_TranslatedKey = 0;		

		switch (HIWORD(LongParam))
		{
			case 49181:
			{
				l_Window->Keys[KEY_LEFTCONTROL] = KEYSTATE_UP;
				l_TranslatedKey = KEY_LEFTCONTROL;
				break;
			}

			case 49437:
			{
				l_Window->Keys[KEY_RIGHTCONTROL] = KEYSTATE_UP;
				l_TranslatedKey = KEY_RIGHTCONTROL;
				break;
			}

			case 49194:
			{
				l_Window->Keys[KEY_LEFTSHIFT] = KEYSTATE_UP;
				l_TranslatedKey = KEY_LEFTSHIFT;
				break;
			}

			case 49206:
			{
				l_Window->Keys[KEY_RIGHTSHIFT] = KEYSTATE_UP;
				l_TranslatedKey = KEY_RIGHTSHIFT;
				break;
			}

			default:
			{
				l_TranslatedKey = Windows_TranslateKey(WordParam, LongParam);
				l_Window->Keys[l_TranslatedKey] = KEYSTATE_UP;
				break;
			}
		}

		if (IsValidKeyEvent(l_Window->KeyEvent))
		{
			l_Window->KeyEvent(l_TranslatedKey, KEYSTATE_UP);
		}
		break;
	}

	case WM_SYSKEYDOWN:
	{
		GLuint l_TranslatedKey = 0;
		switch (HIWORD(LongParam))
		{
		case 8248:
		{
			l_Window->Keys[KEY_LEFTALT] = KEYSTATE_DOWN;
			l_TranslatedKey = KEY_LEFTALT;
			break;
		}


		case 8504:
		{
			l_Window->Keys[KEY_RIGHTALT] = KEYSTATE_DOWN;
			l_TranslatedKey = KEY_RIGHTALT;
		}

		default:
		{
			break;
		}
		}

		if (IsValidKeyEvent(l_Window->KeyEvent))
		{
			l_Window->KeyEvent(l_TranslatedKey, KEYSTATE_DOWN);
		}

		break;
	}

	case WM_SYSKEYUP:
	{
		GLuint l_TranslatedKey = 0;
		switch (HIWORD(LongParam))
		{
		case 49208:
		{
			l_Window->Keys[KEY_LEFTALT] = KEYSTATE_UP;
			l_TranslatedKey = KEY_LEFTALT;
			break;
		}


		case 49464:
		{
			l_Window->Keys[KEY_RIGHTALT] = KEYSTATE_UP;
			l_TranslatedKey = KEY_RIGHTALT;
			break;
		}

		default:
		{
			break;
		}
		}

		if (IsValidKeyEvent(l_Window->KeyEvent))
		{
			l_Window->KeyEvent(l_TranslatedKey, KEYSTATE_UP);
		}
		break;
	}

	case WM_MOUSEMOVE:
	{
		l_Window->MousePosition[0] = (GLuint)LOWORD(LongParam);
		l_Window->MousePosition[1] = (GLuint)HIWORD(LongParam);

		POINT l_Point;
		l_Point.x = l_Window->MousePosition[0];
		l_Point.y = l_Window->MousePosition[1];

		ClientToScreen(WindowHandle, &l_Point);

		//printf("%i %i \n", l_Point.x, l_Point.y);

		if (IsValidMouseMoveEvent(l_Window->MouseMoveEvent))
		{
			l_Window->MouseMoveEvent(l_Window->MousePosition[0],
				l_Window->MousePosition[1], l_Point.x, l_Point.y);
		}
		break;
	}

	case WM_LBUTTONDOWN:
	{
		l_Window->MouseButton[MOUSE_LEFTBUTTON] = MOUSE_BUTTONDOWN;

		if (IsValidKeyEvent(l_Window->MouseButtonEvent))
		{
			l_Window->MouseButtonEvent(MOUSE_LEFTBUTTON, MOUSE_BUTTONDOWN);
		}
		break;
	}

	case WM_LBUTTONUP:
	{
		l_Window->MouseButton[MOUSE_LEFTBUTTON] = MOUSE_BUTTONUP;

		if (IsValidKeyEvent(l_Window->MouseButtonEvent))
		{
			l_Window->MouseButtonEvent(MOUSE_LEFTBUTTON, MOUSE_BUTTONUP);
		}
		break;
	}

	case WM_RBUTTONDOWN:
	{
		l_Window->MouseButton[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONDOWN;

		if (IsValidKeyEvent(l_Window->MouseButtonEvent))
		{
			l_Window->MouseButtonEvent(MOUSE_RIGHTBUTTON, MOUSE_BUTTONDOWN);
		}
		break;
	}

	case WM_RBUTTONUP:
	{
		l_Window->MouseButton[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONUP;

		if (IsValidKeyEvent(l_Window->MouseButtonEvent))
		{
			l_Window->MouseButtonEvent(MOUSE_RIGHTBUTTON, MOUSE_BUTTONUP);
		}
		break;
	}

	case WM_MBUTTONDOWN:
	{
		l_Window->MouseButton[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONDOWN;

		if (IsValidKeyEvent(l_Window->MouseButtonEvent))
		{
			l_Window->MouseButtonEvent(MOUSE_MIDDLEBUTTON, MOUSE_BUTTONDOWN);
		}
		break;
	}

	case WM_MBUTTONUP:
	{
		l_Window->MouseButton[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONUP;

		if (IsValidKeyEvent(l_Window->MouseButtonEvent))
		{
			l_Window->MouseButtonEvent(MOUSE_MIDDLEBUTTON, MOUSE_BUTTONUP);
		}
		break;
	}

	case WM_MOUSEWHEEL:
	{
		if ((WordParam % WHEEL_DELTA) > 0)
		{
			if (IsValidMouseWheelEvent(l_Window->MouseWheelEvent))
			{
				l_Window->MouseWheelEvent(MOUSE_SCROLL_DOWN);
			}
		}

		else
		{
			if (IsValidMouseWheelEvent(l_Window->MouseWheelEvent))
			{
				l_Window->MouseWheelEvent(MOUSE_SCROLL_UP);
			}

		}
		break;
	}

	default:
	{
		return DefWindowProc(WindowHandle, Message, WordParam, LongParam);
	}
	}
	return 0;
}

LRESULT CALLBACK WindowManager::StaticWindowProcedure(HWND WindowHandle, UINT Message, WPARAM WordParam, LPARAM LongParam)
{
	return WindowManager::GetInstance()->WindowProcedure(WindowHandle, Message, WordParam, LongParam);
}

GLboolean WindowManager::Windows_PollForEvents()
{
	if (GetInstance()->IsInitialized())
	{
		if (PeekMessage(&GetInstance()->Message, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&GetInstance()->Message);
			DispatchMessage(&GetInstance()->Message);
			return FOUNDATION_OKAY;
		}

		//if there is no event just return.
		return FOUNDATION_OKAY;
	}

	else
	{

		PrintErrorMessage(ERROR_NOTINITIALIZED);
		return FOUNDATION_ERROR;
	}
}

GLboolean WindowManager::Windows_WaitForEvents()
{
	if (GetInstance()->IsInitialized())
	{
			GetMessage(&GetInstance()->Message, 0, 0, 0);
			TranslateMessage(&GetInstance()->Message);
			DispatchMessage(&GetInstance()->Message);
			return FOUNDATION_OKAY;
	}
	PrintErrorMessage(ERROR_NOTINITIALIZED);
	return FOUNDATION_ERROR;
}

void WindowManager::CreateTerminal()
{
	int hConHandle;
	long lStdHandle;
	FILE *fp;

	// allocate a console for this app
	AllocConsole();

	// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;

	setvbuf(stdout, nullptr, _IONBF, 0);
}

GLboolean WindowManager::Windows_SetMousePositionInScreen(GLuint X, GLuint Y)
{
	if (GetInstance()->IsInitialized())
	{
		POINT l_MousePoint;
		l_MousePoint.x = X;
		l_MousePoint.y = Y;
		SetCursorPos(l_MousePoint.x, l_MousePoint.y);
	}

	PrintErrorMessage(ERROR_NOTINITIALIZED);
	return FOUNDATION_ERROR;
}

GLuint WindowManager::Windows_TranslateKey(WPARAM WordParam, LPARAM LongParam)
{
	switch (WordParam)
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
			return KEY_LEFTWINDOW;
		}

		case VK_RWIN:
		{
			return KEY_RIGHTWINDOW;
		}

		default:
		{
			return WordParam;
		}
	}
}
#endif
