#include "MetaFile.h"

#include "Serialization/Type.h"

json Tristeon::MetaFile::serialize()
{
	json j = Serializable::serialize();
	j["typeID"] = Type<MetaFile>::fullName();
	j["filepath"] = filepath;
	j["GUID"] = GUID;
	return j;
}

void Tristeon::MetaFile::deserialize(json j)
{
	Serializable::deserialize(j);
	
	filepath = j.value("filepath","");
	GUID = j["GUID"];
}