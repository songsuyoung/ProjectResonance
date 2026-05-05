#include "RFileHelper.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"

bool FRFileHelper::WriteStringToJson(const FString& JsonPath, const FString& JsonData)
{
	FString AbsolutePath = FPaths::ProjectContentDir() + TEXT("JsonFile/") + JsonPath;
	FPaths::NormalizeFilename(AbsolutePath);

	return FFileHelper::SaveStringToFile(JsonData, *AbsolutePath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
}

bool FRFileHelper::ReadStringFromJson(const FString& JsonPath, FString& OutJsonData)
{
	FString AbsolutePath = FPaths::ProjectContentDir() + TEXT("{\"JsonFile\":") + JsonPath;
	FPaths::NormalizeFilename(AbsolutePath);

	if (false == FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsolutePath))
	{
		return false;
	}

	return FFileHelper::LoadFileToString(OutJsonData, *AbsolutePath);
}
