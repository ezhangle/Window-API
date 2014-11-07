#include "Window.h"
#include "WindowManager.h"

#if defined(CURRENT_OS_WINDOWS)
HWND F_W::GetWindowHandle()
{
	return m_WindowHandle;
}

void F_W::InitializePixelFormat()
{
	m_PFD = {
		sizeof(PIXELFORMATDESCRIPTOR),  /* size */
		1,                              /* version */
		PFD_SUPPORT_OPENGL |
		PFD_DRAW_TO_WINDOW |
		PFD_DOUBLEBUFFER,               /* support double-buffering */
		PFD_TYPE_RGBA,                  /* color type */
		m_ColourBits, 0,                             /* prefered color depth */
		0, 0,
		0, 0,
		0, 0,
		0,                /* color bits (ignored) */                              /* no alpha buffer */                              /* alpha bits (ignored) */
		0,                              /* no accumulation buffer */
		0, 0, 0, 0,                     /* accum bits (ignored) */
		m_DepthBits,                    /* depth buffer */
		m_StencilBits,                  /* no stencil buffer */
		0,                              /* no auxiliary buffers */
		PFD_MAIN_PLANE,                 /* main layer */
		0,                              /* reserved */
		0, 0, 0,                        /* no layer, visible, damage masks */
	};

	int l_PixelFormat = ChoosePixelFormat(m_DeviceContextHandle, &m_PFD);

	if (l_PixelFormat)
	{
		SetPixelFormat(m_DeviceContextHandle, l_PixelFormat, &m_PFD);
		return;
	}
	return;
}

void F_W::InitializeWin32(LPCSTR a_MenuName, 
	UINT a_Style /* = CS_OWNDC | CS_HREDRAW | CS_DROPSHADOW */,
	int a_ClearScreenExtra /* = 0 */, 
	int a_WindowExtra /* = 0 */,
	HINSTANCE a_Instance /* = GetModuleHandle(0) */,
	HICON a_Icon /* = LoadIcon(0 , IDI_APPLICATION)*/, 
	HCURSOR a_Cursor /* = LoadCursor(0 , IDC_ARROW)*/,
	HBRUSH a_Brush /* = (HBRUSH)BLACK_BRUSH */)
{
	m_WindowClass.style = a_Style;
	m_WindowClass.lpfnWndProc = F_WM::StaticWindowProcedure;
	m_WindowClass.cbClsExtra = 0;
	m_WindowClass.cbWndExtra = 0;
	m_WindowClass.hInstance = a_Instance;
	m_WindowClass.hIcon = a_Icon;
	m_WindowClass.hCursor = a_Cursor;
	m_WindowClass.hbrBackground = a_Brush;
	m_WindowClass.lpszMenuName = a_MenuName;
	m_WindowClass.lpszClassName = a_MenuName;
	RegisterClass(&m_WindowClass);

	m_WindowHandle =
		CreateWindow(a_MenuName, a_MenuName, WS_OVERLAPPEDWINDOW, 0,
		0, m_Resolution[0],
		m_Resolution[1],
		0, 0, 0, 0);

	ShowWindow(m_WindowHandle, true);
	UpdateWindow(m_WindowHandle);
}

