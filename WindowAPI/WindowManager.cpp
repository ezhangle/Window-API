#include "WindowManager.h"

Foundation_WindowManager::Foundation_WindowManager()
{

}

Foundation_WindowManager::~Foundation_WindowManager()
{

}

Foundation_Window* Foundation_WindowManager::GetWindowByName(const char* a_WindowName)
{
	for(GLuint l_Iter = 0; l_Iter <= m_Windows.size(); l_Iter++)
	{
		if(m_Windows[l_Iter]->GetWindowName() == a_WindowName)
		{
			return m_Windows[l_Iter];
		}
	}

return nullptr;	
}

Foundation_Window* Foundation_WindowManager::GetWindowByIndex(GLuint a_WindowIndex)
{
	if(a_WindowIndex < m_Windows.size() - 1)
	{
		return m_Windows[a_WindowIndex];
	}

	return nullptr;
}

Foundation_WindowManager* Foundation_WindowManager::AddWindow(Foundation_Window* a_Window)
{
	if(a_Window != nullptr || a_Window != 0)
	{
		m_Windows.push_back(a_Window);
		return this;
	}
	return nullptr;	
}


