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
		 * Logs the given data to the console
		 */
		static void log(const String& data);

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

#define TRISTEON_LOG(message) Tristeon::Console::log(Tristeon::String("[") + __FILE__ + "] [" + __func__ + "] [" + std::to_string(__LINE__) + "] " + Tristeon::String(message))
#define TRISTEON_WARNING(message) Tristeon::Console::warning(Tristeon::String("[") + __FILE__ + "] [" + __func__ + "] [" + std::to_string(__LINE__) + "] " + Tristeon::String(message))
#define TRISTEON_ERROR(message) Tristeon::Console::error(Tristeon::String("[") + __FILE__ + "] [" + __func__ + "] [" + std::to_string(__LINE__) + "] " + Tristeon::String(message))
#define TRISTEON_ASSERT(condition, message, severity) Tristeon::Console::assertLog(condition, String("[") + __FILE__ + "] [" + __func__ + "] [" + std::to_string(__LINE__) + "] " + Tristeon::String(message), severity)
}