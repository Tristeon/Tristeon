#pragma once
#include <Standard/String.h>

namespace Tristeon
{
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
		 * Throws an error if the given condition is not true
		 * \param condition The condition you expect to be true
		 * \param errorMessage The error message to be shown in the error pop-up
		 *
		 * \exception runtime_error If condition is false
		 */
		static void t_assert(const bool& condition, const String& errorMessage);
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
