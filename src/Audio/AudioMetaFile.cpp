#include "AudioMetaFile.h"
#include "AudioClip.h"

namespace Tristeon
{
	json AudioMetaFile::serialize()
	{
		json j = MetaFile::serialize();
		j["typeID"] = Type<AudioMetaFile>::fullName();
		return j;
	}

	void AudioMetaFile::deserialize(json j)
	{
		MetaFile::deserialize(j);
	}

	Unique<TObject> AudioMetaFile::load()
	{
		return std::make_unique<AudioClip>();
	}
}