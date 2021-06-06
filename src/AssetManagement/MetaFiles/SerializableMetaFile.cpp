#include "SerializableMetaFile.h"
#include <Serialization/JsonSerializer.h>

namespace Tristeon
{
	Unique<TObject> SerializableMetaFile::load(const String& path)
	{
		auto j = JsonSerializer::load(path);
		if (j.contains("typeID") == false)
		{
			TRISTEON_WARNING("Serializable (from file) requested doesn't serialize its type ID! File path: " + path);
			return nullptr;
		}

		auto obj = TypeRegister::createInstance(j["typeID"]);
		if (obj)
			obj->deserialize(j);
		return std::move(obj);
	}

	json SerializableMetaFile::serialize()
	{
		json j;
		j["typeID"] = Type<SerializableMetaFile>::fullName();
		return j;
	}
}
