#include "AudioClip.h"
#include "Utils/Console.h"
#include <wav_loader.hpp>
#include <magic_enum.hpp>

namespace Tristeon
{
	AudioClip::AudioClip()
	{
		alGenBuffers(1, &_buffer);
	}

	AudioClip::~AudioClip()
	{
		alDeleteBuffers(1, &_buffer);
	}

	AudioClip::AudioClip(const String& path)
	{
		//Create an OpenAL buffer to store our data
		alGenBuffers(1, &_buffer);
		Console::assertLog(alGetError() == AL_NO_ERROR, "Failed to generate OpenAL buffer", AssertSeverity::Warning);

		//Load wav file (no other file types supported for now)
		unsigned char* data = nullptr;
		ALsizei size;
		ALsizei freq;
		WAVE_Format format{};

		const auto err = load_wav(path.c_str(), &data, format, &size, &freq);
		if (err != WAVE_Error::NO_ERROR)
		{
			Console::warning("Failed to load: WAV File " + path + ". Error code: " + magic_enum::enum_name(err).data());
			return;
		}

		//Upload data to the OpenAL buffer
		alBufferData(_buffer, AL_FORMAT_MONO8, data, size, freq);
		const auto error = alGetError();
		Console::assertLog(error == AL_NO_ERROR, "Failed to send audio data to OpenAL buffer. Error: " + std::to_string(error), AssertSeverity::Warning);
	}
}