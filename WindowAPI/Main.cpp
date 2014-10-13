#include <stdio.h>
#include "Window.h"

int main()
{
	Foundation_Window::Initialize("Blarg");

	while (Foundation_Window::GetInstance()->GetKey(KEY_ESCAPE) != KEYSTATE_DOWN)
	{		
		glClearColor(0.0f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Foundation_Window::GetInstance()->Window_SwapBuffers();
		Foundation_Window::GetInstance()->PollForEvents();		
	}
	return 0;
}
