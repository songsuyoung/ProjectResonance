#include "System/RNPCSpawnManager.h"

// 
#include "RDataManager.h"
#include "REventManager.h"
#include "RGameInstance.h"
#include "RMessage.h"
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
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(Location);
	SpawnTransform.SetRotation(FRotator::ZeroRotator.Quaternion());
	
	// TODO: NPCID를 지정해야함. 현재 RegionID를 사용하고 있음.
	// 아직 NPCID가 없어서 임의의 랜덤 값 지정.
	ARNPCCharacter* SpawnedNPC = World->SpawnActorDeferred<ARNPCCharacter>(NPCCharacterClass, SpawnTransform);
	SpawnedNPC->SetID(RegionID); //일단 지역 ID로 저장
	SpawnedNPC->FinishSpawning(SpawnTransform);
	
	ActiveNPC.Add({RegionID, SpawnedNPC});
}

void URNPCSpawnManager::AcquireNPC(FName NPCID)
{
	// NPC를 찾는다.
	FRNPCPooling* Slot = NPCPooling.Find(NPCID);
	
	if (nullptr == Slot)
	{
		return;
	}
	NPCPooling.Remove(NPCID);
	ARNPCCharacter* NPCCharacter = Slot->NPC;
	ActiveNPC.Add({NPCID, NPCCharacter});
	
	Slot->SpawnTimerHandle.Invalidate(); //무효화
	
	if (false == IsValid(NPCCharacter))
	{
		return;	
	}
	
	// NPC->Stat 정리
	
	// NPC 활성화
	NPCCharacter->SetActorHiddenInGame(false);
}

void URNPCSpawnManager::ReleaseNPC(const FRMessage* Msg)
{
	const FRNPCReleaseMessage* ReleaseMessage = static_cast<const FRNPCReleaseMessage*>(Msg);
	
	if (nullptr == ReleaseMessage)
	{
		return;
	}
	TObjectPtr<ARNPCCharacter>* NPC = ActiveNPC.Find(ReleaseMessage->NPCID);
	
	if (nullptr == NPC || false == IsValid(*NPC))
	{
		return;
	}
	
	(*NPC)->SetActorHiddenInGame(true);
	
	FName NPCID = ReleaseMessage->NPCID;
	
	FRNPCPooling PoolingSlot;
	PoolingSlot.NPC = *NPC; 
	
	UWorld* World = GetWorld();
	check(World);
	PoolingSlot.PooledTime = World->GetTimeSeconds();
	
	NPCPooling.Add({NPCID, PoolingSlot});
	ActiveNPC.Remove(NPCID);
	
	TWeakObjectPtr<URNPCSpawnManager> ThisWeakPtr = this;
	
	World->GetTimerManager().SetTimer(
		PoolingSlot.SpawnTimerHandle,
		FTimerDelegate::CreateLambda([ThisWeakPtr, NPCID]()
		{
			if (false == ThisWeakPtr.IsValid())
			{
				return;
			}
			ThisWeakPtr->AcquireNPC(NPCID);
		}),
		5.f,
		false
	);
}

void URNPCSpawnManager::OnMessage(ERMessageType Type, FRMessage* Message)
{
	switch (Type)
	{
		case ERMessageType::StartGame:
			InitSpawnNPC();
		break;
		case ERMessageType::EnterRegion:
			ReleaseNPC(Message);
		break;
	}
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
			// TODO: C++ ID 로딩할 수 있도록 수정.
			// const FName NPCID = NPCData->GetID();
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
