#include "String.h"
#include <algorithm>
#include <iterator>

namespace Tristeon
{
	String::String(std::string s) : internalString(s)
	{
		//Empty
	}

	String::operator std::string()
	{
		return internalString;
	}

	String& String::operator+(std::string const& other) const
	{
		return String(internalString + other);
	}

	String& String::operator+(char* const& other) const
	{
		return String(internalString + std::string(other));
	}

	void String::operator+=(std::string const& other)
	{
		internalString += other;
	}

	void String::operator+=(char* const& other)
	{
		internalString += std::string(other);
	}

	String& String::toLower()
	{
		std::transform(internalString.begin(), internalString.end(), internalString.begin(), ::tolower);
		return *this;
	}

	String& String::toLower(String& string)
	{
		return string.toLower();
	}

	std::vector<String> String::split(String const& string, char const& delim)
	{
		return string.split(delim);
	}

	std::vector<String> String::split(char const& delim) const
	{
		std::vector<String> elems;
		internalSplit(internalString, delim, elems);
		return elems;
	}

	String String::generateRandom(int const& length)
	{
		if (length <= 0)
			throw std::invalid_argument("The length parameter of StringUtils::generateRandom must be > 0!");

		static const char alphanum[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		static const int size = sizeof(alphanum);

		std::string output;

		for (int i = 0; i < length; ++i) {
			output.push_back(alphanum[rand() % (size - 1)]);
		}

		return output;
	}

	void String::internalSplit(const std::string& s, char delim, std::vector<String>& result) const
	{
		std::stringstream ss;
		ss.str(s);
		std::string item;
		while (getline(ss, item, delim)) {
			result.push_back(item);
		}
	}
}
