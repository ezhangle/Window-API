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
	WindowManager::AddWindow(new FWindow("Example"));

	WindowManager::SetWindowOnKeyEvent("Example", &OnWindowKeyPressed);
	printf("blarg\n");	
	while (!WindowManager::GetWindowShouldClose("Example"))
	{
		WindowManager::PollForEvents();
		glClearColor(1.0f, 0.25f, 0.25f, 1.0f);
		
		for (GLuint i = 0; i < WindowManager::GetNumWindows(); i++)
		{
			printf("Blarg2\n");
			WindowManager::GetWindowByIndex(i)->MakeCurrentContext();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			WindowManager::GetWindowByIndex(i)->SwapDrawBuffers();
		}
	}	

	WindowManager::ShutDown();
	return 0;
}
