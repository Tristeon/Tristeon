#pragma once
#include <Standard/String.h>

namespace Tristeon
{
	enum class AssertSeverity
	{
		/**
		 * Print a log message
		 */
		Log,
		/**
		 * Display a distinguished yellow log message
		 */
		Warning,
		/**
		 * Throw a std::runtime_error
		 */
		Fatal
	};

	/**
	 * Console defines basic printing/asserting behavior that will be logged to the editor or to a console.
	 */
	class Console final
	{
	public:
		/**
		 * Sets the standard console color.
		 */
		static void init();
		
		/**
		 * Performs an action if the given condition is not true, the action depends on the assert severity (default: Fatal).
		 * \exception runtime_error If condition is false and the severity is Fatal
		 */
		static void assertLog(const bool& condition, const String& message, const AssertSeverity& severity = AssertSeverity::Fatal);
		
		/**
		 * Clears the console.
		 */
		static void clear();

		/**
		 * Writes the given data to the console
		 */
		static void write(const String& data);

		/**
		 * Writes a warning with the given data to the editor/console
		 */
		static void warning(const String& data);

		/**
		 * Opens an error pop-up window and stops the program.
		 *
		 * \exception runtime_error (data)
		 */
		static void error(const String& data);
	};
}