#pragma once
#include <json.h>
using namespace nlohmann;

namespace Tristeon
{
	class Serializable
	{
	public:
		virtual ~Serializable() = default;

		/**
		 * \brief Serialize interface that is called on derived instance of serializable to obtain json data of objects
		 */
		virtual json serialize() { return json(); }
		/**
		 * \brief Deserialize interface for classes to decide how to use json data to load in data into their class
		 */
		virtual void deserialize(json j) {}

		/**
		 * \brief Checks if the T is the exact same type as this one. (Does not work with inheritance yet)
		 */
		template <typename T> bool isType();
	};

	template<typename T>
	bool Serializable::isType()
	{
		return serialize()["typeID"] == TRISTEON_TYPENAME(T);
	}
}