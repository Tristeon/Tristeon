#include "Window.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace Tristeon
{
	void Window::populateResolutions()
	{
#ifdef _WIN32
		DEVMODE dm{ 0 };
		int i = 0;
		dm.dmSize = sizeof(dm);

		while (EnumDisplaySettingsA(nullptr, i, &dm))
		{
			i += 1;
			_resolutions.insert(VectorU{ (unsigned int)dm.dmPelsWidth, (unsigned int)dm.dmPelsHeight });
		}
#elif defined(__linux__)
#error TODO: Linux populateResolutions()
#endif
	}
}