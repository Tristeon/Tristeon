#pragma once
#include <fstream>
#include <iostream>
#include <json.h>

using namespace nlohmann;

namespace Tristeon
{
	class JsonSerializer
	{
	public:
		/**
		 * \brief Serializes the obj into json and creates a file using the given path
		 * \param path the filepath that is used to create the file
		 * \param obj the object that is turned into json
		 */
		template <typename T> static void serialize(const std::string& path, T& obj);
		static void serialize(const std::string& path, json& obj);

		/**
		 * \brief Create an instance with type T using the given filepath
		 */
		template <typename T> static T* deserialize(const std::string& path);

		static json load(const std::string& path);
	};

	inline void JsonSerializer::serialize(const std::string& path, json& obj)
	{
		//check if there is anything serialized
		if (obj.is_null()) {
			throw std::invalid_argument("The passed object cannot be serialized");
		}

		//Write to file
		std::ofstream stream;
		stream.open(path, std::fstream::out);
		stream << obj;
	}

	template <typename T>
	void JsonSerializer::serialize(const std::string& path, T& obj)
	{
		//Convert the object instance to json data
		json j = obj.serialize();
		serialize(path, j);
	}

	inline json JsonSerializer::load(const std::string& path)
	{
		//Read file and check if it read it successfully
		std::ifstream stream(path);
		if (!stream.good()) {
			std::cout << "Serializer can't serialize cus can't read file: " << path << "\n";
			return nullptr;// throw std::invalid_argument("Serializer can't serialize cus can't read file :c");
		}

		//Check if any json data was found in the file, perhaps the file is not a json file or it is an empty file
		json input;
		stream >> input;
		if (input.is_null())
		{
			std::cout << "file is either a non-json file or corrupted" << std::endl;
			throw std::invalid_argument("file is either a non-json file or corrupted");
		}

		return input;
	}


	template <typename T>
	T* JsonSerializer::deserialize(const std::string& path)
	{
		json input = load(path);

		//Check if the object is serializing its typeID
		const auto iterator = input.find("typeID");
		if (iterator == input.end())
		{
			throw std::invalid_argument("The object you are serializing is not serializing its typeID!");
		}
		else if (input.is_null())
		{
			std::cout << "file is either a non-json file or corrupted" << std::endl;
			throw std::invalid_argument("file is either a non-json file or corrupted");
		}

		//Create instance of the type that is specified in the json file under the "typeID" member
		auto instance = TypeRegister::createInstance(input["typeID"]);
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