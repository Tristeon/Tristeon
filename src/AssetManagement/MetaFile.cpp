#include "MetaFile.h"

#include "Serialization/Typename.h"

json Tristeon::MetaFile::serialize()
{
	json j;
	j["typeID"] = TRISTEON_TYPENAME(MetaFile);
	j["filepath"] = filepath;
	j["GUID"] = GUID;
	return j;
}

void Tristeon::MetaFile::deserialize(json j)
{
	filepath = j.value("filepath","");
	GUID = j["GUID"];
}