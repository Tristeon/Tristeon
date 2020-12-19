#include "JsonSerializer.h"
#include <fstream>

//Attempt to get file read errors
#ifdef _WIN32
#undef WIN32_LEAN_AND_MEAN
#include <Windows.h>
#define SYSERROR()  GetLastError()
#else
#include <errno.h>
#define SYSERROR()  errno
#endif

namespace Tristeon
{
	json JsonSerializer::load(const String& path)
	{
		//Read file and check if it read it successfully
		std::ifstream stream(path);
		if (!stream.good()) {
			Console::warning("JsonSerializer failed to read file: " + path);
			return nullptr;
		}

		//Check if any json data was found in the file, perhaps the file is not a json file or it is an empty file
		json input;
		stream >> input;
		if (input.is_null())
		{
			Console::warning("The given file is either a non-json file or the file is corrupted");
			throw std::invalid_argument("file is either a non-json file or corrupted");
		}
		return input;
	}
	
	void JsonSerializer::save(const String& path, const json& obj)
	{
		//check if json isn't empty
		if (obj.is_null())
			throw std::invalid_argument("The passed object cannot be serialized");

		//Write to file
		std::ofstream stream;
		stream.open(path, std::ios::trunc);
		if (!stream.is_open())
		{
			Console::warning("Failed to write to path " + path + ", system error code: " + std::to_string(SYSERROR()));
			return;
		}
		stream << obj.dump(4);
	}

	void JsonSerializer::serialize(const String& path, Serializable* obj)
	{
		if (obj == nullptr)
			throw std::invalid_argument("The passed object can't be nullptr!");

		//Convert the object instance to json data
		const json j = obj->serialize();
		save(path, j);
	}
}