#include "WindowManager.h"
#include "Window_Tools.h"

#if defined(CURRENT_OS_WINDOWS)
F_W* F_WM::GetWindowByHandle(HWND a_WindowHandle)
{
	for (GLuint l_Iter = 0; l_Iter < GetInstance()->m_Windows.size() - 1; l_Iter++)
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
	switch (a_Message)
	{
	case WM_CREATE:
	{
		GetWindowByIndex(m_Windows.size() - 1)->m_WindowHandle = a_WindowHandle;
		GetWindowByHandle(a_WindowHandle)->m_DeviceContextHandle = GetDC(GetWindowByHandle(a_WindowHandle)->m_WindowHandle);
		GetWindowByHandle(a_WindowHandle)->InitializePixelFormat();
		GetWindowByHandle(a_WindowHandle)->m_GLRenderingcontextHandle = wglCreateContext(GetWindowByHandle(a_WindowHandle)->m_DeviceContextHandle);
		wglMakeCurrent(GetWindowByHandle(a_WindowHandle)->m_DeviceContextHandle, GetWindowByHandle(a_WindowHandle)->m_GLRenderingcontextHandle);
		//GetWindowByHandle(a_WindowHandle)->InitializeGL();
		break;
	}
	case WM_DESTROY:
	{
		GetWindowByHandle(a_WindowHandle)->WindowShouldClose = true;
		GetWindowByHandle(a_WindowHandle)->ShutDownWindow();
		break;
	}
	case WM_MOVE:
	{
		GetWindowByHandle(a_WindowHandle)->m_Position[0] = LOWORD(a_LongParam);
		GetWindowByHandle(a_WindowHandle)->m_Position[1] = HIWORD(a_LongParam);
		
		break;
	}

	case WM_SIZE:
	{
		if (GetWindowByHandle(a_WindowHandle)->m_GLRenderingcontextHandle)
		{	
			GetWindowByHandle(a_WindowHandle)->m_Resolution[0] = (GLuint)LOWORD(a_LongParam);
			GetWindowByHandle(a_WindowHandle)->m_Resolution[1] =(GLuint)HIWORD(a_LongParam);
			break;
		}
		break;
	}

	case WM_SIZING:
	{
		if (GetWindowByHandle(a_WindowHandle)->m_GLRenderingcontextHandle)
		{
			GetWindowByHandle(a_WindowHandle)->m_Resolution[0] = (GLuint)LOWORD(a_LongParam);
			GetWindowByHandle(a_WindowHandle)->m_Resolution[1] = (GLuint)HIWORD(a_LongParam);
			break;
		}
		break;
	}

	case WM_KEYDOWN:
	{
		GetWindowByHandle(a_WindowHandle)->Win32TranslateKey(a_WordParam, a_LongParam, KEYSTATE_DOWN);

		break;
	}

	case WM_KEYUP:
	{
		GetWindowByHandle(a_WindowHandle)->Win32TranslateKey(a_WordParam, a_LongParam, KEYSTATE_UP);
		break;
	}

	case WM_MOUSEMOVE:
	{
		GetWindowByHandle(a_WindowHandle)->m_MousePosition[0] = (GLuint)LOWORD(a_LongParam);
		GetWindowByHandle(a_WindowHandle)->m_MousePosition[1] = (GLuint)HIWORD(a_LongParam);
		break;
	}

	case WM_LBUTTONDOWN:
	{
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_LBUTTONUP:
	{
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONUP;
		break;
	}

	case WM_RBUTTONDOWN:
	{
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_RBUTTONUP:
	{
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_MBUTTONDOWN:
	{
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_MBUTTONUP:
	{
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONUP;
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
	GetMessage(&m_Message, 0, 0, 0);
	TranslateMessage(&m_Message);
	DispatchMessage(&m_Message);
}

#endif

