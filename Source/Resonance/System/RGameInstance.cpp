#include "System/RGameInstance.h"

// Newly Created File Names ..
#include "System/RDataManager.h"

void URGameInstance::Init()
{
	Super::Init();

	check(DataManagerClass);

	DataManager = NewObject<URDataManager>(this, DataManagerClass);
}
