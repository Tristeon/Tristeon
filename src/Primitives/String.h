#pragma once
#include <sstream>
#include <vector>

namespace Tristeon
{
	class String final
	{
	public:
		String(std::string s);
		operator std::string();
		std::string getStdString() const { return internalString; }
		String& operator+(std::string const& other) const;
		String& operator+(char* const& other) const;
		void operator+=(std::string const& other);
		void operator+=(char* const& other);
		
		String& toLower();
		static String& toLower(String& string);

		/**
		 * Splits a string into separate strings based on the given delimiter.
		 */
		static std::vector<String> split(String const& string, char const& delim);
		/**
		 * Splits a string into separate strings based on the given delimiter.
		 */
		std::vector<String> split(char const& delim) const;

		/**
		 * Generates a string with random characters with a given length.
		 *
		 * \exception invalid_argument if length <= 0
		 */
		static String generateRandom(int const& length);
	private:
		void internalSplit(const std::string& s, char delim, std::vector<String>& result) const;
		std::string internalString;
	};
}