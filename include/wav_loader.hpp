//Courtesy to Jelle Vrieze

#pragma once
#pragma warning(disable : 4996)
#define _CRT_SECURE_NO_WARNINGS 
#include <cstdint>
#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cassert>
#include <cmath>

typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
typedef uint8_t uint8;
typedef const char* cstring;
typedef unsigned char byte;

bool compare(uint8 arr[], cstring string, int size);
inline std::vector<uint8_t> read_file(cstring path);

/**
* Structs for storing WAV information
* Information of WAVE RIFF header format
* Information on this format can be found @ http://soundfile.sapp.org/doc/WaveFormat/
*/
struct RIFF_Header
{
	uint8 chunckId[4];
	int32 chunckSize;
	uint8 format[4];
};

struct WAVE_Format
{
	uint8 subChunckId[4];
	int32 subchunckSize;
	int16 audioFormat;
	int16 channels;
	int32 sampleRate;
	int32 byteRate;
	int16 blockAlign;
	int16 bitsPerSample;
};

struct WAVE_Data 
{
	uint8 subChunckId[4];
	int32 subChunck2Size;
};

enum class WAVE_Error
{
	NO_ERROR,
	INVALID_FILE,
	INVALID_RIFF,
	INVALID_WAVE,
	INVALID_FMT,
	INVALID_DATA
};

WAVE_Error load_wav(cstring filePath, unsigned char** data, WAVE_Format& wave_format, int* size, int* freq, bool resample = false, bool toInt16 = false)
{
	RIFF_Header riff_header;
	WAVE_Data wave_data;
	std::vector<uint8> fileData;

	fileData = read_file(filePath);
	if (fileData.empty())
		return WAVE_Error::INVALID_FILE;
	
	riff_header.chunckId[0] = fileData[0];
	riff_header.chunckId[1] = fileData[1];
	riff_header.chunckId[2] = fileData[2];
	riff_header.chunckId[3] = fileData[3];
	if (!compare(riff_header.chunckId, "RIFF", 4))
		return WAVE_Error::INVALID_RIFF;

	riff_header.format[0] = fileData[8];
	riff_header.format[1] = fileData[9];
	riff_header.format[2] = fileData[10];
	riff_header.format[3] = fileData[11];
	if (!compare(riff_header.format, "WAVE", 4))
		return WAVE_Error::INVALID_WAVE;

	wave_format.subChunckId[0] = fileData[12];
	wave_format.subChunckId[1] = fileData[13];
	wave_format.subChunckId[2] = fileData[14];
	wave_format.subChunckId[3] = fileData[15];
	if (!compare(wave_format.subChunckId, "fmt ", 4))
		return WAVE_Error::INVALID_FMT;

	memcpy(&wave_format.subchunckSize, (void*)&fileData[16], 4);
	memcpy(&wave_format.audioFormat, (void*)&fileData[20], 2);
	memcpy(&wave_format.channels, (void*)&fileData[22], 2);
	memcpy(&wave_format.sampleRate, (void*)&fileData[24], 4);
	memcpy(&wave_format.byteRate, (void*)&fileData[28], 4);
	memcpy(&wave_format.blockAlign, (void*)&fileData[32], 2);
	memcpy(&wave_format.bitsPerSample, (void*)&fileData[34], 2);

	wave_data.subChunckId[0] = fileData[36];
	wave_data.subChunckId[1] = fileData[37];
	wave_data.subChunckId[2] = fileData[38];
	wave_data.subChunckId[3] = fileData[39];
	if (!compare(wave_data.subChunckId, "data", 4))
		return WAVE_Error::INVALID_DATA;

	memcpy(&wave_data.subChunck2Size, (void*)&fileData[40], 4);

	int dataSize = (fileData.size() - 44);

	byte* audioData;
	if (toInt16 && wave_format.bitsPerSample == 32)
	{
		//audiodata
	}
	else
	{
		audioData = new byte[dataSize];
		memcpy(audioData, (void*)&fileData[44], dataSize);
		delete[] audioData;
	}

	if (resample)
	{
		int upSampledRate = 4;
		int upsampledDataSize = dataSize * upSampledRate;

		unsigned char* upSampledData = new unsigned char[upsampledDataSize];
		int j = 0;
		for (size_t i = 0; i < dataSize; ++i)
		{
			if (i == (dataSize - 1))
			{
				// end of the loop
				upSampledData[i] = fileData[44 + i];
				upSampledData[i + 1] = fileData[44 + i];
				break;
			}
			upSampledData[j] = fileData[44 + i];
			upSampledData[j + 2] = static_cast<unsigned char>(static_cast<int16>(fileData[44 + i] + fileData[44 + i + 1]) / 2);
			upSampledData[j + 1] = static_cast<unsigned char>(static_cast<int16>(fileData[44 + i] + upSampledData[j + 2]) / 2);
			upSampledData[j + 3] = static_cast<unsigned char>(static_cast<int16>(upSampledData[j + 2] + fileData[44 + i + 1]) / 2);
			j += upSampledRate;
		}

		*size = upsampledDataSize;
		*data = new unsigned char[upsampledDataSize];
		memcpy(*data, upSampledData, upsampledDataSize);

		*freq = wave_format.sampleRate * upSampledRate;

		delete[] upSampledData;
	}
	else
	{
		*size = dataSize;
		*data = new unsigned char[dataSize];
		memcpy(*data, (void*)&fileData[44], dataSize);

		*freq = wave_format.sampleRate;
	}

	return WAVE_Error::NO_ERROR;
}

bool compare(uint8 arr[], cstring string, int size)
{
	for (int i = 0; i < size; ++i)
	{
		if (arr[i] != string[i]) return false;
	}
	return true;
}

inline std::vector<uint8_t> read_file(cstring path)
{
	//create managed FILE ptr
	const std::unique_ptr<FILE, decltype(&fclose)> file(
		fopen(std::string(path).c_str(), "r+b"),
		fclose // deleter is fclose
	);

	if (!file)
		return {};

	//get size and create container of that size
	fseek(file.get(), 0L, SEEK_END);
	std::vector<uint8_t> container(ftell(file.get()));
	fseek(file.get(), 0L, SEEK_SET);

	//read file 
	fread(container.data(), sizeof(unsigned char), container.size(), file.get());

	return container;
}