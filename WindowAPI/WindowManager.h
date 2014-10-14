#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#ifdef _MSC_VER
	#include <Windows.h>
	#include <GL/GL.h>
#endif

#ifdef __linux__ || __GNUG__ || __GNUC__ || __clang__
#include <GL/gl.h>
#endif
#include "Keys.h"
#include "Window.h"

class Foundation_Window;

class Foundation_WindowManager
{
	public:
		Foundation_WindowManager();
		~Foundation_WindowManager();

		static Foundation_Window* GetWindowByName(const char* a_WindowName);
		static Foundation_Window* GetWindowByIndex(GLuint a_WindowIndex);

		static Foundation_WindowManager* AddWindow(Foundation_Window* a_Window);
		static Foundation_WindowManager* GetInstance();
		static GLuint GetNumWindows();
		static void InitializeWindows();

		friend Foundation_Window;
	private:

		std::vector<Foundation_Window*> m_Windows;
		static Foundation_WindowManager* m_Instance;

#ifdef _MSC_VER
		LRESULT CALLBACK WindowProcedure(HWND a_WindowHandle, GLuint a_Message, WPARAM a_WordParam, LPARAM a_LongParam);
	
		static LRESULT CALLBACK StaticWindowProcedure(HWND a_WindowHandle, UINT a_Message, WPARAM a_WordParam, LPARAM a_LongParam);

		static Foundation_Window* GetWindowByHandle(HWND a_WindowHandle);
#endif
};
#endif 
