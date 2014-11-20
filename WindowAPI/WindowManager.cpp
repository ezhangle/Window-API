#include "WindowManager.h"
#include "Tools.h"

F_WM::F_WM()
{

}

void F_WM::Initialize()
{
#if defined(CURRENT_OS_LINUX)
	Linux_Initialize();
#endif	

#if defined(CURRENT_OS_WINDOWS)
	Windows_Initialize();
#endif
}

F_WM::~F_WM()
{
	if(!GetInstance()->m_Windows.empty())
	{
		for(GLuint l_Iter = 0; l_Iter <= GetInstance()->m_Windows.size() - 1; l_Iter++)
		{
			delete GetInstance()->m_Windows[l_Iter];
		}

		GetInstance()->m_Windows.clear();
	}
}

F_W* F_WM::GetWindowByName(const char* a_WindowName)
{
	if(a_WindowName != nullptr)
	{
		for (GLuint l_Iter = 0; l_Iter <= GetInstance()->m_Windows.size() - 1; l_Iter++)
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

F_W* F_WM::GetWindowByIndex(GLuint a_WindowIndex)
{
	if (a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		return GetInstance()->m_Windows[a_WindowIndex];
	}

	return nullptr;
}

F_WM* F_WM::AddWindow(F_W* a_Window)
{
	if(a_Window != nullptr)
	{
		GetInstance()->m_Windows.push_back(a_Window);
		a_Window->m_WindowID = GetInstance()->m_Windows.size() - 1;
		a_Window->Initialize();
		return GetInstance();
	}
	return nullptr;
}

F_WM* F_WM::GetInstance()
{
	if(!F_WM::m_Instance)
	{
		F_WM::m_Instance = new F_WM();
		return F_WM::m_Instance;
	}

	else 
	{		
		return F_WM::m_Instance;
	}
}

GLuint F_WM::GetNumWindows()
{
	return GetInstance()->m_Windows.size();
}

void F_WM::ShutDown()
{	
	for(GLuint l_CurrentWindow = 0; l_CurrentWindow < GetInstance()->m_Windows.size() - 1; l_CurrentWindow++)
	{
		delete GetInstance()->m_Windows[l_CurrentWindow];
	}

	GetInstance()->m_Windows.clear();

#if defined(CURRENT_OS_WINDOWS)
#endif

#if defined(CURRENT_OS_LINUX)
	XCloseDisplay(GetInstance()->m_Display);
#endif

	delete m_Instance;
}

void F_WM::GetMousePositionInScreen(GLuint& a_X, GLuint& a_Y)
{
	a_X = GetInstance()->m_ScreenMousePosition[0];
	a_Y = GetInstance()->m_ScreenMousePosition[1];
}

GLuint* F_WM::GetMousePositionInScreen()
{
	return GetInstance()->m_ScreenMousePosition;
}

void F_WM::SetMousePositionInScreen(GLuint a_X, GLuint a_Y)
{
#if defined(CURRENT_OS_LINUX)
	Linux_SetMousePositionInScreen(a_X, a_Y);
#endif
}

GLuint* F_WM::GetScreenResolution()
{
#if defined(CURRENT_OS_WINDOWS)
	RECT l_Screen;
	HWND m_Desktop = GetDesktopWindow();
	GetWindowRect(m_Desktop, &l_Screen);

	GetInstance()->m_ScreenResolution[0] = l_Screen.right;
	GetInstance()->m_ScreenResolution[1] = l_Screen.bottom;
	return GetInstance()->m_ScreenResolution;

#endif

#if defined(CURRENT_OS_LINUX)
	GetInstance()->m_ScreenResolution[0] = WidthOfScreen(XDefaultScreenOfDisplay(GetInstance()->m_Display));
	GetInstance()->m_ScreenResolution[1] = HeightOfScreen(XDefaultScreenOfDisplay(GetInstance()->m_Display));
	//

	return GetInstance()->m_ScreenResolution;
#endif
}

void F_WM::PollForEvents()
{
#if defined(CURRENT_OS_WINDOWS)
	GetInstance()->Windows_PollForEvents();
#endif

#if defined (CURRENT_OS_LINUX)
	GetInstance()->Linux_PollForEvents();
#endif
}



void F_WM::GetScreenResolution(GLuint& a_Width, GLuint& a_Height)
{
#if defined(CURRENT_OS_WINDOWS)

	RECT l_Screen;
	HWND m_Desktop = GetDesktopWindow();
	GetWindowRect(m_Desktop, &l_Screen);
	a_Width = l_Screen.right;
	a_Height = l_Screen.bottom;
#endif

#if defined(CURRENT_OS_LINUX)

	a_Width = WidthOfScreen(XDefaultScreenOfDisplay(GetInstance()->m_Display));
	a_Height = HeightOfScreen(XDefaultScreenOfDisplay(GetInstance()->m_Display));


	GetInstance()->m_ScreenResolution[0] = a_Width;
	GetInstance()->m_ScreenResolution[1] = a_Height;
#endif
}

void F_WM::GetWindowResolution(const char* a_WindowName, GLuint& a_Width, GLuint& a_Height)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->GetResolution(a_Width, a_Height);
	}
}