void F_W::Win32TranslateKey(WPARAM a_WordParam, LPARAM a_LongParam, bool a_KeyState)
{
	switch (a_WordParam)
	{
		case VK_ESCAPE:
		{
			m_Keys[KEY_ESCAPE] = a_KeyState;
			break;
		}

		case VK_F1:
		{
			m_Keys[KEY_F1] = a_KeyState;
			break;
		}

		case VK_F2:
		{
			m_Keys[KEY_F2] = a_KeyState;
			break;
		}

		case VK_F3:
		{
			m_Keys[KEY_F3] = a_KeyState;
			break;
		}

		case VK_F4:
		{
			m_Keys[KEY_F4] = a_KeyState;
			break;
		}

		case VK_F5:
		{
			m_Keys[KEY_F5] = a_KeyState;
			break;
		}

		case VK_F6:
		{
			m_Keys[KEY_F6] = a_KeyState;
			break;
		}

		case VK_F7:
		{
			m_Keys[KEY_F7] = a_KeyState;
			break;
		}

		case VK_F8:
		{
			m_Keys[KEY_F8] = a_KeyState;
			break;
		}

		case VK_F9:
		{
			m_Keys[KEY_F9] = a_KeyState;
			break;
		}

		case VK_F10:
		{
			m_Keys[KEY_F10] = a_KeyState;
			break;
		}

		case VK_F11:
		{
			m_Keys[KEY_F11] = a_KeyState;
			break;
		}

		case VK_F12:
		{
			m_Keys[KEY_F12] = a_KeyState;
			break;
		}

		case VK_BACK:
		{
			m_Keys[KEY_BACKSPACE] = a_KeyState;
			break;
		}

		case VK_TAB:
		{
			m_Keys[KEY_TAB] = a_KeyState;
			break;
		}

		case VK_CAPITAL:
		{
			m_Keys[KEY_CAPSLOCK] = a_KeyState;
			break;
		}

		case VK_SHIFT:
		{
			//m_Keys
			break;
		}

		case VK_CONTROL:
		{
			break;
		}

		case VK_MENU:
		{
			break;
		}

		case VK_RETURN:
		{
			m_Keys[KEY_ENTER] = a_KeyState;
			break;
		}

		case VK_PRINT:
		{
			m_Keys[KEY_PRINTSCREEN] = a_KeyState;
			break;
		}

		case VK_SCROLL:
		{
			m_Keys[KEY_SCROLLLOCK] = a_KeyState;
			break;
		}

		case VK_PAUSE:
		{
			m_Keys[KEY_PAUSE] = a_KeyState;
			break;
		}

		case VK_INSERT:
		{
			m_Keys[KEY_INSERT] = a_KeyState;
			break;
		}

		case VK_HOME:
		{
			m_Keys[KEY_HOME] = a_KeyState;
			break;
		}

		case VK_DELETE:
		{
			m_Keys[KEY_DELETE] = a_KeyState;
			break;
		}

		case VK_END:
		{
			m_Keys[KEY_END] = a_KeyState;
			break;
		}

		case VK_DOWN:
		{
			m_Keys[KEY_ARROW_DOWN] = a_KeyState;
			break;
		}

		case VK_UP:
		{
			m_Keys[KEY_ARROW_UP] = a_KeyState;
			break;
		}

		case VK_LEFT:
		{
			m_Keys[KEY_ARROW_LEFT] = a_KeyState;
			break;
		}

		case VK_RIGHT:
		{
			m_Keys[KEY_ARROW_RIGHT] = a_KeyState;
			break;
		}

		case VK_DIVIDE:
		{
			m_Keys[KEY_KEYPAD_DIVIDE] = a_KeyState;
			break;
		}

		case VK_MULTIPLY:
		{
			m_Keys[KEY_KEYPAD_MULTIPLY] = a_KeyState;
			break;
		}

		case VK_SUBTRACT:
		{
			m_Keys[KEY_KEYPAD_DIVIDE] = a_KeyState;
			break;
		}

		case VK_ADD:
		{
			m_Keys[KEY_KEYPAD_ADD] = a_KeyState;
			break;
		}

		case VK_DECIMAL:
		{
			m_Keys[KEY_KEYPAD_PERIOD] = a_KeyState;
			break;
		}

		case VK_NUMPAD0:
		{
			m_Keys[KEY_KEYPAD_0] = a_KeyState;
			break;
		}

		case VK_NUMPAD1:
		{
			m_Keys[KEY_KEYPAD_1] = a_KeyState;
			break;
		}

		case VK_NUMPAD2:
		{
			m_Keys[KEY_KEYPAD_2] = a_KeyState;
			break;
		}

		case VK_NUMPAD3:
		{
			m_Keys[KEY_KEYPAD_3] = a_KeyState;
			break;
		}

		case VK_NUMPAD4:
		{
			m_Keys[KEY_KEYPAD_4] = a_KeyState;
			break;
		}

		case VK_NUMPAD5:
		{
			m_Keys[KEY_KEYPAD_5] = a_KeyState;
			break;
		}

		case VK_NUMPAD6:
		{
			m_Keys[KEY_KEYPAD_6] = a_KeyState;
			break;
		}

		case VK_NUMPAD7:
		{
			m_Keys[KEY_KEYPAD_7] = a_KeyState;
			break;
		}

		case VK_NUMPAD8:
		{
			m_Keys[KEY_KEYPAD_8] = a_KeyState;
			break;
		}

		case VK_NUMPAD9:
		{
			m_Keys[KEY_KEYPAD_9] = a_KeyState;
			break;
		}
		default:
		{
			return;
		}
	}
}


#endif
