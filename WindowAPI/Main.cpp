#include <stdio.h>
#include "WindowManager.h"

int main()
{
	Foundation_WindowManager::Initialize();
	Foundation_WindowManager::AddWindow(new Foundation_Window("Blarg"));
	printf("Renderer: %s Version: %s\n", glGetString(GL_RENDERER), glGetString(GL_VERSION));
	while (!Foundation_WindowManager::WindowGetKey("Blarg", KEY_ESCAPE) ||
		!Foundation_WindowManager::GetWindowShouldClose("Blarg"))
	{
		Foundation_WindowManager::PollForEvents();
		for (GLuint i = 0; i < Foundation_WindowManager::GetNumWindows(); i++)
		{
			Foundation_WindowManager::GetWindowByIndex(i)->MakeCurrentContext();

			//Foundation_WindowManager::GetWindowByIndex(i)->SetWindowName("Penish");

			glClearColor(1.0f, 0.25f, 0.25f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			Foundation_WindowManager::WindowSwapBuffers("Blarg");
		}
	}
	
	return 0;
}
