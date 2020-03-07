#include "HUD.h"

namespace Tristeon
{
	REGISTER_TYPE_CPP(HUD)
	
	json HUD::serialize()
	{
		json j;
		j["typeID"] = TRISTEON_TYPENAME(HUD);
		//TODO: HUD Serialization
		return j;
	}

	void HUD::deserialize(json j)
	{
		//TODO: HUD Deserialization
	}
}
