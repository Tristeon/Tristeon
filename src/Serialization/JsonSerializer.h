#pragma once
#include <json.h>

#include <Serialization/TypeRegister.h>
#include <Serialization/Serializable.h>

using namespace nlohmann;

namespace Tristeon
{
	template<typename T>
	using IsSerializable = std::enable_if_t<std::is_base_of<Serializable, T>::value, T>;
	
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
		template <typename T>
		static void serialize(const std::string& path, IsSerializable<T>& obj);

		/**
		 * Saves the json data into a file with the given filepath.
		 *
		 * \exception invalid_argument Throws if the json data is empty.
		 */
		static void save(const std::string& path, const json& obj);

		/**
		 * Create an instance with type T from a json file at the given filepath.
		 *
		 * \exception invalid_argument If the object didn't serialize a typeID.
		 * \exception invalid_argument If the json file is invalid or empty.
		 */
		template <typename T>
		static IsSerializable<T>* deserialize(const std::string& path);

		/**
		 * Loads json data from a file using the given filepath.
		 *
		 * \exception invalid_argument Throws if the file has no json data.
		 */
		static json load(const std::string& path);
	};

	template <typename T>
	void JsonSerializer::serialize(const std::string& path, IsSerializable<T>& obj)
	{
		//Convert the object instance to json data
		const json j = obj.serialize();
		save(path, j);
	}

	template <typename T>
	IsSerializable<T>* JsonSerializer::deserialize(const std::string& path)
	{
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
		
		Serializable * deserializedObject = static_cast<Serializable*>(instance.get());
		instance.release();
		//Load json data into the instance
		deserializedObject->deserialize(input);
		//Cast into the desired type
		T * obj = static_cast<T*>(deserializedObject);
		return obj;
	}
}