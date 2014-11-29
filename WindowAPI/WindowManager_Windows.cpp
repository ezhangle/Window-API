/**********************************************************************************************//**
 * @file	WindowAPI\WindowManager_Windows.cpp
 *
 * @brief	Implements the window manager windows class.
 **************************************************************************************************/

#include "WindowManager.h"
#include "Tools.h"

#if defined(CURRENT_OS_WINDOWS)

/**********************************************************************************************//**
 * @fn	void WindowManager::Windows_Initialize()
 *
 * @brief	Windows initialize.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

void WindowManager::Windows_Initialize()
{
	CreateTerminal();
	RECT l_Desktop;

	HWND l_DesktopHandle = GetDesktopWindow();

	GetWindowRect(l_DesktopHandle, &l_Desktop);

	GetInstance()->ScreenResolution[0] = l_Desktop.right;
	GetInstance()->ScreenResolution[1] = l_Desktop.bottom;
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
	for (GLuint l_Iter = 0; l_Iter < GetInstance()->Windows.size(); l_Iter++)
	{
		if (GetInstance()->Windows[l_Iter]->GetWindowHandle() == WindowHandle)
		{
			return GetInstance()->Windows[l_Iter];
		}
	}

	return nullptr;
}

LRESULT CALLBACK WindowManager::WindowProcedure(HWND WindowHandle, UINT Message, WPARAM WordParam, LPARAM LongParam)
{
	FWindow* l_Window = GetWindowByHandle(WindowHandle);
	switch (Message)
	{
	case WM_CREATE:
	{
		GetWindowByIndex(Windows.size() - 1)->WindowHandle = WindowHandle;
		l_Window = GetWindowByHandle(WindowHandle);

		l_Window->InitializeGL();
		break;
	}

	case WM_DESTROY:
	{
		l_Window->ShouldClose = GL_TRUE;

		if (Foundation_Tools::IsValid(l_Window->DestroyedEvent))
		{
			l_Window->DestroyedEvent();
		}

		l_Window->Shutdown();
		break;
	}
	case WM_MOVE:
	{
		l_Window->Position[0] = LOWORD(LongParam);
		l_Window->Position[1] = HIWORD(LongParam);
		
		if (Foundation_Tools::IsValid(l_Window->MovedEvent))
		{
			l_Window->MovedEvent(l_Window->Position[0], l_Window->Position[1]);
		}
		
		break;
	}

	case WM_MOVING:
	{
		l_Window->Position[0] = LOWORD(LongParam);
		l_Window->Position[1] = HIWORD(LongParam);

		if (Foundation_Tools::IsValid(l_Window->MovedEvent))
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
				if (Foundation_Tools::IsValid(l_Window->MaximizedEvent))
				{
					l_Window->MaximizedEvent();
				}

				break;
			}

			case SIZE_MINIMIZED:
			{
				if (Foundation_Tools::IsValid(l_Window->MinimizedEvent))
				{
					l_Window->MinimizedEvent();
				}
				break;
			}

			/*case SIZE_RESTORED:
			{
				if (Foundation_Tools::IsValid(l_Window->MaximizedEvent))
				{
					l_Window->RestoredEvent();
				}
				break;
			}*/

			default:
			{
				if (Foundation_Tools::IsValid(l_Window->ResizeEvent))
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

		if (Foundation_Tools::IsValid(l_Window->ResizeEvent))
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
				l_TranslatedKey = Foundation_Tools::Windows_TranslateKey(WordParam, LongParam);
				l_Window->Keys[l_TranslatedKey] = KEYSTATE_DOWN;
				break;
			}
		}

		if (Foundation_Tools::IsValid(l_Window->KeyEvent))
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
				l_TranslatedKey = Foundation_Tools::Windows_TranslateKey(WordParam, LongParam);
				l_Window->Keys[l_TranslatedKey] = KEYSTATE_UP;
				break;
			}
		}

		if (Foundation_Tools::IsValid(l_Window->KeyEvent))
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

		if (Foundation_Tools::IsValid(l_Window->KeyEvent))
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

		if (Foundation_Tools::IsValid(l_Window->KeyEvent))
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

		if (Foundation_Tools::IsValid(l_Window->MouseMoveEvent))
		{
			l_Window->MouseMoveEvent(l_Window->MousePosition[0],
				l_Window->MousePosition[1], l_Point.x, l_Point.y);
		}
		break;
	}

	case WM_LBUTTONDOWN:
	{
		l_Window->MouseButton[MOUSE_LEFTBUTTON] = MOUSE_BUTTONDOWN;

		if (Foundation_Tools::IsValid(l_Window->MouseButtonEvent))
		{
			l_Window->MouseButtonEvent(MOUSE_LEFTBUTTON, MOUSE_BUTTONDOWN);
		}
		break;
	}

	case WM_LBUTTONUP:
	{
		l_Window->MouseButton[MOUSE_LEFTBUTTON] = MOUSE_BUTTONUP;

		if (Foundation_Tools::IsValid(l_Window->MouseButtonEvent))
		{
			l_Window->MouseButtonEvent(MOUSE_LEFTBUTTON, MOUSE_BUTTONUP);
		}
		break;
	}

	case WM_RBUTTONDOWN:
	{
		l_Window->MouseButton[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONDOWN;

		if (Foundation_Tools::IsValid(l_Window->MouseButtonEvent))
		{
			l_Window->MouseButtonEvent(MOUSE_RIGHTBUTTON, MOUSE_BUTTONDOWN);
		}
		break;
	}

	case WM_RBUTTONUP:
	{
		l_Window->MouseButton[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONUP;

		if (Foundation_Tools::IsValid(l_Window->MouseButtonEvent))
		{
			l_Window->MouseButtonEvent(MOUSE_RIGHTBUTTON, MOUSE_BUTTONUP);
		}
		break;
	}

	case WM_MBUTTONDOWN:
	{
		l_Window->MouseButton[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONDOWN;

		if (Foundation_Tools::IsValid(l_Window->MouseButtonEvent))
		{
			l_Window->MouseButtonEvent(MOUSE_MIDDLEBUTTON, MOUSE_BUTTONDOWN);
		}
		break;
	}

	case WM_MBUTTONUP:
	{
		l_Window->MouseButton[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONUP;

		if (Foundation_Tools::IsValid(l_Window->MouseButtonEvent))
		{
			l_Window->MouseButtonEvent(MOUSE_MIDDLEBUTTON, MOUSE_BUTTONUP);
		}
		break;
	}

	case WM_MOUSEWHEEL:
	{
		if ((WordParam % WHEEL_DELTA) > 0)
		{
			if (Foundation_Tools::IsValid(l_Window->MouseWheelEvent))
			{
				l_Window->MouseWheelEvent(MOUSE_SCROLL_DOWN);
			}
		}

		else
		{
			if (Foundation_Tools::IsValid(l_Window->MouseWheelEvent))
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

void WindowManager::Windows_PollForEvents()
{
	GetMessage(&GetInstance()->Message, 0, 0, 0);
	TranslateMessage(&GetInstance()->Message);
	DispatchMessage(&GetInstance()->Message);
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

void WindowManager::Windows_SetMousePositionInScreen(GLuint X, GLuint Y)
{
	POINT l_MousePoint;
	l_MousePoint.x = X;
	l_MousePoint.y = Y;
	SetCursorPos(l_MousePoint.x, l_MousePoint.y);
}

#endif

