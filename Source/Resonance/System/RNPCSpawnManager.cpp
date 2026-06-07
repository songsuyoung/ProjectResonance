#include "System/RNPCSpawnManager.h"

#include "RGameInstance.h"
#include "RPathFinder.h"
#include "Character/RNPCCharacter.h"
#include "Data/RCoreEnums.h"


URNPCSpawnManager* URNPCSpawnManager::Get(UObject* Context)
{
	UWorld* World = Context->GetWorld();

	check(World);

	URGameInstance* GameInstance = Cast<URGameInstance>(World->GetGameInstance());

	check(GameInstance);

	return GameInstance->GetNPCSpawnManager();
}

void URNPCSpawnManager::Initialize()
{
	PathFinder = URPathFinder::Get(this);
	
	check(PathFinder.Get());
	
	SpawnPointIndex = PathFinder->GetTypedPointIndex(ERPathPointType::Spawn);
}

void URNPCSpawnManager::SpawnNPC()
{
	int32 SpawnIndex = FMath::RandRange(0, SpawnPointIndex.Num() - 1);
	
	if (false == SpawnPointIndex.IsValidIndex(SpawnIndex))
	{
		return;
	}
	
	const FVector& Location = PathFinder->GetPathLocation(SpawnPointIndex[SpawnIndex]);
	
	UWorld* World = GetWorld();
	check(World);
	
	FActorSpawnParameters SpawnParams;
	ActiveNPC.Add(World->SpawnActor<ARNPCCharacter>(NPCCharacterClass, Location, FRotator::ZeroRotator, SpawnParams));
}
