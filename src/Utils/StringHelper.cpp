#include "StringHelper.h"

#include <algorithm>
#include <sstream>

#include "Random.h"
#include <Standard/List.h>
#include <Standard/String.h>

namespace Tristeon
{
	String StringHelper::toLower(String string)
	{
		std::transform(string.begin(), string.end(), string.begin(), [](unsigned char c) -> unsigned char { return std::toupper(c); });
		return string;
	}

	List<String> StringHelper::split(const String& string, const char& delim)
	{
		List<String> elems;
		internalSplit(string, delim, elems);
		return elems;
	}

	void StringHelper::internalSplit(const String& s, const char& delim, List<String>& result)
	{
		std::stringstream ss;
		ss.str(s);
		std::string item;
		while (getline(ss, item, delim)) {
			result.add(item);
		}
	}
}