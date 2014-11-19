#include <stdio.h>
#include "WindowManager.h"

void OnWindowKeyPressed(GLuint a_KeySym, bool a_KeyState)
{
    if(a_KeySym == KEY_WINDOW_LEFT && a_KeyState == KEYSTATE_DOWN)
	{
		printf("blarg \n");
	}
}

int main()
{
	F_WM::Initialize();
	F_WM::AddWindow(new F_W("Example"));

	F_WM::SetWindowOnKeyEvent("Example", &OnWindowKeyPressed);

	while (!F_WM::GetWindowShouldClose("Example"))
	{
		F_WM::PollForEvents();
		glClearColor(1.0f, 0.25f, 0.25f, 1.0f);
		for (GLuint i = 0; i < F_WM::GetNumWindows(); i++)
		{
			F_WM::GetWindowByIndex(i)->MakeCurrentContext();
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			F_WM::WindowSwapBuffers("Example");
		}
	}	

	F_WM::ShutDown();
	return 0;
}
