#ifndef TOOLS_H
#define TOOLS_H
#include "WindowAPI_Defs.h"

class WindowManager;

class Foundation_Tools
{
public:

   static GLboolean IsValid(const char* String)
	{
		return (String != nullptr);
	}

	static GLboolean IsValid(OnKeyEvent OnKeyPressed)
	{
		return (OnKeyPressed != nullptr);
	}

	static GLboolean IsValid(OnMouseWheelEvent OnMouseWheelEvent)
	{
		return (OnMouseWheelEvent != nullptr);
	}

	static GLboolean IsValid(OnMaximizedEvent OnMaximized)
	{
		return (OnMaximized != nullptr);
	}

	static GLboolean IsValid(OnFocusEvent OnFocus)
	{
		return (OnFocus != nullptr);
	}

	static GLboolean IsValid(OnMovedEvent OnMoved)
	{
		return (OnMoved != nullptr);
	}

	static GLboolean IsValid(OnMouseMoveEvent OnMouseMove)
	{
		return (OnMouseMove != nullptr);
	}

	static void PrintSupportedExtensions()
	{
#if defined(CONTEXT_CREATED)
		const char* SupportedExtensions = glGetString(GL_EXTENSIONS);
		printf("%s \n", SupportedExtensions);
#else
		printf("error: an OpenGL context must first be created \n");
#endif
	}

	static const char* GetSupportedExtensions()
	{
#if defined(CONTEXT_CREATED)
		return glGetString(GL_EXTENSIONS);
#else
		printf("error: an OpenGL context must first be created \n");
		return nullptr;
#endif
	}

	static void PrintOpenGLVersion()
	{
#if defined(CONTEXT_CREATED)
		const char* OpenGLVersions = glGetString(GL_VERSION);
		printf("%s \n", OpenGLVersions);
#else
		printf("error: an OpenGL context must first be created \n");
#endif
	}

	static const char* GetOpenGLVersion()
	{
#if defined(CONTEXT_CREATED)
		return glGetString(GL_VERSION)
#else
		printf("error: an OpenGL context must first be created \n");
		return nullptr;
#endif
	}

	static void PrintWarningMessage(GLuint WarningNumber)
	{
		switch(WarningNumber)
		{
			case WARNING_NOGLEXTENSIONS:
				{
					printf("Warning: no OpenGL extensions available \n");
					break;
				}

			case WARNING_NOTCURRENTCONTEXT:
				{
					printf("Warning: window not the current OpenGL context being rendered to \n");
					break;
				}

			default:
				{
					printf("Warning: unsecified warning \n");
					break;
				}
		}
	}


	static GLboolean PrintErrorMessage(GLuint ErrorNumber)
	{
		switch(ErrorNumber)
		{
			case ERROR_NOCONTEXT:
				{
					printf("Error: An OpenGL context must first be created(initialize the window) \n");
					return GL_FALSE;
				}

			case ERROR_INVALIDWINDOWNAME:
				{
					printf("Error: invalid window anme \n");
					return GL_FALSE;
				}

			case ERROR_INVALIDWINDOWSTATE:
				{
					printf("Error: invalid window state \n");
					return GL_FALSE;
				}

			case ERROR_INVALIDRESOLUTION:
				{
					printf("Error: invalid resolution \n");
					return GL_FALSE;
				}

			case ERROR_INVALIDCONTEXT:
				{
					printf("Error: Failed to create OpenGL context \n");
					return GL_FALSE;
				}

			case ERROR_LINUX_CANNOTCONNECTXSERVER:
				{
					printf("Error: cannot connect to X server \n");
					return GL_FALSE;
				}

			case ERROR_LINUX_INVALIDVISUALINFO:
				{
					printf("Error: Invalid visual information given \n");
					return GL_FALSE;
				}

			case ERROR_LINUX_CANNOTCREATEWINDOW:
				{
					printf("Error: failed to create window \n");
					return GL_FALSE;

			default:
				{
					printf("Error: unspecified Error \n");
					return GL_FALSE;
				}	
		}
	}
	}

	friend class WindowManager;

private:
	//these are internal. do not mess with these
#if defined(CURRENT_OS_WINDOWS)
	static GLuint Windows_TranslateKey(WPARAM WordParam, LPARAM LongParam);

#endif

#if defined(CURRENT_OS_LINUX)
	static GLuint Linux_TranslateKey(GLuint KeySym);

	static const char* Linux_GetEventType(XEvent Event);
#endif
};

#endif
