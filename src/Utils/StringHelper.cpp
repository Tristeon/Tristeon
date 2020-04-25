#include "StringHelper.h"

#include <TypeDefinitions.h>

#include <algorithm>
#include <sstream>

namespace Tristeon
{
	String StringHelper::toLower(String & string)
	{
		std::transform(string.begin(), string.end(), string.begin(), ::tolower);
		return string;
	}

	Vector<String> StringHelper::split(String const& string, char const& delim)
	{
		Vector<String> elems;
		internalSplit(string, delim, elems);
		return elems;
	}

	String StringHelper::generateRandom(unsigned int const& length)
	{
		if (length == 0)
			throw std::invalid_argument("The length parameter of StringUtils::generateRandom must be > 0!");

		static const char alphanum[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		static const int size = sizeof(alphanum);

		std::string output;

		for (size_t i = 0; i < length; ++i) {
			output.push_back(alphanum[rand() % (size - 1)]);
		}

		return output;
	}

	void StringHelper::internalSplit(const std::string& s, char const& delim, Vector<String>& result)
	{
		std::stringstream ss;
		ss.str(s);
		std::string item;
		while (getline(ss, item, delim)) {
			result.push_back(item);
		}
	}
}
