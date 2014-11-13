#include <stdio.h>
#include "WindowManager.h"

void OnWindowKeyPressed(GLuint a_KeySym, bool a_KeyState)
{
    if(a_KeySym == ' ' && a_KeyState == KEYSTATE_DOWN)
	{
		F_WM::GetWindowByName("Blarg")->SetVerticalSync(true);
	}

	if(a_KeySym == KEY_ARROW_LEFT && a_KeyState == KEYSTATE_DOWN)
	{
		F_WM::SetFullScreen("Blarg", false);
	}
}

int main()
{
	F_WM::Initialize();
	F_WM::AddWindow(new F_W("Blarg"));

	F_WM::GetWindowByName("Blarg")->SetOnKeyEvent(&OnWindowKeyPressed);

	while (!F_WM::WindowGetKey("Blarg", KEY_ESCAPE) ||
		!F_WM::GetWindowShouldClose("Blarg"))
	{
		F_WM::PollForEvents();
		for (GLuint i = 0; i < F_WM::GetNumWindows(); i++)
		{
			F_WM::GetWindowByIndex(i)->MakeCurrentContext();
			glClearColor(1.0f, 0.25f, 0.25f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			F_WM::WindowSwapBuffers("Blarg");
		}
	}	
	return 0;
}
