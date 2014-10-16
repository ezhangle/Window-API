#include "WindowManager.h"

Foundation_WindowManager::Foundation_WindowManager()
{

}

Foundation_WindowManager::~Foundation_WindowManager()
{
	if(!m_Windows.empty())
	{
		for(GLuint l_Iter = 0; l_Iter <= m_Windows.size(); l_Iter++)
		{
			delete m_Windows[l_Iter];
		}

		m_Windows.clear();
	}
}

Foundation_Window* Foundation_WindowManager::GetWindowByName(const char* a_WindowName)
{
	if(a_WindowName != nullptr || a_WindowName != 0)
	{
		for (GLuint l_Iter = 0; l_Iter <= GetInstance()->m_Windows.size(); l_Iter++)
		{
			if(GetInstance()->m_Windows[l_Iter]->GetWindowName() == a_WindowName)
			{
				return GetInstance()->m_Windows[l_Iter];
			}
		}

		return nullptr;	
	}

	return nullptr;
}

Foundation_Window* Foundation_WindowManager::GetWindowByIndex(GLuint a_WindowIndex)
{
	if (a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		return GetInstance()->m_Windows[a_WindowIndex];
	}

	return nullptr;
}

void Foundation_WindowManager::GetMousePositionInScreen(GLuint& a_X, GLuint& a_Y)
{
#if defined(_MSC_VER_) || defined(_WIN32) || defined(_WIN64)


	POINT l_Point;

	if (GetCursorPos(&l_Point))
	{
		a_X = l_Point.x;
		a_Y = l_Point.y;
	}

#endif

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)

#endif

}

GLuint* Foundation_WindowManager::GetMousePositionInScreen()
{
#if defined(_MSC_VER_) || defined(_WIN32) || defined(_WIN64)

	POINT l_Point;

	if (GetCursorPos(&l_Point))
	{
		GetInstance()->m_ScreenMousePosition[0] = l_Point.x;
		GetInstance()->m_ScreenMousePosition[1] = l_Point.y;
	}

#endif

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)

#endif



	return GetInstance()->m_ScreenMousePosition;
}

GLuint* Foundation_WindowManager::GetScreenResolution()
{
#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
	RECT l_Screen;
	HWND m_Desktop = GetDesktopWindow();
	GetWindowRect(m_Desktop, &l_Screen);

	GetInstance()->m_ScreenResolution[0] = l_Screen.right;
	GetInstance()->m_ScreenResolution[1] = l_Screen.bottom;
	return GetInstance()->m_ScreenResolution;

#endif
}

void Foundation_WindowManager::GetScreenResolution(GLuint& a_Width, GLuint& a_Height)
{
#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)

	RECT l_Screen;
	HWND m_Desktop = GetDesktopWindow();
	GetWindowRect(m_Desktop, &l_Screen);
	a_Width = l_Screen.right;
	a_Height = l_Screen.bottom;
#endif


}

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
Foundation_Window* Foundation_WindowManager::GetWindowByHandle(HWND a_WindowHandle)
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
#endif

#if defined(__linux__) || defined(__GNUG__) || defined(__GNUC__) || defined(__clang__)
Foundation_Window* Foundation_WindowManager::GetWindowByHandle(Window a_WindowHandle)
{
	for (GLuint l_Iter = 0; l_Iter < GetInstance()->m_Windows.size(); l_Iter++)
	{
		if (GetInstance()->m_Windows[l_Iter]->GetWindowHandle() == a_WindowHandle)
		{
			return GetInstance()->m_Windows[l_Iter];
		}
	}
}

#endif

Foundation_WindowManager* Foundation_WindowManager::AddWindow(Foundation_Window* a_Window)
{
	GetInstance()->m_Windows.push_back(a_Window);
	a_Window->m_WindowID = GetInstance()->m_Windows.size() - 1;
	a_Window->InitializeGL();
	return m_Instance;
}

Foundation_WindowManager* Foundation_WindowManager::GetInstance()
{
	if(!Foundation_WindowManager::m_Instance)
	{
		Foundation_WindowManager::m_Instance = new Foundation_WindowManager();
		return Foundation_WindowManager::m_Instance;
	}

	else 
	{		
		return Foundation_WindowManager::m_Instance;
	}
}

GLuint Foundation_WindowManager::GetNumWindows()
{
	return GetInstance()->m_Windows.size();
}

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
LRESULT CALLBACK Foundation_WindowManager::WindowProcedure(HWND a_WindowHandle, UINT a_Message, WPARAM a_WordParam, LPARAM a_LongParam)
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

#endif

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
LRESULT CALLBACK Foundation_WindowManager::StaticWindowProcedure(HWND a_WindowHandle, UINT a_Message, WPARAM a_WordParam, LPARAM a_LongParam)
{
	return Foundation_WindowManager::GetInstance()->WindowProcedure(a_WindowHandle, a_Message, a_WordParam, a_LongParam);
}
#endif




Foundation_WindowManager* Foundation_WindowManager::m_Instance = 0;
