#pragma once

#include "CoreMinimal.h"

struct RESONANCECORE_API FRFileHelper
{
	static bool WriteStringToJson(const FString& JsonPath, const FString& JsonData);
	static bool ReadStringFromJson(const FString& JsonPath, FString& OutJsonData);
};