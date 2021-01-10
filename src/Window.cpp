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
	unsigned Window::numberOfDisplays()
	{
#if defined(_WIN32)
		return GetSystemMetrics(SM_CMONITORS);
#elif defined(__linux__)
#error Linux implementation not implemented!
#else
#error This platform isnt supported (yet)
#endif
	}

	void Window::populateResolutions(const unsigned int& monitor)
	{
		_resolutions.clear();
		
#if defined(_WIN32)
		DEVMODE dm{ 0 };
		int i = 0;
		dm.dmSize = sizeof(dm);

		//Get display device name using the given monitor index
		DISPLAY_DEVICE dd;
		dd.cb = sizeof(DISPLAY_DEVICE);
		EnumDisplayDevices(NULL, monitor, &dd, EDD_GET_DEVICE_INTERFACE_NAME);
		while (EnumDisplaySettings(dd.DeviceName, i, &dm))
		{
			i += 1;
			_resolutions.insert(VectorU{ (unsigned int)dm.dmPelsWidth, (unsigned int)dm.dmPelsHeight });
		}
		Console::write("Populated resolutions for monitor " + std::to_string(monitor) + ": ");
#elif defined(__linux__)
        Display *dpy = XOpenDisplay(std::string(":" + std::to_string(monitor)).c_str());
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
		for (auto resolution : _resolutions)
			Console::write("\t" + std::to_string(resolution.x) + "x" + std::to_string(resolution.y));
	}
}