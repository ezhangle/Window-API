/**********************************************************************************************//**
 * @file	WindowAPI\Example.cpp
 *
 * @brief	Implements the example class.
 **************************************************************************************************/
#include <stdio.h>
#include "WindowManager.h"

/**********************************************************************************************//**
 * @fn	void OnWindowKeyPressed(GLuint KeySym, GLboolean KeyState)
 *
 * @brief	Executes the window key pressed action.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	KeySym  	The key symbol.
 * @param	KeyState	State of the key.
 **************************************************************************************************/

void OnWindowKeyPressed(GLuint KeySym, GLboolean KeyState)
{
    if(KeySym == KEY_RIGHTWINDOW && KeyState == KEYSTATE_DOWN)
	{
		printf("1234\n");
	}
}

/**********************************************************************************************//**
 * @fn	int main()
 *
 * @brief	Main entry-point for this application.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	Exit-code for the process - 0 for success, else an error code.
 **************************************************************************************************/

int main()
{
#if defined(DEBUG)
	printf("blarg \n");
#endif

	for (int i = 0; i < 20; i++)
	{
		printf("%i\n", 1L<<i);
	}
	WindowManager::Initialize();
	
	WindowManager::AddWindow(new FWindow("Example"));
	WindowManager::SetWindowOnKeyEvent("Example", &OnWindowKeyPressed);
	WindowManager::GetWindowByIndex(0)->EnableDecorator(DECORATOR_BORDER);
	while (!WindowManager::GetWindowShouldClose("Example"))
	{
		WindowManager::PollForEvents();
		glClearColor(1.0f, 0.25f, 0.25f, 1.0f);
		
		for (GLuint i = 0; i < WindowManager::GetNumWindows(); i++)
		{
			WindowManager::GetWindowByIndex(i)->MakeCurrentContext();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			WindowManager::GetWindowByIndex(i)->SwapDrawBuffers();
		}
	}	

	WindowManager::ShutDown();
	return 0;
}
