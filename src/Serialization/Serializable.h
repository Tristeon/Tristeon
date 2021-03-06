#pragma once
#include <json.h>
#include <TObject.h>
#include <Utils/ClassDefaults.h>

using namespace nlohmann;

namespace Tristeon
{
	/**
	 * Serializable is the base class for all Serializable objects in Tristeon.
	 * It implements a serialize() and deserialize() function which need to be overridden to serialize variables,
	 * and in order for type introspection to work properly.
	 */
	class Serializable : public TObject
	{
	public:
		Serializable() = default;
		virtual ~Serializable() = default;

		DELETE_COPY(Serializable);
		DEFAULT_MOVE(Serializable);
		
		/**
		 * Serialize interface that is called on derived instance of serializable to obtain json data of objects
		 * Each class that implements this function must set json["typeID"] to TRISTEON_TYPENAME(ClassType).
		 * Any other variables are optional but required for serialization purposes.
		 */
		[[nodiscard]] virtual json serialize() { return json{}; }
		
		/**
		 * Deserialize interface for classes to decide how to use json data to load in data into their class
		 */
		virtual void deserialize(json j) { }
	};
}