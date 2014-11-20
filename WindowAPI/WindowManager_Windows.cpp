#include "WindowManager.h"
#include "Tools.h"

#if defined(CURRENT_OS_WINDOWS)
void F_WM::Windows_Initialize()
{
	CreateTerminal();
	RECT l_Desktop;

	HWND l_DesktopHandle = GetDesktopWindow();

	GetWindowRect(l_DesktopHandle, &l_Desktop);

	GetInstance()->m_ScreenResolution[0] = l_Desktop.right;
	GetInstance()->m_ScreenResolution[1] = l_Desktop.bottom;
}

F_W* F_WM::GetWindowByHandle(HWND a_WindowHandle)
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

LRESULT CALLBACK F_WM::WindowProcedure(HWND a_WindowHandle, UINT a_Message, WPARAM a_WordParam, LPARAM a_LongParam)
{
	F_W* l_Window = GetWindowByHandle(a_WindowHandle);
	switch (a_Message)
	{
	case WM_CREATE:
	{
		GetWindowByIndex(m_Windows.size() - 1)->m_WindowHandle = a_WindowHandle;
		l_Window = GetWindowByHandle(a_WindowHandle);

		l_Window->InitializeGL();
		break;
	}

	case WM_DESTROY:
	{
		l_Window->m_ShouldClose = true;

		if (Foundation_Tools::IsValid(l_Window->m_OnDestroyed))
		{
			l_Window->m_OnDestroyed();
		}

		l_Window->Shutdown();
		break;
	}
	case WM_MOVE:
	{
		l_Window->m_Position[0] = LOWORD(a_LongParam);
		l_Window->m_Position[1] = HIWORD(a_LongParam);
		
		if (Foundation_Tools::IsValid(l_Window->m_OnMoved))
		{
			l_Window->m_OnMoved(l_Window->m_Position[0], l_Window->m_Position[1]);
		}
		
		break;
	}

	case WM_MOVING:
	{
		l_Window->m_Position[0] = LOWORD(a_LongParam);
		l_Window->m_Position[1] = HIWORD(a_LongParam);

		if (Foundation_Tools::IsValid(l_Window->m_OnMoved))
		{
			l_Window->m_OnMoved(l_Window->m_Position[0], l_Window->m_Position[1]);
		}
		break;
	}

	case WM_SIZE:
	{
		l_Window->m_Resolution[0] = (GLuint)LOWORD(a_LongParam);
		l_Window->m_Resolution[1] = (GLuint)HIWORD(a_LongParam);

		switch (a_WordParam)
		{
			case SIZE_MAXIMIZED:
			{
				if (Foundation_Tools::IsValid(l_Window->m_OnMaximized))
				{
					l_Window->m_OnMaximized();
				}

				break;
			}

			case SIZE_MINIMIZED:
			{
				if (Foundation_Tools::IsValid(l_Window->m_OnMinimized))
				{
					l_Window->m_OnMinimized();
				}
				break;
			}

			case SIZE_RESTORED:
			{
				if (Foundation_Tools::IsValid(l_Window->m_OnMaximized))
				{
					l_Window->m_OnRestored();
				}
				break;
			}

			default:
			{
				if (Foundation_Tools::IsValid(l_Window->m_OnResize))
				{
					l_Window->m_OnResize(l_Window->m_Resolution[0],
						l_Window->m_Resolution[1]);
				}

				break;
			}
		}
		break;
	}

	case WM_SIZING:
	{

		l_Window->m_Resolution[0] = (GLuint)LOWORD(a_LongParam);
		l_Window->m_Resolution[1] = (GLuint)HIWORD(a_LongParam);

		if (Foundation_Tools::IsValid(l_Window->m_OnResize))
		{
			l_Window->m_OnResize(l_Window->m_Resolution[0],
				l_Window->m_Resolution[1]);
		}


		break;
	}

	case WM_KEYDOWN:
	{
 		GLuint l_TranslatedKey = 0;

		switch (HIWORD(a_LongParam))
		{
			case 29:
			{
				l_Window->m_Keys[KEY_LEFTCONTROL] = KEYSTATE_DOWN;
				l_TranslatedKey = KEY_LEFTCONTROL;
				break;
			}

			case 285:
			{
				l_Window->m_Keys[KEY_RIGHTCONTROL] = KEYSTATE_DOWN;
				l_TranslatedKey = KEY_RIGHTCONTROL;
				break;
			}

			case 42:
			{
				l_Window->m_Keys[KEY_LEFTSHIFT] = KEYSTATE_DOWN;
				l_TranslatedKey = KEY_LEFTSHIFT;
				break;
			}

			case 54:
			{
				l_Window->m_Keys[KEY_RIGHTSHIFT] = KEYSTATE_DOWN;
				l_TranslatedKey = KEY_RIGHTSHIFT;
				break;
			}

			default:
			{
				l_TranslatedKey = Foundation_Tools::Windows_TranslateKey(a_WordParam, a_LongParam);
				l_Window->m_Keys[l_TranslatedKey] = KEYSTATE_DOWN;
				break;
			}
		}

		if (Foundation_Tools::IsValid(l_Window->m_OnKeyEvent))
		{
			l_Window->m_OnKeyEvent(l_TranslatedKey, KEYSTATE_DOWN);
		}
		break;
	}

	case WM_KEYUP:
	{
		GLuint l_TranslatedKey = 0;		

		switch (HIWORD(a_LongParam))
		{
			case 49181:
			{
				l_Window->m_Keys[KEY_LEFTCONTROL] = KEYSTATE_UP;
				l_TranslatedKey = KEY_LEFTCONTROL;
				break;
			}

			case 49437:
			{
				l_Window->m_Keys[KEY_RIGHTCONTROL] = KEYSTATE_UP;
				l_TranslatedKey = KEY_RIGHTCONTROL;
				break;
			}

			case 49194:
			{
				l_Window->m_Keys[KEY_LEFTSHIFT] = KEYSTATE_UP;
				l_TranslatedKey = KEY_LEFTSHIFT;
				break;
			}

			case 49206:
			{
				l_Window->m_Keys[KEY_RIGHTSHIFT] = KEYSTATE_UP;
				l_TranslatedKey = KEY_RIGHTSHIFT;
				break;
			}

			default:
			{
				l_TranslatedKey = Foundation_Tools::Windows_TranslateKey(a_WordParam, a_LongParam);
				l_Window->m_Keys[l_TranslatedKey] = KEYSTATE_UP;
				break;
			}
		}

		if (Foundation_Tools::IsValid(l_Window->m_OnKeyEvent))
		{
			l_Window->m_OnKeyEvent(l_TranslatedKey, KEYSTATE_UP);
		}
		break;
	}

	case WM_SYSKEYDOWN:
	{
		GLuint l_TranslatedKey = 0;
		switch (HIWORD(a_LongParam))
		{
		case 8248:
		{
			l_Window->m_Keys[KEY_LEFTALT] = KEYSTATE_DOWN;
			l_TranslatedKey = KEY_LEFTALT;
			break;
		}


		case 8504:
		{
			l_Window->m_Keys[KEY_RIGHTALT] = KEYSTATE_DOWN;
			l_TranslatedKey = KEY_RIGHTALT;
		}

		default:
		{
			break;
		}
		}

		if (Foundation_Tools::IsValid(l_Window->m_OnKeyEvent))
		{
			l_Window->m_OnKeyEvent(l_TranslatedKey, KEYSTATE_DOWN);
		}

		break;
	}

	case WM_SYSKEYUP:
	{
		GLuint l_TranslatedKey = 0;
		switch (HIWORD(a_LongParam))
		{
		case 49208:
		{
			l_Window->m_Keys[KEY_LEFTALT] = KEYSTATE_UP;
			l_TranslatedKey = KEY_LEFTALT;
			break;
		}


		case 49464:
		{
			l_Window->m_Keys[KEY_RIGHTALT] = KEYSTATE_UP;
			l_TranslatedKey = KEY_RIGHTALT;
			break;
		}

		default:
		{
			break;
		}
		}

		if (Foundation_Tools::IsValid(l_Window->m_OnKeyEvent))
		{
			l_Window->m_OnKeyEvent(l_TranslatedKey, KEYSTATE_UP);
		}
		break;
	}

	case WM_MOUSEMOVE:
	{
		l_Window->m_MousePosition[0] = (GLuint)LOWORD(a_LongParam);
		l_Window->m_MousePosition[1] = (GLuint)HIWORD(a_LongParam);

		POINT l_Point;
		l_Point.x = l_Window->m_MousePosition[0];
		l_Point.y = l_Window->m_MousePosition[1];

		ClientToScreen(a_WindowHandle, &l_Point);

		//printf("%i %i \n", l_Point.x, l_Point.y);

		if (Foundation_Tools::IsValid(l_Window->m_OnMouseMove))
		{
			l_Window->m_OnMouseMove(l_Window->m_MousePosition[0],
				l_Window->m_MousePosition[1], l_Point.x, l_Point.y);
		}
		break;
	}

	case WM_LBUTTONDOWN:
	{
		l_Window->m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONDOWN;

		if (Foundation_Tools::IsValid(l_Window->m_OnMouseButtonEvent))
		{
			l_Window->m_OnMouseButtonEvent(MOUSE_LEFTBUTTON, MOUSE_BUTTONDOWN);
		}
		break;
	}

	case WM_LBUTTONUP:
	{
		l_Window->m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONUP;

		if (Foundation_Tools::IsValid(l_Window->m_OnMouseButtonEvent))
		{
			l_Window->m_OnMouseButtonEvent(MOUSE_LEFTBUTTON, MOUSE_BUTTONUP);
		}
		break;
	}

	case WM_RBUTTONDOWN:
	{
		l_Window->m_MouseEvents[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONDOWN;

		if (Foundation_Tools::IsValid(l_Window->m_OnMouseButtonEvent))
		{
			l_Window->m_OnMouseButtonEvent(MOUSE_RIGHTBUTTON, MOUSE_BUTTONDOWN);
		}
		break;
	}

	case WM_RBUTTONUP:
	{
		l_Window->m_MouseEvents[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONUP;

		if (Foundation_Tools::IsValid(l_Window->m_OnMouseButtonEvent))
		{
			l_Window->m_OnMouseButtonEvent(MOUSE_RIGHTBUTTON, MOUSE_BUTTONUP);
		}
		break;
	}

	case WM_MBUTTONDOWN:
	{
		l_Window->m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONDOWN;

		if (Foundation_Tools::IsValid(l_Window->m_OnMouseButtonEvent))
		{
			l_Window->m_OnMouseButtonEvent(MOUSE_MIDDLEBUTTON, MOUSE_BUTTONDOWN);
		}
		break;
	}

	case WM_MBUTTONUP:
	{
		l_Window->m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONUP;

		if (Foundation_Tools::IsValid(l_Window->m_OnMouseButtonEvent))
		{
			l_Window->m_OnMouseButtonEvent(MOUSE_MIDDLEBUTTON, MOUSE_BUTTONUP);
		}
		break;
	}

	case WM_MOUSEWHEEL:
	{
		if ((a_WordParam % WHEEL_DELTA) > 0)
		{
			if (Foundation_Tools::IsValid(l_Window->m_OnMouseWheel))
			{
				l_Window->m_OnMouseWheel(MOUSE_SCROLL_DOWN);
			}
		}

		else
		{
			if (Foundation_Tools::IsValid(l_Window->m_OnMouseWheel))
			{
				l_Window->m_OnMouseWheel(MOUSE_SCROLL_UP);
			}

		}
		break;
	}

	default:
	{
		return DefWindowProc(a_WindowHandle, a_Message, a_WordParam, a_LongParam);
	}
	}
	return 0;
}

LRESULT CALLBACK F_WM::StaticWindowProcedure(HWND a_WindowHandle, UINT a_Message, WPARAM a_WordParam, LPARAM a_LongParam)
{
	return F_WM::GetInstance()->WindowProcedure(a_WindowHandle, a_Message, a_WordParam, a_LongParam);
}

void F_WM::Windows_PollForEvents()
{
	GetMessage(&GetInstance()->m_Message, 0, 0, 0);
	TranslateMessage(&GetInstance()->m_Message);
	DispatchMessage(&GetInstance()->m_Message);
}

void F_WM::CreateTerminal()
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

#endif

