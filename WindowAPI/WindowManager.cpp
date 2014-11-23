#include "WindowManager.h"
#include "Tools.h"

WindowManager::WindowManager()
{

}

void WindowManager::Initialize()
{
#if defined(CURRENT_OS_LINUX)
	Linux_Initialize();
#endif	

#if defined(CURRENT_OS_WINDOWS)
	Windows_Initialize();
#endif
}

WindowManager::~WindowManager()
{
	if(!GetInstance()->Windows.empty())
	{
		for(GLuint l_Iter = 0; l_Iter <= GetInstance()->Windows.size() - 1; l_Iter++)
		{
			delete GetInstance()->Windows[l_Iter];
		}

		GetInstance()->Windows.clear();
	}
}

Window* WindowManager::GetWindowByName(const char* WindowName)
{
	if(WindowName != nullptr)
	{
		for (GLuint l_Iter = 0; l_Iter <= GetInstance()->Windows.size() - 1; l_Iter++)
		{
			if(GetInstance()->Windows[l_Iter]->GetWindowName() == WindowName)
			{
				return GetInstance()->Windows[l_Iter];
			}
		}

		return nullptr;	
	}

	return nullptr;
}

Window* WindowManager::GetWindowByIndex(GLuint WindowIndex)
{
	if (WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		return GetInstance()->Windows[WindowIndex];
	}

	return nullptr;
}

WindowManager* WindowManager::AddWindow(Window* Window)
{
	if(Window != nullptr)
	{
		GetInstance()->Windows.push_back(Window);
		Window->ID = GetInstance()->Windows.size() - 1;
		Window->Initialize();
		return GetInstance();
	}
	return nullptr;
}

WindowManager* WindowManager::GetInstance()
{
	if(!WindowManager::Instance)
	{
		WindowManager::Instance = new WindowManager();
		return WindowManager::Instance;
	}

	else 
	{		
		return WindowManager::Instance;
	}
}

GLuint WindowManager::GetNumWindows()
{
	return GetInstance()->Windows.size();
}

void WindowManager::ShutDown()
{	
	for(GLuint l_CurrentWindow = 0; l_CurrentWindow < GetInstance()->Windows.size() - 1; l_CurrentWindow++)
	{
		delete GetInstance()->Windows[l_CurrentWindow];
	}

	GetInstance()->Windows.clear();

#if defined(CURRENT_OS_WINDOWS)
#endif

#if defined(CURRENT_OS_LINUX)
	XCloseDisplay(GetInstance()->m_Display);
#endif

	delete Instance;
}

void WindowManager::GetMousePositionInScreen(GLuint& X, GLuint& Y)
{
	X = GetInstance()->ScreenMousePosition[0];
	Y = GetInstance()->ScreenMousePosition[1];
}

GLuint* WindowManager::GetMousePositionInScreen()
{
	return GetInstance()->ScreenMousePosition;
}

void WindowManager::SetMousePositionInScreen(GLuint X, GLuint Y)
{
#if defined(CURRENT_OS_LINUX)
	Linux_SetMousePositionInScreen(X, Y);
#endif
}

GLuint* WindowManager::GetScreenResolution()
{
#if defined(CURRENT_OS_WINDOWS)
	RECT l_Screen;
	HWND m_Desktop = GetDesktopWindow();
	GetWindowRect(m_Desktop, &l_Screen);

	GetInstance()->ScreenResolution[0] = l_Screen.right;
	GetInstance()->ScreenResolution[1] = l_Screen.bottom;
	return GetInstance()->ScreenResolution;

#endif

#if defined(CURRENT_OS_LINUX)
	GetInstance()->ScreenResolution[0] = WidthOfScreen(XDefaultScreenOfDisplay(GetInstance()->m_Display));
	GetInstance()->ScreenResolution[1] = HeightOfScreen(XDefaultScreenOfDisplay(GetInstance()->m_Display));
	//

	return GetInstance()->ScreenResolution;
#endif
}

void WindowManager::PollForEvents()
{
#if defined(CURRENT_OS_WINDOWS)
	GetInstance()->Windows_PollForEvents();
#endif

#if defined (CURRENT_OS_LINUX)
	GetInstance()->Linux_PollForEvents();
#endif
}



