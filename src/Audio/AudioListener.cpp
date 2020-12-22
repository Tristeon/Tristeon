#include "AudioListener.h"
#include "Audio.h"
#include "AL/al.h"

namespace Tristeon
{
	json AudioListener::serialize()
	{
		json j = Behaviour::serialize();
		j["typeID"] = Type<AudioListener>::fullName();
		return j;
	}

	void AudioListener::deserialize(json j)
	{
		Behaviour::deserialize(j);
	}

	void AudioListener::lateUpdate()
	{
		//Update the listener's position
		AUDIO_ASSERT(alListener3f(AL_POSITION, actor()->position.x, actor()->position.y, 0));
	}
}