void F_WM::GetWindowResolution(GLuint a_WindowIndex, GLuint& a_Width, GLuint& a_Height)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->GetResolution(a_Width, a_Height);
	}
}

GLuint* F_WM::GetWindowResolution(const char* a_WindowName)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		return GetWindowByName(a_WindowName)->GetResolution();
	}

	return nullptr;
}

GLuint* F_WM::GetWindowResolution(GLuint a_WindowIndex)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		return GetWindowByIndex(a_WindowIndex)->GetResolution();
	}

	return nullptr;
}

void F_WM::SetWindowResolution(const char* a_WindowName, GLuint a_Width, GLuint a_Height)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->SetResolution(a_Width, a_Height);
	}
}

void F_WM::SetWindowResolution(GLuint a_WindowIndex, GLuint a_Width, GLuint a_Height)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->SetResolution(a_Width, a_Height);
	}
}

void F_WM::GetWindowPosition(const char* a_WindowName, GLuint& a_X, GLuint& a_Y)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->GetPosition(a_X, a_Y);
	}
}

void F_WM::GetWindowPosition(GLuint a_WindowIndex, GLuint& a_X, GLuint& a_Y)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->GetPosition(a_X, a_Y);
	}
}

GLuint* F_WM::GetWindowPosition(const char* a_WindowName)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		return GetWindowByName(a_WindowName)->GetPosition();
	}

	return nullptr;
}

GLuint* F_WM::GetWindowPosition(GLuint a_WindowIndex)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() -1)
	{
		return GetWindowByIndex(a_WindowIndex)->GetPosition();
	}

	return nullptr;
}

void F_WM::SetWindowPosition(const char* a_WindowName, GLuint a_X, GLuint a_Y)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->SetPosition(a_X, a_Y);
	}
}

void F_WM::SetWindowPosition(GLuint a_WindowIndex, GLuint a_X, GLuint a_Y)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() -1)
	{
		GetWindowByIndex(a_WindowIndex)->SetPosition(a_X, a_Y);
	}
}

void F_WM::GetMousePositionInWindow(const char* a_WindowName, GLuint& a_X, GLuint& a_Y)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->GetMousePosition(a_X, a_Y);
	}
}

void F_WM::GetMousePositionInWindow(GLuint a_WindowIndex, GLuint& a_X, GLuint& a_Y)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->GetMousePosition(a_X, a_Y);
	}
}

GLuint* F_WM::GetMousePositionInWindow(const char* a_WindowName)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		return GetWindowByName(a_WindowName)->GetMousePosition();
	}

	return nullptr;
}

GLuint* F_WM::GetMousePositionInWindow(GLuint a_WindowIndex)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		return GetWindowByIndex(a_WindowIndex)->GetMousePosition();
	}

	return nullptr;
}

void F_WM::SetMousePositionInWindow(const char* a_WindowName, GLuint a_X, GLuint a_Y)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->SetMousePosition(a_X, a_Y);
	}
}

void F_WM::SetMousePositionInWindow(GLuint a_WindowIndex, GLuint a_X, GLuint a_Y)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->SetMousePosition(a_X, a_Y);
	}
}

bool F_WM::WindowGetKey(const char* a_WindowName, GLuint a_Key)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		return GetWindowByName(a_WindowName)->GetKeyState(a_Key);
	}

	return false;
}

bool F_WM::WindowGetKey(GLuint a_WindowIndex, GLuint a_Key)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		return GetWindowByIndex(a_WindowIndex)->GetKeyState(a_Key);
	}

	return false;
}

bool F_WM::GetWindowShouldClose(const char* a_WindowName)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		return GetWindowByName(a_WindowName)->GetShouldClose();
	}

	return false;
}

bool F_WM::GetWindowShouldClose(GLuint a_WindowIndex)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		return GetWindowByIndex(a_WindowIndex)->GetShouldClose();
	}

	return false;
}

void F_WM::WindowSwapBuffers(const char* a_WindowName)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->SwapDrawBuffers();
	}
}

