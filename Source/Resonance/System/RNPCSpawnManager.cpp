#include "System/RNPCSpawnManager.h"

// 
#include "RDataManager.h"
#include "System/REventManager.h"
#include "RGameInstance.h"
#include "System/RMessage.h"
#include "RRegionManager.h"
#include "Character/RNPCCharacter.h"
#include "Components/Stat/RBaseStatComponent.h"
#include "Data/ResonanceEnums.h"
#include "Data/RNPCDataTable.h"
#include "System/ResonanceMacro.h"

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
	REVENT_MESSAGE_REMOVE(this, this);
}

void URNPCSpawnManager::SpawnNPC(const FName& NPCID, const FName& RegionID)
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
	
	ARNPCCharacter* SpawnedNPC = World->SpawnActorDeferred<ARNPCCharacter>(NPCCharacterClass, SpawnTransform);
	SpawnedNPC->SetID(NPCID, RegionID);
	SpawnedNPC->FinishSpawning(SpawnTransform);
	
	ActiveNPC.Add({NPCID, SpawnedNPC});
}

void URNPCSpawnManager::AcquireNPC(FName NPCID)
{
	// NPC를 찾는다.
	FRNPCPooling* Slot = NPCPooling.Find(NPCID);
	
	if (nullptr == Slot)
	{
		return;
	}
	ARNPCCharacter* NPCCharacter= Slot->NPC;
	if (false == IsValid(NPCCharacter))
	{
		return;	
	}
	
	Slot->SpawnTimerHandle.Invalidate(); //무효화
	NPCPooling.Remove(NPCID);
	ActiveNPC.Add({NPCID, NPCCharacter});
	
	// NPC 활성화
	NPCCharacter->OnRegionExited.Broadcast();
}

void URNPCSpawnManager::ReleaseNPC(const FRMessage* Msg)
{
	const FRNPCEnterRegion* EnterRegionMsg = static_cast<const FRNPCEnterRegion*>(Msg);
	
	if (nullptr == EnterRegionMsg)
	{
		return;
	}
	
	const FRNPCEnterRegion& MsgRef = *EnterRegionMsg;
	
	TObjectPtr<ARNPCCharacter>* NPC = ActiveNPC.Find(MsgRef.NPCID);
	
	if (nullptr == NPC || false == IsValid(*NPC))
	{
		return;
	}
	
	(*NPC)->OnRegionEntered.Broadcast();
	
	UWorld* World = GetWorld();
	check(World);
	
	FRNPCPooling& PoolingSlot = NPCPooling.FindOrAdd(MsgRef.NPCID);
	PoolingSlot.NPC = *NPC; 
	PoolingSlot.PooledTime = World->GetTimeSeconds();
	
	// 유효하면 이미 스포너타이머가 사용 중
	if (PoolingSlot.SpawnTimerHandle.IsValid())
	{
		return;
	}
	FName CapturedNPCID = MsgRef.NPCID;

	TWeakObjectPtr<URNPCSpawnManager> ThisWeakPtr = this;

	World->GetTimerManager().SetTimer(
		PoolingSlot.SpawnTimerHandle,
		FTimerDelegate::CreateLambda([ThisWeakPtr,CapturedNPCID]()
		{
			if (false == ThisWeakPtr.IsValid())
			{
				return;
			}
			ThisWeakPtr->AcquireNPC(CapturedNPCID);
		}),
		MsgRef.HangAroundTime,
		false
	);
	
	ActiveNPC.Remove(MsgRef.NPCID);
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
			const FName NPCID = NPCData->GetID();
			const FName RegionID = NPCData->HomeID;
			const float SpawnTime = NPCData->SpawnTime;
			
			FTimerHandle TimerHandle;
			World->GetTimerManager().SetTimer(TimerHandle, [ThisWeakPtr, NPCID, RegionID]()
			{
				if (false == ThisWeakPtr.IsValid())
				{
					return;
				}
				ThisWeakPtr->SpawnNPC(NPCID, RegionID);
			}
			,SpawnTime
			,false
			);
		}
	}
}