void WindowManager::GetScreenResolution(GLuint& Width, GLuint& Height)
{
#if defined(CURRENT_OS_WINDOWS)

	RECT l_Screen;
	HWND m_Desktop = GetDesktopWindow();
	GetWindowRect(m_Desktop, &l_Screen);
	Width = l_Screen.right;
	Height = l_Screen.bottom;
#endif

#if defined(CURRENT_OS_LINUX)

	Width = WidthOfScreen(XDefaultScreenOfDisplay(GetInstance()->m_Display));
	Height = HeightOfScreen(XDefaultScreenOfDisplay(GetInstance()->m_Display));


	GetInstance()->ScreenResolution[0] = Width;
	GetInstance()->ScreenResolution[1] = Height;
#endif
}

void WindowManager::GetWindowResolution(const char* WindowName, GLuint& Width, GLuint& Height)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->GetResolution(Width, Height);
	}
}

void WindowManager::GetWindowResolution(GLuint WindowIndex, GLuint& Width, GLuint& Height)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->GetResolution(Width, Height);
	}
}

GLuint* WindowManager::GetWindowResolution(const char* WindowName)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		return GetWindowByName(WindowName)->GetResolution();
	}

	return nullptr;
}

GLuint* WindowManager::GetWindowResolution(GLuint WindowIndex)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		return GetWindowByIndex(WindowIndex)->GetResolution();
	}

	return nullptr;
}

void WindowManager::SetWindowResolution(const char* WindowName, GLuint Width, GLuint Height)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->SetResolution(Width, Height);
	}
}

void WindowManager::SetWindowResolution(GLuint WindowIndex, GLuint Width, GLuint Height)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->SetResolution(Width, Height);
	}
}

void WindowManager::GetWindowPosition(const char* WindowName, GLuint& X, GLuint& Y)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->GetPosition(X, Y);
	}
}

void WindowManager::GetWindowPosition(GLuint WindowIndex, GLuint& X, GLuint& Y)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->GetPosition(X, Y);
	}
}

GLuint* WindowManager::GetWindowPosition(const char* WindowName)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		return GetWindowByName(WindowName)->GetPosition();
	}

	return nullptr;
}

GLuint* WindowManager::GetWindowPosition(GLuint WindowIndex)
{
	if(WindowIndex <= GetInstance()->Windows.size() -1)
	{
		return GetWindowByIndex(WindowIndex)->GetPosition();
	}

	return nullptr;
}

void WindowManager::SetWindowPosition(const char* WindowName, GLuint X, GLuint Y)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->SetPosition(X, Y);
	}
}

void WindowManager::SetWindowPosition(GLuint WindowIndex, GLuint X, GLuint Y)
{
	if(WindowIndex <= GetInstance()->Windows.size() -1)
	{
		GetWindowByIndex(WindowIndex)->SetPosition(X, Y);
	}
}

void WindowManager::GetMousePositionInWindow(const char* WindowName, GLuint& X, GLuint& Y)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->GetMousePosition(X, Y);
	}
}

void WindowManager::GetMousePositionInWindow(GLuint WindowIndex, GLuint& X, GLuint& Y)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->GetMousePosition(X, Y);
	}
}

GLuint* WindowManager::GetMousePositionInWindow(const char* WindowName)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		return GetWindowByName(WindowName)->GetMousePosition();
	}

	return nullptr;
}

GLuint* WindowManager::GetMousePositionInWindow(GLuint WindowIndex)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		return GetWindowByIndex(WindowIndex)->GetMousePosition();
	}

	return nullptr;
}

void WindowManager::SetMousePositionInWindow(const char* WindowName, GLuint X, GLuint Y)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->SetMousePosition(X, Y);
	}
}

void WindowManager::SetMousePositionInWindow(GLuint WindowIndex, GLuint X, GLuint Y)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->SetMousePosition(X, Y);
	}
}

GLboolean WindowManager::WindowGetKey(const char* WindowName, GLuint Key)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		return GetWindowByName(WindowName)->GetKeyState(Key);
	}

	return GL_FALSE;
}

GLboolean WindowManager::WindowGetKey(GLuint WindowIndex, GLuint Key)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		return GetWindowByIndex(WindowIndex)->GetKeyState(Key);
	}

	return GL_FALSE;
}

GLboolean WindowManager::GetWindowShouldClose(const char* WindowName)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		return GetWindowByName(WindowName)->GetShouldClose();
	}

	return GL_FALSE;
}

GLboolean WindowManager::GetWindowShouldClose(GLuint WindowIndex)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		return GetWindowByIndex(WindowIndex)->GetShouldClose();
	}

	return GL_FALSE;
}

void WindowManager::WindowSwapBuffers(const char* WindowName)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->SwapDrawBuffers();
	}
}

void WindowManager::WindowSwapBuffers(GLuint WindowIndex)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->SwapDrawBuffers();
	}
}

