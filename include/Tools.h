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

	static GLboolean IsValid(OnMouseWheelEvent MouseWheelEvent)
	{
		return (MouseWheelEvent != nullptr);
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
					printf("Warning: unspecified warning \n");
					break;
				}
		}
	}


	static void PrintErrorMessage(GLuint ErrorNumber)
	{
		switch(ErrorNumber)
		{
			case ERROR_NOCONTEXT:
			{
				printf("Error: An OpenGL context must first be created(initialize the window) \n");
				break;
			}

			case ERROR_INVALIDWINDOWNAME:
			{
				printf("Error: invalid window name \n");
				break;
			}

			case ERROR_INVALIDWINDOWINDEX:
			{
				printf("Error: invalid window index \n");
				break;
			}

			case ERROR_INVALIDWINDOWSTATE:
			{
				printf("Error: invalid window state \n");
				break;
			}

			case ERROR_INVALIDRESOLUTION:
			{
				printf("Error: invalid resolution \n");
				break;
			}

			case ERROR_INVALIDCONTEXT:
			{
				printf("Error: Failed to create OpenGL context \n");
				break;
			}

			case ERROR_EXISTINGCONTEXT:
			{
				printf("Error: context already created \n");
				break;
			}

			case ERROR_NOTINITIALIZED:
			{
				printf("Error: Window manager not initialized \n");
				break;
			}

			case ERROR_ALREADYINITIALIZED:
			{
				printf("Error: window has already been initialized \n");
				break;
			}

			case ERROR_INVALIDTITLEBAR:
			{
				printf("Error: invalid title bar name (cannot be null or nullptr) \n");
				break;
			}

			case ERROR_INVALIDEVENT:
			{
				printf("Error: invalid event callback given \n");
				break;
			}

			case ERROR_WINDOWNOTFOUND:
			{
				printf("Error: window was not found \n");
				break;
			}

			case ERROR_INVALIDWINDOWSTYLE:
			{
				printf("Error: invalid window style given \n");
				break;
			}

			case ERROR_INVALIDWINDOW:
			{
				printf("Error: invalid window given \n");
				break;
			}

			case ERROR_FUNCTIONNOTIMPLEMENTED:
			{
				printf("Error: I'm sorry but this function has not been implemented yet :( \n");
				break;
			}

			case ERROR_LINUX_CANNOTCONNECTXSERVER:
			{
				printf("Error: cannot connect to X server \n");
				break;
			}

			case ERROR_LINUX_INVALIDVISUALINFO:
			{
				printf("Error: Invalid visual information given \n");
				break;
			}

			case ERROR_LINUX_CANNOTCREATEWINDOW:
			{
				printf("Error: failed to create window \n");
				break;
			}

			case ERROR_LINUX_FUNCTIONNOTIMPLEMENTED:
			{
				printf("Error: function not implemented on linux platform yet. sorry :( \n");
				break;
			}

			case ERROR_WINDOWS_CANNOTCREATEWINDOW:
			{
				printf("Error: failed to create window \n");
				break;
			}

			case ERROR_WINDOWS_FUNCTIONNOTIMPLEMENTED:
			{
				printf("Error: function not implemented on Windows platform yet. sorry ;( \n");
				break;
			}

			default:
			{
				printf("Error: unspecified Error \n");
				break;
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
