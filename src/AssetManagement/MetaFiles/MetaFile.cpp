#include "MetaFile.h"
#include <Serialization/Type.h>

namespace Tristeon
{
	json MetaFile::serialize()
	{
		json j = Serializable::serialize();
		j["typeID"] = Type<MetaFile>::fullName();
		j["filepath"] = filepath;
		j["GUID"] = GUID;
		return j;
	}

	void MetaFile::deserialize(json j)
	{
		Serializable::deserialize(j);

		filepath = j.value("filepath", "");
		GUID = j["GUID"];
	}
}
