#include "StringHelper.h"

#include <Utils/TypeDefinitions.h>

#include <algorithm>
#include <sstream>

#include "Random.h"

namespace Tristeon
{
	String StringHelper::toLower(String string)
	{
		std::transform(string.begin(), string.end(), string.begin(), std::tolower);
		return string;
	}

	Vector<String> StringHelper::split(const String& string, const char& delim)
	{
		Vector<String> elems;
		internalSplit(string, delim, elems);
		return elems;
	}

	void StringHelper::internalSplit(const String& s, const char& delim, Vector<String>& result)
	{
		std::stringstream ss;
		ss.str(s);
		std::string item;
		while (getline(ss, item, delim)) {
			result.add(item);
		}
	}
}