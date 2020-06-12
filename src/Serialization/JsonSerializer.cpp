#include "JsonSerializer.h"
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#define SYSERROR()  GetLastError()
#else
#include <errno.h>
#define SYSERROR()  errno
#endif

namespace Tristeon
{
	json JsonSerializer::load(const std::string& path)
	{
		//Read file and check if it read it successfully
		std::ifstream stream(path);
		if (!stream.good()) {
			std::cout << "JsonSerializer can't read file: " << path << "\n";
			return nullptr;
		}

		//Check if any json data was found in the file, perhaps the file is not a json file or it is an empty file
		json input;
		stream >> input;
		if (input.is_null())
		{
			std::cout << "file is either a non-json file or corrupted" << std::endl;
			throw std::invalid_argument("file is either a non-json file or corrupted");
		}
		return input;
	}
	
	void JsonSerializer::save(const std::string& path, json const& obj)
	{
		//check if json isn't empty
		if (obj.is_null())
			throw std::invalid_argument("The passed object cannot be serialized");

		//Write to file
		std::ofstream stream;
		stream.open(path, std::ios::trunc);
		if (!stream.is_open())
			std::cout << "Failed to write to path " << path << ", system error code: " << SYSERROR() << std::endl;
		stream << obj.dump(4);
	}
}