void F_WM::WindowSwapBuffers(GLuint a_WindowIndex)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->SwapDrawBuffers();
	}
}

bool F_WM::GetWindowIsFullScreen(const char* a_WindowName)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		return GetWindowByName(a_WindowName)->GetIsFullScreen();
	}

	return false;
}

bool F_WM::GetWindowIsFullScreen(GLuint a_WindowIndex)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() -1)
	{
		return GetWindowByIndex(a_WindowIndex)->GetIsFullScreen();
	}

	return false;
}

void F_WM::SetFullScreen(const char* a_WindowName, bool a_NewState)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->FullScreen(a_NewState);
	}
}

void F_WM::SetFullScreen(GLuint a_WindowIndex, bool a_NewState)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->FullScreen(a_NewState);
	}
}

bool F_WM::GetWindowIsMinimized(const char* a_WindowName)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		return GetWindowByName(a_WindowName)->GetIsMinimized();
	}

	return false;
}

bool F_WM::GetWindowIsMinimized(GLuint a_WindowIndex)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		return GetWindowByIndex(a_WindowIndex)->GetIsMinimized();
	}

	return false;
}

void F_WM::MinimizeWindow(const char* a_WindowName, bool a_NewState)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->Minimize(a_NewState);
	}
}

void F_WM::MinimizeWindow(GLuint a_WindowIndex, bool a_NewState)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->Minimize(a_NewState);
	}
}

bool F_WM::GetWindowIsMaximized(const char* a_WindowName)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		return GetWindowByName(a_WindowName)->GetIsMaximized();
	}

	return false;
}

bool F_WM::GetWindowIsMaximized(GLuint a_WindowIndex)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		return GetWindowByIndex(a_WindowIndex)->GetIsMaximized();
	}

	return false;
}

void F_WM::MaximizeWindow(const char* a_WindowName, bool a_NewState)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->Maximize(a_NewState);
	}
}

void F_WM::MaximizeWindow(GLuint a_WindowIndex, bool a_NewState)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->Maximize(a_NewState);
	}
}

const char* F_WM::GetWindowName(GLuint a_WindowIndex)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		return GetWindowByIndex(a_WindowIndex)->GetWindowName();
	}

	return nullptr;
}

GLuint F_WM::GetWindowIndex(const char* a_WindowName)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		return GetWindowByName(a_WindowName)->m_WindowID;
	}

	return 0;
}

void F_WM::SetWindowName(const char* a_WindowName, const char* a_NewName)
{
	if(Foundation_Tools::IsValid(a_WindowName) && Foundation_Tools::IsValid(a_NewName))
	{
		GetWindowByName(a_WindowName)->SetName(a_NewName);
	}
}

void F_WM::SetWindowName(GLuint a_WindowIndex, const char* a_NewName)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1 && Foundation_Tools::IsValid(a_NewName))
	{
		GetWindowByIndex(a_WindowIndex)->SetName(a_NewName);
	}
}

bool F_WM::GetWindowIsInFocus(const char* a_WindowName)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		return GetWindowByName(a_WindowName)->GetInFocus();
	}

	return false;
}

bool F_WM::GetWindowIsInFocus(GLuint a_WindowIndex)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		return GetWindowByIndex(a_WindowIndex)->GetInFocus();
	}

	return false;
}

void F_WM::FocusWindow(const char* a_WindowName, bool a_NewState)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->Focus(a_NewState);
	}
}

void F_WM::FocusWindow(GLuint a_WindowIndex, bool a_NewState)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->Focus(a_NewState);
	}
}

void F_WM::RestoreWindow(const char* a_WindowName)
{
	if(Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->Restore();
	}
	//implement window focusing
}

void F_WM::RestoreWindow(GLuint a_WindowIndex)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->Restore();
	}
	//implement window focusing
}

void F_WM::SetWindowSwapInterval(const char* a_WindowName, GLint a_SyncSetting)
{
	if (Foundation_Tools::IsValid(a_WindowName))
	{
		return GetWindowByName(a_WindowName)->SetSwapInterval(a_SyncSetting);
	}
}

void F_WM::SetWindowSwapInterval(GLuint a_WindowIndex, GLint a_SyncSetting)
{
	if (a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		return GetWindowByIndex(a_WindowIndex)->SetSwapInterval(a_SyncSetting);
	}
}

void F_WM::SetWindowOnKeyEvent(const char* a_WindowName, OnKeyEvent a_OnKeyEvent)
{
	if (Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->SetOnKeyEvent(a_OnKeyEvent);
	}
}

