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


#ifdef _MSC_VER
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

Foundation_WindowManager* Foundation_WindowManager::AddWindow(Foundation_Window* a_Window)
{
	GetInstance()->m_Windows.push_back(a_Window);
	a_Window->m_WindowID = GetInstance()->m_Windows.size() - 1;
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

void Foundation_WindowManager::InitializeWindows()
{
	if (!m_Instance->m_Windows.empty())
	{
		for (int i = 0; i < m_Instance->m_Windows.size(); i++)
		{
			m_Instance->m_Windows[i]->Initialize();
		}
	}
}

#ifdef _MSC_VER
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
		GetWindowByHandle(a_WindowHandle)->InitializeGL();
		break;
	}
	case WM_DESTROY:
	{
		GetWindowByHandle(a_WindowHandle)->ShutDownWindow();
		break;
	}

	case WM_SIZE:
	{
		if (GetWindowByHandle(a_WindowHandle)->m_GLRenderingcontextHandle)
		{	
			GetWindowByHandle(a_WindowHandle)->Resize((unsigned int)LOWORD(a_LongParam), (unsigned int)HIWORD(a_LongParam));
			break;
		}
		break;
	}

	case WM_SIZING:
	{
		if (GetWindowByHandle(a_WindowHandle)->m_GLRenderingcontextHandle)
		{
			GetWindowByHandle(a_WindowHandle)->Resize((unsigned int)LOWORD(a_LongParam), (unsigned int)HIWORD(a_LongParam));
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

	case WM_LBUTTONDOWN:
	{
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_LBUTTONUP:
	{
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONUP;
		m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONUP;
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
		m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONDOWN;
		break;
	}

	case WM_MBUTTONUP:
	{
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONUP;
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

#ifdef _MSC_VER
LRESULT CALLBACK Foundation_WindowManager::StaticWindowProcedure(HWND a_WindowHandle, UINT a_Message, WPARAM a_WordParam, LPARAM a_LongParam)
{
	return Foundation_WindowManager::GetInstance()->WindowProcedure(a_WindowHandle, a_Message, a_WordParam, a_LongParam);
}
#endif


Foundation_WindowManager* Foundation_WindowManager::m_Instance = 0;