GLboolean WindowManager::GetWindowIsFullScreen(const char* WindowName)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		return GetWindowByName(WindowName)->GetIsFullScreen();
	}

	return GL_FALSE;
}

GLboolean WindowManager::GetWindowIsFullScreen(GLuint WindowIndex)
{
	if(WindowIndex <= GetInstance()->Windows.size() -1)
	{
		return GetWindowByIndex(WindowIndex)->GetIsFullScreen();
	}

	return GL_FALSE;
}

void WindowManager::SetFullScreen(const char* WindowName, GLboolean NewState)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->FullScreen(NewState);
	}
}

void WindowManager::SetFullScreen(GLuint WindowIndex, GLboolean NewState)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->FullScreen(NewState);
	}
}

GLboolean WindowManager::GetWindowIsMinimized(const char* WindowName)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		return GetWindowByName(WindowName)->GetIsMinimized();
	}

	return GL_FALSE;
}

GLboolean WindowManager::GetWindowIsMinimized(GLuint WindowIndex)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		return GetWindowByIndex(WindowIndex)->GetIsMinimized();
	}

	return GL_FALSE;
}

void WindowManager::MinimizeWindow(const char* WindowName, GLboolean NewState)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->Minimize(NewState);
	}
}

void WindowManager::MinimizeWindow(GLuint WindowIndex, GLboolean NewState)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->Minimize(NewState);
	}
}

GLboolean WindowManager::GetWindowIsMaximized(const char* WindowName)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		return GetWindowByName(WindowName)->GetIsMaximized();
	}

	return GL_FALSE;
}

GLboolean WindowManager::GetWindowIsMaximized(GLuint WindowIndex)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		return GetWindowByIndex(WindowIndex)->GetIsMaximized();
	}

	return GL_FALSE;
}

void WindowManager::MaximizeWindow(const char* WindowName, GLboolean NewState)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->Maximize(NewState);
	}
}

void WindowManager::MaximizeWindow(GLuint WindowIndex, GLboolean NewState)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->Maximize(NewState);
	}
}

const char* WindowManager::GetWindowName(GLuint WindowIndex)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		return GetWindowByIndex(WindowIndex)->GetWindowName();
	}

	return nullptr;
}

GLuint WindowManager::GetWindowIndex(const char* WindowName)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		return GetWindowByName(WindowName)->ID;
	}

	return 0;
}

void WindowManager::SetWindowTitleBar(const char* WindowName, const char* NewTitle)
{
	if(Foundation_Tools::IsValid(WindowName) && Foundation_Tools::IsValid(NewTitle))
	{
		GetWindowByName(WindowName)->SetTitleBar(NewTitle);
	}
}

void WindowManager::SetWindowTitleBar(GLuint WindowIndex, const char* NewTitle)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1 && Foundation_Tools::IsValid(NewTitle))
	{
		GetWindowByIndex(WindowIndex)->SetTitleBar(NewTitle);
	}
}

GLboolean WindowManager::GetWindowIsInFocus(const char* WindowName)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		return GetWindowByName(WindowName)->GetInFocus();
	}

	return GL_FALSE;
}

GLboolean WindowManager::GetWindowIsInFocus(GLuint WindowIndex)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		return GetWindowByIndex(WindowIndex)->GetInFocus();
	}

	return GL_FALSE;
}

void WindowManager::FocusWindow(const char* WindowName, GLboolean NewState)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->Focus(NewState);
	}
}

void WindowManager::FocusWindow(GLuint WindowIndex, GLboolean NewState)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->Focus(NewState);
	}
}

void WindowManager::RestoreWindow(const char* WindowName)
{
	if(Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->Restore();
	}
	//implement window focusing
}

void WindowManager::RestoreWindow(GLuint WindowIndex)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->Restore();
	}
	//implement window focusing
}

void WindowManager::SetWindowSwapInterval(const char* WindowName, GLint a_SyncSetting)
{
	if (Foundation_Tools::IsValid(WindowName))
	{
		return GetWindowByName(WindowName)->SetSwapInterval(a_SyncSetting);
	}
}

void WindowManager::SetWindowSwapInterval(GLuint WindowIndex, GLint a_SyncSetting)
{
	if (WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		return GetWindowByIndex(WindowIndex)->SetSwapInterval(a_SyncSetting);
	}
}

void WindowManager::SetWindowOnKeyEvent(const char* WindowName, OnKeyEvent OnKey)
{
	if (Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->SetOnKeyEvent(OnKey);
	}
}

