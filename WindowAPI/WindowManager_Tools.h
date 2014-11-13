#ifndef WINDOWMANAGER_TOOLS_H
#define WINDOWMANAGER_TOOLS_H

#include "WindowManager.h"
#include <limits.h>
#if defined(CURRENT_OS_LINUX)
class WindowManager_Tools
{
	public:

	static unsigned long Linux_GetWindowProperty(Display* a_Display, 
			Window a_Window, Atom a_Property, Atom a_Type, unsigned char** a_Value)
	{
		Atom l_ActualType;
		int l_ActualFormat;
		unsigned long l_ItemCount, l_BytesAfter;

		XGetWindowProperty(a_Display, a_Window, 
			   a_Property, 0, LONG_MAX, 
			   False, AnyPropertyType, 
			   &l_ActualType, 
			   &l_ActualFormat, 
			   &l_ItemCount,
			   &l_BytesAfter,
			   a_Value);

		if(l_ActualType != a_Type)
		{
			return 0;
		}

		return l_ItemCount;
	}	

	static const char* Linux_GetEventType(XEvent a_Event)
	{
		switch(a_Event.type)
		{
			case MotionNotify:
				{
					return "Motion Notify Event\n";
				}

			case ButtonPress:
				{
					return "Button Press Event\n";
				}

			case ButtonRelease:
				{
					return "Button Release Event\n";
				}

			case ColormapNotify:
				{
					return "Color Map Notify event \n";
				}

			case EnterNotify:
				{
					return "Enter Notify Event\n";
				}

			case LeaveNotify:
				{
					return "Leave Notify Event\n";
				}
				
			case Expose:
				{
					return "Expose Event\n";
				}

			case GraphicsExpose:
				{
					return "Graphics expose event\n";
				}

			case NoExpose:
				{
					return "No Expose Event\n";
				}

			case FocusIn:
				{
					return "Focus In Event\n";
				}

			case FocusOut:
				{
					return "Focus Out Event\n";
				}

			case KeymapNotify:
				{
					return "Key Map Notify Event\n";
				}

			case KeyPress:
				{
					return "Key Press Event\n";
				}

			case KeyRelease:
				{
					return "Key Release Event\n";
				}

			case PropertyNotify:
				{
					return "Property Notfiy Event\n";
				}

			case ResizeRequest:
				{
					return "Resize Property Event\n";
				}

			case CirculateNotify:
				{
					return "Circulate Notify Event\n";
				}

			case ConfigureNotify:
				{
					return "configure Notify Event\n";
				}

			case DestroyNotify:
				{
					return "Destroy Notify Request\n";
				}

			case GravityNotify:
				{
					return "Gravity Notify Event \n";
				}

			case MapNotify:
				{
					return "Map Notify Event\n";
				}

			case ReparentNotify:
				{
					return "Reparent Notify Event\n";
				}

			case UnmapNotify:
				{
					return "Unmap notify event\n";
				}

			case MapRequest:
				{
					return "Map request event\n";
				}

			case ClientMessage:
				{
					return "Client Message Event\n";
				}

			case MappingNotify:
				{
					return "Mapping notify event\n";
				}

			case SelectionClear:
				{
					return "Selection Clear event\n";
				}

			case SelectionNotify:
				{
					return "Selection Notify Event\n";
				}

			case SelectionRequest:
				{
					return "Selection Request event\n";
				}

			case VisibilityNotify:
				{
					return "Visibility Notify Event\n";
				}

			default:
				{
					return 0;
				}
		}
	}

};


#endif

#endif
