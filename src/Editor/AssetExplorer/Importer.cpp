#include "Importer.h"

#include "Project.h"
#include "AssetManagement/MetaFiles/MetaFile.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/Register.h"
#include "Utils/Random.h"

using namespace TristeonEditor;
using namespace Tristeon;

namespace fs = std::filesystem;

void Importer::import(std::filesystem::path assetPath)
{
	if (!exists(assetPath)) throw std::exception("Invalid assetPath has been passed. File does not exist");
	
	if (fs::exists(assetPath.string() + ".meta")) return;
	
	Unique<MetaFile> metaFile = Register<MetaFile>::createInstance(assetPath.extension().string());

	if (metaFile == nullptr) metaFile = std::make_unique<MetaFile>();

	metaFile->GUID = Random::generateUInt();
	metaFile->filepath = assetPath.string();
	
	JsonSerializer::serialize(assetPath.string() + ".meta", metaFile.get());
}

void Importer::scanProject()
{
	Vector<fs::path> metaFilesToRemove;
	
	for (auto& directoryEntry : fs::recursive_directory_iterator(Project::assetPath()))
	{
		auto path = directoryEntry.path();
		if (path.extension().string() == ".meta")
		{
			String assetPath = path.parent_path().string() + path.stem().string();
			if (!fs::exists(assetPath)) metaFilesToRemove.add(path);
			continue;
		}
		
		if (fs::exists(path.string() + ".meta")) continue;

		import(path);
	}

	for (auto& metaFile : metaFilesToRemove)
	{
		fs::remove(metaFile);
	}
}