void WindowManager::SetWindowOnKeyEvent(GLuint WindowIndex, OnKeyEvent OnKey)
{
	if (WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->SetOnKeyEvent(OnKey);
	}
}

void WindowManager::SetWindowOnMouseButtonEvent(const char* WindowName, OnMouseButtonEvent a_OnMouseButtonEvent)
{
	if (Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->SetOnMouseButtonEvent(a_OnMouseButtonEvent);
	}
}

void WindowManager::SetWindowOnMouseButtonEvent(GLuint WindowIndex, OnMouseButtonEvent a_OnMouseButtonEvent)
{
	if (WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->SetOnMouseButtonEvent(a_OnMouseButtonEvent);
	}
}

void WindowManager::SetWindowOnMouseWheelEvent(const char* WindowName, OnMouseWheelEvent OnMouseWheelEvent)
{
	if (Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->SetOnMouseWheelEvent(OnMouseWheelEvent);
	}
}

void WindowManager::SetWindowOnMouseWheelEvent(GLuint WindowIndex, OnMouseWheelEvent OnMouseWheelEvent)
{
	if (WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->SetOnMouseWheelEvent(OnMouseWheelEvent);
	}
}

void WindowManager::SetWindowOnDestroyed(const char* WindowName, OnDestroyedEvent OnDestroyed)
{
	if (Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->SetOnDestroyed(OnDestroyed);
	}
}

void WindowManager::SetWindowOnDestroyed(GLuint WindowIndex, OnDestroyedEvent OnDestroyed)
{
	if (WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->SetOnDestroyed(OnDestroyed);
	}
}

void WindowManager::SetWindowOnMaximized(const char* WindowName, OnMaximizedEvent OnMaximized)
{
	if (Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->SetOnMaximized(OnMaximized);
	}
}

void WindowManager::SetWindowOnMaximized(GLuint WindowIndex, OnMaximizedEvent OnMaximized)
{
	if (WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->SetOnMaximized(OnMaximized);
	}
}

void WindowManager::SetWindowOnMinimized(const char* WindowName, OnMinimizedEvent a_OnMiniimzed)
{
	if (Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->SetOnMinimized(a_OnMiniimzed);
	}
}

void WindowManager::SetWindowOnMinimized(GLuint WindowIndex, OnMinimizedEvent a_OnMiniimzed)
{
	if (WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->SetOnMinimized(a_OnMiniimzed);
	}
}

void WindowManager::SetWindowOnRestored(const char* WindowName, OnRestoredEvent OnRestored)
{
	if (Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->SetOnRestored(OnRestored);
	}
}

void WindowManager::SetWindowOnRestored(GLuint WindowIndex, OnRestoredEvent OnRestored)
{
	if (WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->SetOnRestored(OnRestored);
	}
}

void WindowManager::SetWindowOnFocus(const char* WindowName, OnFocusEvent OnFocus)
{
	if(Foundation_Tools::IsValid(OnFocus))
	{
		GetWindowByName(WindowName)->FocusEvent = OnFocus;
	}
}

void WindowManager::SetWindowOnFocus(GLuint WindowIndex, OnFocusEvent OnFocus)
{
	if(WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->FocusEvent = OnFocus;
	}
}

void WindowManager::SetWindowOnMoved(const char* WindowName, OnMovedEvent OnMoved)
{
	if (Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->SetOnMoved(OnMoved);
	}
}

void WindowManager::SetWindowOnMoved(GLuint WindowIndex, OnMovedEvent OnMoved)
{
	if (WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->SetOnMoved(OnMoved);
	}
}

void WindowManager::SetWindowOnResize(const char* WindowName, OnResizeEvent OnResize)
{
	if (Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->SetOnResize(OnResize);
	}
}

void WindowManager::SetWindowOnResize(GLuint WindowIndex, OnResizeEvent OnResize)
{
	if (WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->SetOnResize(OnResize);
	}
}

void WindowManager::SetWindowOnMouseMove(const char* WindowName, OnMouseMoveEvent OnMouseMove)
{
	if (Foundation_Tools::IsValid(WindowName))
	{
		GetWindowByName(WindowName)->SetOnMouseMove(OnMouseMove);
	}
}

void WindowManager::SetWindowOnMouseMove(GLuint WindowIndex, OnMouseMoveEvent OnMouseMove)
{
	if (WindowIndex <= GetInstance()->Windows.size() - 1)
	{
		GetWindowByIndex(WindowIndex)->SetOnMouseMove(OnMouseMove);
	}
}

WindowManager* WindowManager::Instance = 0;
