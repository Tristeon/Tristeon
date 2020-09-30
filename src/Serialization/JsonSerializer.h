#pragma once
#include <json.h>

#include <Serialization/TypeRegister.h>
#include <Serialization/Serializable.h>

using namespace nlohmann;

namespace Tristeon
{
	/**
	 * The JsonSerializer class is a static class used to load & save json objects from a file.
	 */
	class JsonSerializer
	{
	public:
		/**
		 * Serializes the object into json and creates a file using the given path.
		 * \param path the filepath at which to create and save the json object.
		 * \param obj the object that is to be turned into json.
		 * 
		 * \exception invalid_argument Throws if the json data is empty.
		 */
		static void serialize(const String& path, Serializable* obj);

		/**
		 * Saves the json data into a file with the given filepath.
		 *
		 * \exception invalid_argument Throws if the json data is empty.
		 */
		static void save(const String& path, const json& obj);

		/**
		 * Create an instance with type T from a json file at the given filepath.
		 *
		 * \exception invalid_argument If the object didn't serialize a typeID.
		 * \exception invalid_argument If the json file is invalid or empty.
		 */
		template <typename T>
		[[nodiscard]] static Unique<T> deserialize(const String& path);

		/**
		 * Loads json data from a file using the given filepath.
		 *
		 * \exception invalid_argument Throws if the file has no json data.
		 */
		[[nodiscard]] static json load(const String& path);
	};

	template <typename T>
	Unique<T> JsonSerializer::deserialize(const String& path)
	{
		static_assert(std::is_base_of<Serializable, T>::value, "Can't deserialize an object if it isn't of type Serializable");
		static_assert(!std::is_abstract<T>::value, "Can't deserialize an abstract type!");
		
		json input = load(path);

		//Check if the object is serializing its typeID
		const auto iterator = input.find("typeID");
		if (iterator == input.end())
			throw std::invalid_argument("The object you are serializing is not serializing its typeID!");

		if (input.is_null())
			throw std::invalid_argument("File is either a non-json file or corrupted");

		//Create instance of the type that is specified in the json file under the "typeID" member
		auto instance = TypeRegister::createInstance(input.value("typeID", ""));
		if (instance == nullptr)
			return nullptr;
		instance->deserialize(input);

		//Cast into the desired type
		if (dynamic_cast<T*>(instance.get()) != nullptr)
			return Unique<T>(dynamic_cast<T*>(instance.release())); //Move and cast value
		return nullptr; //If cast fails, return nullptr, instance gets cleaned automatically.
	}
}