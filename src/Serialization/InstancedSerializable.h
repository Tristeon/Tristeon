#pragma once
#include "Serializable.h"

namespace Tristeon
{
	/**
	 * InstancedSerializable is a specification of Serializable that contains an InstanceID.
	 * InstanceIDs are used to lookup references of instances throughout the project.
	 *
	 * Common use cases for this are Actors and Behaviours, but it may also apply to other objects.
	 */
	class InstancedSerializable : public Serializable
	{
	public:
		InstancedSerializable();
		virtual ~InstancedSerializable() noexcept;

		DELETE_COPY(InstancedSerializable);
		DEFAULT_MOVE(InstancedSerializable);
		
		/**
		 * Serialize interface that is called on derived instance of serializable to obtain json data of objects
		 * Each class that implements this function must set json["typeID"] to TRISTEON_TYPENAME(ClassType).
		 * Any other variables are optional but required for serialization purposes.
		 */
		json serialize() override;

		/**
		 * Deserialize interface for classes to decide how to use json data to load in data into their class
		 */
		void deserialize(json j) override;

		/**
		 * A unique identifier to allow for reliable lookups.
		 * The instanceID is generated upon initial creation and then stored.
		 * It can be used to serialize references to objects, which can then be looked up through the Scene interface.
		 */
		[[nodiscard]] unsigned int instanceID() const;
	private:
		unsigned int _instanceID = 0;
	};
}