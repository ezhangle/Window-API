#include <stdio.h>
#include "WindowManager.h"

void OnWindowKeyPressed(GLuint KeySym, GLboolean KeyState)
{
    if(KeySym == KEY_WINDOW_LEFT && KeyState == KEYSTATE_DOWN)
	{
		printf("1234\n");
	}
}

int main()
{
	WindowManager::Initialize();
	WindowManager::AddWindow(new FWindow("Example"))->AddWindow(
		new FWindow("Example2"));

	WindowManager::SetWindowOnKeyEvent("Example", &OnWindowKeyPressed);
	
	while (!WindowManager::GetWindowShouldClose("Example"))
	{
		WindowManager::PollForEvents();
		
		for (GLuint i = 0; i < WindowManager::GetNumWindows(); i++)
		{
			WindowManager::GetWindowByIndex(i)->MakeCurrentContext();
			glClearColor(1.0f, 0.25f, 0.25f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			WindowManager::GetWindowByIndex(i)->SwapDrawBuffers();
		}
	}	

	WindowManager::ShutDown();
	return 0;
}
