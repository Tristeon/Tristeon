#include "AnimationClip.h"

namespace Tristeon
{
	REGISTER_TYPE_CPP(AnimationClip)
	
	AnimationClip::AnimationClip(std::string const& texturePath, int const& cols, int const& rows, bool const& loops)
		: AnimationClip(texturePath, cols, rows, 0, cols* rows, loops)
	{
		//Empty
	}

	AnimationClip::AnimationClip(std::string const& texturePath, int const& cols, int const& rows,
		int const& startIndex, int const& endIndex, bool const& loops) : texturePath(texturePath), cols(cols), rows(rows), startIndex(startIndex), endIndex(endIndex), loops(loops)
	{
		//Empty
	}

	json AnimationClip::serialize()
	{
		json j;
		j["typeID"] = TRISTEON_TYPENAME(AnimationClip);
		j["rows"] = rows;
		j["cols"] = cols;
		j["startIndex"] = startIndex;
		j["endIndex"] = endIndex;
		j["loops"] = loops;
		j["texturePath"] = texturePath;
		return j;
	}

	void AnimationClip::deserialize(json j)
	{
		rows = j["rows"];
		cols = j["cols"];
		startIndex = j["startIndex"];
		endIndex = j["endIndex"];
		loops = j["loops"];
		texturePath = j["texturePath"].get<std::string>();
	}
}
