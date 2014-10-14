#include "WindowManager.h"

<<<<<<< HEAD

=======
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
Foundation_WindowManager::Foundation_WindowManager()
{

}

Foundation_WindowManager::~Foundation_WindowManager()
{
<<<<<<< HEAD
	m_Windows.clear();
=======
	if(!m_Windows.empty())
	{
		for(GLuint l_Iter = 0; l_Iter <= m_Windows.size(); l_Iter++)
		{
			delete m_Windows[l_Iter];
		}

		m_Windows.clear();
	}
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
}

Foundation_Window* Foundation_WindowManager::GetWindowByName(const char* a_WindowName)
{
	if(a_WindowName != nullptr || a_WindowName != 0)
	{
	
<<<<<<< HEAD
		for (GLuint l_Iter = 0; l_Iter <= GetInstance()->m_Windows.size(); l_Iter++)
		{
			if(GetInstance()->m_Windows[l_Iter]->GetWindowName() == a_WindowName)
			{
				return GetInstance()->m_Windows[l_Iter];
=======
		for(GLuint l_Iter = 0; l_Iter <= m_Windows.size(); l_Iter++)
		{
			if(m_Windows[l_Iter]->GetWindowName() == a_WindowName)
			{
				return m_Windows[l_Iter];
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
			}
		}

		return nullptr;	
	}

	return nullptr;
}

Foundation_Window* Foundation_WindowManager::GetWindowByIndex(GLuint a_WindowIndex)
{
<<<<<<< HEAD
	if (a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		return GetInstance()->m_Windows[a_WindowIndex];
=======
	if(a_WindowIndex < m_Windows.size() - 1)
	{
		return m_Windows[a_WindowIndex];
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
	}

	return nullptr;
}

<<<<<<< HEAD
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
=======
Foundation_WindowManager* Foundation_WindowManager::AddWindow(Foundation_Window* a_Window)
{
	if(a_Window != nullptr || a_Window != 0)
	{
		m_Windows.push_back(a_Window);
		return this;
	}
	return nullptr;	
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
}

Foundation_WindowManager* Foundation_WindowManager::GetInstance()
{
<<<<<<< HEAD
	if(!Foundation_WindowManager::m_Instance)
	{
		Foundation_WindowManager::m_Instance = new Foundation_WindowManager();
		return Foundation_WindowManager::m_Instance;
=======
	if(!m_Instance)
	{
		m_Instance = new Foundation_WindowManager();
		return m_Instance;
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
	}

	else 
	{		
<<<<<<< HEAD
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
=======
		return m_Instance;
	}
}

#ifdef _MSC_VER
LRESULT CALLBACK Foundation_Window::WindowProcedure(HWND a_WindowHandle, UINT a_Message, WPARAM a_WordParam, LPARAM a_LongParam)
{
	if(m_WindowHandle == a_WindowHandle)
	{
		printf("It's AliVe!!!");
	}

>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
	switch (a_Message)
	{
	case WM_CREATE:
	{
<<<<<<< HEAD
		GetWindowByIndex(m_Windows.size() - 1)->m_WindowHandle = a_WindowHandle;
		GetWindowByHandle(a_WindowHandle)->m_DeviceContextHandle = GetDC(GetWindowByHandle(a_WindowHandle)->m_WindowHandle);
		GetWindowByHandle(a_WindowHandle)->InitializePixelFormat();
		GetWindowByHandle(a_WindowHandle)->m_GLRenderingcontextHandle = wglCreateContext(GetWindowByHandle(a_WindowHandle)->m_DeviceContextHandle);
		wglMakeCurrent(GetWindowByHandle(a_WindowHandle)->m_DeviceContextHandle, GetWindowByHandle(a_WindowHandle)->m_GLRenderingcontextHandle);
		GetWindowByHandle(a_WindowHandle)->InitializeGL();
=======
		m_DeviceContextHandle = GetDC(m_WindowHandle);
		InitializePixelFormat();
		m_GLRenderingcontextHandle = wglCreateContext(m_DeviceContextHandle);
		wglMakeCurrent(m_DeviceContextHandle, m_GLRenderingcontextHandle);
		InitializeGL();
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
		break;
	}
	case WM_DESTROY:
	{
		/* finish OpenGL rendering */
<<<<<<< HEAD
		GetWindowByHandle(a_WindowHandle)->ShutDownWindow();
=======
		ShutDownWindow();
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
		break;
	}

	case WM_SIZE:
	{
<<<<<<< HEAD
		if (GetWindowByHandle(a_WindowHandle)->m_GLRenderingcontextHandle)
		{	
			GetWindowByHandle(a_WindowHandle)->Resize((unsigned int)LOWORD(a_LongParam), (unsigned int)HIWORD(a_LongParam));
=======
		if (m_GLRenderingcontextHandle)
		{	
			Resize((unsigned int)LOWORD(m_LongParam), (unsigned int)HIWORD(m_LongParam));
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
			break;
		}
		break;
	}

	case WM_SIZING:
	{
<<<<<<< HEAD
		if (GetWindowByHandle(a_WindowHandle)->m_GLRenderingcontextHandle)
		{
			GetWindowByHandle(a_WindowHandle)->Resize((unsigned int)LOWORD(a_LongParam), (unsigned int)HIWORD(a_LongParam));
=======
		if (m_GLRenderingcontextHandle)
		{
			Resize((unsigned int)LOWORD(m_LongParam), (unsigned int)HIWORD(m_LongParam));
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
			break;
		}
		break;
	}

	case WM_KEYDOWN:
	{
<<<<<<< HEAD
		GetWindowByHandle(a_WindowHandle)->Win32TranslateKey(a_WordParam, a_LongParam, KEYSTATE_DOWN);
=======
		TranslateKey(m_WordParam, m_LongParam, KEYSTATE_DOWN);
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
		break;
	}

	case WM_KEYUP:
	{
<<<<<<< HEAD
		GetWindowByHandle(a_WindowHandle)->Win32TranslateKey(a_WordParam, a_LongParam, KEYSTATE_UP);
=======
		TranslateKey(a_WordParam, a_LongParam, KEYSTATE_UP);
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
		break;
	}

	case WM_LBUTTONDOWN:
	{
<<<<<<< HEAD
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONDOWN;
=======
		m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONDOWN;
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
		break;
	}

	case WM_LBUTTONUP:
	{
<<<<<<< HEAD
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONUP;
=======
		m_MouseEvents[MOUSE_LEFTBUTTON] = MOUSE_BUTTONUP;
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
		break;
	}

	case WM_RBUTTONDOWN:
	{
<<<<<<< HEAD
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONDOWN;
=======
		m_MouseEvents[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONDOWN;
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
		break;
	}

	case WM_RBUTTONUP:
	{
<<<<<<< HEAD
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONDOWN;
=======
		m_MouseEvents[MOUSE_RIGHTBUTTON] = MOUSE_BUTTONDOWN;
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
		break;
	}

	case WM_MBUTTONDOWN:
	{
<<<<<<< HEAD
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONDOWN;
=======
		m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONDOWN;
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
		break;
	}

	case WM_MBUTTONUP:
	{
<<<<<<< HEAD
		GetWindowByHandle(a_WindowHandle)->m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONUP;
=======
		m_MouseEvents[MOUSE_MIDDLEBUTTON] = MOUSE_BUTTONUP;
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
		break;
	}

	default:
	{
<<<<<<< HEAD
		return DefWindowProc(a_WindowHandle, a_Message, a_WordParam, a_LongParam);
=======
		return DefWindowProc(m_WindowHandle, a_Message, m_WordParam, m_LongParam);
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
	}
	}
	return 0;
}

#endif

#ifdef _MSC_VER
<<<<<<< HEAD
LRESULT CALLBACK Foundation_WindowManager::StaticWindowProcedure(HWND a_WindowHandle, UINT a_Message, WPARAM a_WordParam, LPARAM a_LongParam)
{
	return Foundation_WindowManager::GetInstance()->WindowProcedure(a_WindowHandle, a_Message, a_WordParam, a_LongParam);
=======
LRESULT CALLBACK Foundation_Window::StaticWindowProcedure(HWND a_WindowHandle, UINT a_Message, WPARAM a_WordParam, LPARAM a_LongParam)
{
	return WindowProcedure(a_WindowHandle, a_Message, a_WordParam, a_LongParam);
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
}
#endif


Foundation_WindowManager* Foundation_WindowManager::m_Instance = 0;


