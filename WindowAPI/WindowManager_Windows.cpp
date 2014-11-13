#include "WindowManager.h"
#include "Window_Tools.h"

#if defined(CURRENT_OS_WINDOWS)
void F_WM::Windows_Initialize()
{
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
		l_Window->m_DeviceContextHandle = GetDC(l_Window->m_WindowHandle);
		l_Window->InitializePixelFormat();
		l_Window->m_GLRenderingcontextHandle = wglCreateContext(l_Window->m_DeviceContextHandle);
		wglMakeCurrent(l_Window->m_DeviceContextHandle, l_Window->m_GLRenderingcontextHandle);
		//l_Window->InitializeGL();
		break;
	}
	case WM_DESTROY:
	{
		l_Window->m_ShouldClose = true;
		l_Window->Shutdown();
		break;
	}
	case WM_MOVE:
	{
		l_Window->m_Position[0] = LOWORD(a_LongParam);
		l_Window->m_Position[1] = HIWORD(a_LongParam);
		
		break;
	}

	case WM_SIZE:
	{
		if (l_Window->m_GLRenderingcontextHandle)
		{	
			l_Window->m_Resolution[0] = (GLuint)LOWORD(a_LongParam);
			l_Window->m_Resolution[1] =(GLuint)HIWORD(a_LongParam);
			break;
		}
		break;
	}

	case WM_SIZING:
	{
		if (l_Window->m_GLRenderingcontextHandle)
		{
			l_Window->m_Resolution[0] = (GLuint)LOWORD(a_LongParam);
			l_Window->m_Resolution[1] = (GLuint)HIWORD(a_LongParam);
			break;
		}
		break;
	}

	case WM_KEYDOWN:
	{
		GLuint l_TranslatedKey = 0;
		if (a_WordParam > 255)
		{
			l_TranslatedKey = l_Window->Windows_TranslateKey(a_WordParam, a_LongParam);
			l_Window->m_Keys[l_TranslatedKey] = KEYSTATE_DOWN;
		}

		else
		{
			l_TranslatedKey = a_WordParam;
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
		
		if (a_WordParam > 255)
		{
			l_TranslatedKey = l_Window->Windows_TranslateKey(a_WordParam, a_LongParam);
			l_Window->m_Keys[l_TranslatedKey] = KEYSTATE_UP;
		}

		else
		{
			l_TranslatedKey = a_WordParam;
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
		break;
	}

	case WM_LBUTTONDOWN:
	{
		l_Window->m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_LBUTTONUP:
	{
		l_Window->m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONUP;
		break;
	}

	case WM_RBUTTONDOWN:
	{
		l_Window->m_MouseEvents[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_RBUTTONUP:
	{
		l_Window->m_MouseEvents[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_MBUTTONDOWN:
	{
		l_Window->m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_MBUTTONUP:
	{
		l_Window->m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONUP;
		break;
	}

	case WM_MOUSEWHEEL:
	{
		printf("%i %i\n", (GLint)HIWORD(a_WordParam), (GLint)HIWORD(a_WordParam) % WHEEL_DELTA);
		break;
	}

	case WM_MOUSELEAVE:
	{
		printf("Mouse had left");
		break;
	}

	case WM_GETMINMAXINFO:
	{
		break;
	}

	case WM_CHAR:
	{
		printf("%i \n", TCHAR(a_WordParam));
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

#endif

