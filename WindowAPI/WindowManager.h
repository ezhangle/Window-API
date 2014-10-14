#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H
#include <stdio.h>
#include <stdlib.h>
#include <vector>
<<<<<<< HEAD
#include <Windows.h>
#include "GL/GL.h"
#include "Keys.h"
#include "Window.h"

class Foundation_Window;

=======
#include "Window.h"

>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
class Foundation_WindowManager
{
	public:
		Foundation_WindowManager();
		~Foundation_WindowManager();

<<<<<<< HEAD
		static Foundation_Window* GetWindowByName(const char* a_WindowName);
		static Foundation_Window* GetWindowByIndex(GLuint a_WindowIndex);

		static Foundation_WindowManager* AddWindow(Foundation_Window* a_Window);
		static Foundation_WindowManager* GetInstance();
		static GLuint GetNumWindows();
		static void InitializeWindows();

		friend Foundation_Window;
=======
		Foundation_Window* GetWindowByName(const char* a_WindowName);
		Foundation_Window* GetWindowByIndex(GLuint a_WindowIndex);

		Foundation_WindowManager* AddWindow(Foundation_Window* a_Window);
		static Foundation_WindowManager* GetInstance();
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066

	private:

		std::vector<Foundation_Window*> m_Windows;
		static Foundation_WindowManager* m_Instance;
<<<<<<< HEAD

=======
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
#ifdef _MSC_VER
		LRESULT CALLBACK WindowProcedure(HWND a_WindowHandle, GLuint a_Message, WPARAM a_WordParam, LPARAM a_LongParam);
	
		static LRESULT CALLBACK StaticWindowProcedure(HWND a_WindowHandle, UINT a_Message, WPARAM a_WordParam, LPARAM a_LongParam);
<<<<<<< HEAD

		static Foundation_Window* GetWindowByHandle(HWND a_WindowHandle);

=======
>>>>>>> 2e0397aa3527a8f258506bd46c0cd142b195a066
#endif
};
#endif 
