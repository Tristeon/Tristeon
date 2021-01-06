#include "Window.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif defined(__linux__)
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#endif

namespace Tristeon
{
	void Window::populateResolutions()
	{
#if defined(_WIN32)
		DEVMODE dm{ 0 };
		int i = 0;
		dm.dmSize = sizeof(dm);

		while (EnumDisplaySettingsA(nullptr, i, &dm))
		{
			i += 1;
			_resolutions.insert(VectorU{ (unsigned int)dm.dmPelsWidth, (unsigned int)dm.dmPelsHeight });
		}
#elif defined(__linux__)
        Display *dpy = XOpenDisplay(":0");
        auto root = DefaultRootWindow(dpy);
        auto sc = XRRGetScreenInfo (dpy, root);
        int nsize = 0;
        auto sizes = XRRConfigSizes(sc, &nsize);
        for (int i = 0; i < nsize; i++) {
            _resolutions.insert(VectorU { sizes[i].width, sizes[i].height });
        }
#else
#error This platform isnt supported (yet)
#endif
	}
}