#include "Console.h"
#include <iostream>
#include "rlutil.h"

namespace Tristeon
{
	void Console::init()
	{
#ifdef TRISTEON_LOGENABLED
		//Clear and set the color to white
		clear();
		setColor(rlutil::WHITE);
#endif
	}

	void Console::assertLog(const bool& condition, const String& message, const AssertSeverity& severity)
	{
#ifdef TRISTEON_LOGENABLED
		if (!condition)
		{
			switch(severity)
			{
				case AssertSeverity::Log: 
				{
					write(message);
					break;
				}
				case AssertSeverity::Warning:
				{
					warning(message);
					break;
				}
				case AssertSeverity::Fatal:
				{
					error(message);
				}
				break;
			}
		}
#endif
	}

	void Console::clear()
	{
#ifdef TRISTEON_LOGENABLED
		rlutil::cls();
#endif
	}

	void Console::write(const String& data)
	{
#ifdef TRISTEON_LOGENABLED
		std::cout << data << std::endl;
#endif
	}

	void Console::warning(const String& data)
	{
#ifdef TRISTEON_LOGENABLED
		setColor(rlutil::YELLOW);
		std::cout << "[WARNING]\t" << data << std::endl;
		setColor(rlutil::WHITE);
#endif
	}

	void Console::error(const String& data)
	{
#ifdef TRISTEON_LOGENABLED
		setColor(rlutil::LIGHTRED);
		std::cout << "[ERROR]\t" << data << std::endl;
		setColor(rlutil::WHITE);
#endif

#if (defined(_WIN32) | defined(_WIN64)) && TRISTEON_LOGENABLED

		MessageBox(nullptr, data.c_str(), "Oops! Something went wrong...", MB_OK);
#endif
		throw std::runtime_error(data.c_str());
	}
}