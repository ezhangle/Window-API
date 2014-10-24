#include <stdio.h>
#include "WindowManager.h"

int main()
{
	
	//Foundation_Window* l_Window = new Foundation_Window("Blarg");
	Foundation_WindowManager::Initialize();
	Foundation_WindowManager::AddWindow(new Foundation_Window("Blarg"));

//	Foundation_WindowManager::GetWindowByIndex(0)->SetResolution(800, 600);	
	//const GLubyte* l_Renderer = glGetString(GL_RENDERER);
	printf("Renderer: %s Version: %s\n", glGetString(GL_RENDERER), glGetString(GL_VERSION));
	while (!Foundation_WindowManager::GetWindowByIndex(0)->GetKey(KEY_ESCAPE) ||
		!Foundation_WindowManager::GetWindowByIndex(0)->GetWindowShouldClose())
	{
		Foundation_WindowManager::PollForEvents();
		for (GLuint i = 0; i < Foundation_WindowManager::GetNumWindows(); i++)
		{
			//Foundation_WindowManager::SetMousePositionInScreen(0, 0);
			Foundation_WindowManager::GetWindowByIndex(i)->MakeCurrentContext();

			/*glViewport(0, 0, Foundation_WindowManager::GetWindowByIndex(0)->GetResolution()[0],
					Foundation_WindowManager::GetWindowByIndex(0)->GetResolution()[1]);*/

			glClearColor(1.0f, 0.25f, 0.25f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			Foundation_WindowManager::GetWindowByIndex(i)->Window_SwapBuffers();
		}
	}
	
	return 0;
}
