#include "System/RNPCSpawnManager.h"

// 
#include "RDataManager.h"
#include "REventManager.h"
#include "RGameInstance.h"
#include "RRegionManager.h"
#include "Character/RNPCCharacter.h"
#include "Data/ResonanceEnums.h"
#include "Data/ResonanceMacro.h"
#include "Data/RNPCDataTable.h"

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
	REVENT_MESSAGE_ADD(this, this);
}

void URNPCSpawnManager::DeInitialize()
{
	REVENT_MESSAGE_Remove(this, this);
}

void URNPCSpawnManager::SpawnNPC(const FName& RegionID)
{
	URRegionManager* RegionManager = URRegionManager::Get(this);
	check(RegionManager);
	
	//RegionDataTable 에 NPC ID들이 저장, NPC를 스폰하는데 사용하도록 함.
	FVector Location;
	bool bFind = RegionManager->FindRegionLocation(RegionID, Location);
	
	if (false == bFind)
	{
		return;
	}
	
	UWorld* World = GetWorld();
	check(World);
	
	FActorSpawnParameters SpawnParams;
	ActiveNPC.Add(World->SpawnActor<ARNPCCharacter>(NPCCharacterClass, Location, FRotator::ZeroRotator, SpawnParams));
}

void URNPCSpawnManager::OnMessage(ERMessageType Type, FRMessage* Message)
{
	if (Type != ERMessageType::StartGame)
	{
		return;
	}

	InitSpawnNPC();
}

void URNPCSpawnManager::InitSpawnNPC()
{
	URDataManager* DataManager = URDataManager::Get(this);
	
	check(DataManager);
	
	UDataTable* DataTable = DataManager->GetDataTable(ERDataTableType::NPCDataTable);
	
	if (IsValid(DataTable))
	{
		UWorld* World = GetWorld();
		check(World);
		
		TArray<FRNPCDataTable*> NPCDataTable;
		DataTable->GetAllRows<FRNPCDataTable>(TEXT(""), NPCDataTable);
		
		TWeakObjectPtr<URNPCSpawnManager> ThisWeakPtr = this;
		
		for (const FRNPCDataTable* NPCData : NPCDataTable)
		{
			const FName RegionID = NPCData->RegionID;
			const float SpawnTime = NPCData->SpawnTime;
			FTimerHandle TimerHandle;
			World->GetTimerManager().SetTimer(TimerHandle, [ThisWeakPtr, RegionID]()
			{
				if (false == ThisWeakPtr.IsValid())
				{
					return;
				}
				ThisWeakPtr->SpawnNPC(RegionID);
			}
			,SpawnTime
			,false
			);
		}
	}
}
