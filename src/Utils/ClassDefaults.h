/**
 * Defines that help simplify default creation (or deletion) of move/copy operators.
 */
#pragma once

namespace Tristeon
{
	/**
	 * Deletes the copy constructor and operator
	 */
#define DELETE_COPY(type) type(const type& copy) = delete; type& operator=(const type& copy) = delete
	/**
	 * Creates a default copy constructor and operator
	 */
#define DEFAULT_COPY(type) type(const type& copy) = default; type& operator=(const type& copy) = default

	/**
	 * Deletes the move constructor and operator
	 */
#define DELETE_MOVE(type) type(type&& move) = delete; type& operator=(type&& move) = delete
	/**
	 * Creates a default move constructor and operator
	 */
#define DEFAULT_MOVE(type) type(type&& move) noexcept = default; type& operator=(type&& move) noexcept = default
}