void F_WM::SetWindowOnKeyEvent(GLuint a_WindowIndex, OnKeyEvent a_OnKeyEvent)
{
	if (a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->SetOnKeyEvent(a_OnKeyEvent);
	}
}

void F_WM::SetWindowOnMouseButtonEvent(const char* a_WindowName, OnMouseButtonEvent a_OnMouseButtonEvent)
{
	if (Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->SetOnMouseButtonEvent(a_OnMouseButtonEvent);
	}
}

void F_WM::SetWindowOnMouseButtonEvent(GLuint a_WindowIndex, OnMouseButtonEvent a_OnMouseButtonEvent)
{
	if (a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->SetOnMouseButtonEvent(a_OnMouseButtonEvent);
	}
}

void F_WM::SetWindowOnMouseWheelEvent(const char* a_WindowName, OnMouseWheelEvent a_OnMouseWheelEvent)
{
	if (Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->SetOnMouseWheelEvent(a_OnMouseWheelEvent);
	}
}

void F_WM::SetWindowOnMouseWheelEvent(GLuint a_WindowIndex, OnMouseWheelEvent a_OnMouseWheelEvent)
{
	if (a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->SetOnMouseWheelEvent(a_OnMouseWheelEvent);
	}
}

void F_WM::SetWindowOnDestroyed(const char* a_WindowName, OnDestroyed a_OnDestroyed)
{
	if (Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->SetOnDestroyed(a_OnDestroyed);
	}
}

void F_WM::SetWindowOnDestroyed(GLuint a_WindowIndex, OnDestroyed a_OnDestroyed)
{
	if (a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->SetOnDestroyed(a_OnDestroyed);
	}
}

void F_WM::SetWindowOnMaximized(const char* a_WindowName, OnMaximized a_OnMaximized)
{
	if (Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->SetOnMaximized(a_OnMaximized);
	}
}

void F_WM::SetWindowOnMaximized(GLuint a_WindowIndex, OnMaximized a_OnMaximized)
{
	if (a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->SetOnMaximized(a_OnMaximized);
	}
}

void F_WM::SetWindowOnMinimized(const char* a_WindowName, OnMinimized a_OnMiniimzed)
{
	if (Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->SetOnMinimized(a_OnMiniimzed);
	}
}

void F_WM::SetWindowOnMinimized(GLuint a_WindowIndex, OnMinimized a_OnMiniimzed)
{
	if (a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->SetOnMinimized(a_OnMiniimzed);
	}
}

void F_WM::SetWindowOnRestored(const char* a_WindowName, OnRestored a_OnRestored)
{
	if (Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->SetOnRestored(a_OnRestored);
	}
}

void F_WM::SetWindowOnRestored(GLuint a_WindowIndex, OnRestored a_OnRestored)
{
	if (a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->SetOnRestored(a_OnRestored);
	}
}

void F_WM::SetWindowOnFocus(const char* a_WindowName, OnFocus a_OnFocus)
{
	if(Foundation_Tools::IsValid(a_OnFocus))
	{
		GetWindowByName(a_WindowName)->m_OnFocus = a_OnFocus;
	}
}

void F_WM::SetWindowOnFocus(GLuint a_WindowIndex, OnFocus a_OnFocus)
{
	if(a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->m_OnFocus = a_OnFocus;
	}
}

void F_WM::SetWindowOnMoved(const char* a_WindowName, OnMoved a_OnMoved)
{
	if (Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->SetOnMoved(a_OnMoved);
	}
}

void F_WM::SetWindowOnMoved(GLuint a_WindowIndex, OnMoved a_OnMoved)
{
	if (a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->SetOnMoved(a_OnMoved);
	}
}

void F_WM::SetWindowOnResize(const char* a_WindowName, OnResize a_OnResize)
{
	if (Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->SetOnResize(a_OnResize);
	}
}

void F_WM::SetWindowOnResize(GLuint a_WindowIndex, OnResize a_OnResize)
{
	if (a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->SetOnResize(a_OnResize);
	}
}

void F_WM::SetWindowOnMouseMove(const char* a_WindowName, OnMouseMove a_OnMouseMove)
{
	if (Foundation_Tools::IsValid(a_WindowName))
	{
		GetWindowByName(a_WindowName)->SetOnMouseMove(a_OnMouseMove);
	}
}

void F_WM::SetWindowOnMouseMove(GLuint a_WindowIndex, OnMouseMove a_OnMouseMove)
{
	if (a_WindowIndex <= GetInstance()->m_Windows.size() - 1)
	{
		GetWindowByIndex(a_WindowIndex)->SetOnMouseMove(a_OnMouseMove);
	}
}

F_WM* F_WM::m_Instance = 0;
