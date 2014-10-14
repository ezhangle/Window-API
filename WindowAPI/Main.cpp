#include <stdio.h>
#include "WindowManager.h"

int main()
{
	//Foundation_Window* l_Window = new Foundation_Window("Blarg");
	Foundation_WindowManager::AddWindow(new Foundation_Window("Blarg"))->
		AddWindow(new Foundation_Window("Penish"));

	printf("%i\n", Foundation_WindowManager::GetNumWindows());

	Foundation_WindowManager::InitializeWindows();
	while (!Foundation_WindowManager::GetWindowByIndex(0)->GetKey(KEY_ESCAPE))
	{

		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (GLuint i = 0; i < Foundation_WindowManager::GetNumWindows(); i++)
		{
			Foundation_WindowManager::GetWindowByIndex(i)->PollForEvents();
			Foundation_WindowManager::GetWindowByIndex(i)->Window_SwapBuffers();
		}
	}

	return 0;
}
