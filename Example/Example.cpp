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
    if(KeySym == ' ' && KeyState == KEYSTATE_DOWN)
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
	WindowManager::Initialize();
	WindowManager::AddWindow(new FWindow("Example"));
	WindowManager::GetWindowByName("Example")->PrintOpenGLVersion();
	WindowManager::SetWindowOnKeyEvent("Example", &OnWindowKeyPressed);

	while (!WindowManager::GetWindowShouldClose("Example"))
	{
		WindowManager::PollForEvents(); // or waitForEvents
		for (GLuint i = 0; i < WindowManager::GetNumWindows(); i++,)
		{
			WindowManager::GetWindowByIndex(i)->MakeCurrentContext();
			glClearColor(0.25f, 0.25f, 0.25f, 0.25f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			WindowManager::GetWindowByIndex(i)->SwapDrawBuffers();
		}
	}	

	WindowManager::ShutDown();
	return 0;
}
