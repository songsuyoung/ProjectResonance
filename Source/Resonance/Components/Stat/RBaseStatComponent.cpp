#include "RBaseStatComponent.h"

// 
#include "Character/RBaseCharacter.h"
#include "Data/RBaseDataTable.h"
#include "Data/ResonanceMacro.h"
#include "Data/RNPCDataTable.h"
#include "System/RDataManager.h"
#include "Data/ResonanceStructs.h"
#include "Data/RMessage.h"
#include "System/REventManager.h"

URBaseStatComponent::URBaseStatComponent()
{
}

void URBaseStatComponent::SetViewTarget(bool bView)
{
	bViewTarget = bView;
	
	if (bViewTarget)
	{
		FRInitStat InitStat;
		InitStat.NPCID = OwnerID;
		InitStat.StatInfos = StatInfos;
	
		for (const FRStatInfo& StatInfo : StatInfos)
		{
			OnStatChanged.Broadcast(StatInfo.StatType, StatInfo.MaxValue, StatInfo.Value);
		}
	
		REVENT_MESSAGE_NOTIFY_MSG(this, ERMessageType::InitStat, InitStat);
	}
}

float URBaseStatComponent::GetCurrentStatValue(ERStatType StatType)
{
	for (FRStatInfo& StatInfo : StatInfos)
	{
		if (StatInfo.StatType == StatType)
		{
			return StatInfo.Value;
		}
	}
	
	return 0.f;
}

float URBaseStatComponent::GetMaxStatValue(ERStatType StatType)
{
	for (FRStatInfo& StatInfo : StatInfos)
	{
		if (StatInfo.StatType == StatType)
		{
			return StatInfo.MaxValue;
		}
	}
	
	return 0.f;
}

void URBaseStatComponent::UpdateStat(ERStatType StatType, float NewStatValue)
{
	for (FRStatInfo& StatInfo : StatInfos)
	{
		if (StatInfo.StatType == StatType)
		{
			float NewValue = FMath::Clamp(NewStatValue, 0.f, StatInfo.MaxValue);

			if (StatInfo.Value != NewValue)
			{
				StatInfo.Value = NewValue;
				
				FRUpdateStat UpdateStat;
				UpdateStat.NPCID = OwnerID;
				UpdateStat.StatType = StatType;
				UpdateStat.NewValue = NewValue;
	
				OnStatChanged.Broadcast(StatInfo.StatType, StatInfo.MaxValue, StatInfo.Value);			
				if (bViewTarget)
				{
					REVENT_MESSAGE_NOTIFY_MSG(this, ERMessageType::UpdateStat, UpdateStat);
				}
			}
			break;
		}
	}
}

void URBaseStatComponent::RecoverStatOnSpawn(ERStatType StatType, float RecoveryRatio)
{
	float CurrentValue = GetCurrentStatValue(StatType);
	// TODO: 추후 좋아하는 장소라면, +a를 더해주든가 한다.
	float RecoveryAmount = GetMaxStatValue(StatType) * RecoveryRatio;

	float NewValue = CurrentValue + RecoveryAmount; 
	UpdateStat(StatType, NewValue);
}

void URBaseStatComponent::GetRegionPreferences(TMap<FName, float>& OutPreferredRegions, TMap<FName, float>& OutDislikedRegions)
{
	OutPreferredRegions = PreferredRegions;
	OutDislikedRegions = DislikedRegions;
}

void URBaseStatComponent::SetRegionPreferences(const TMap<FName, float>& InPreferredRegions, const TMap<FName, float>& InDislikedRegions)
{
	PreferredRegions = InPreferredRegions;
	DislikedRegions = InDislikedRegions;
}

void URBaseStatComponent::BeginPlay()
{
	Super::BeginPlay();

	ARBaseCharacter* BaseCharacter = Cast<ARBaseCharacter>(GetOwner());

	if (false == IsValid(BaseCharacter))
	{
		return;
	}

	URDataManager* DataManager = URDataManager::Get(this);

	check(DataManager);

	OwnerID = BaseCharacter->GetID();
	
	/*FRBaseDataTable* BaseDataTable = DataManager->GetDataTableRow<FRBaseDataTable>(
		ERDataTableType::CharacterData, OwnerID);

	if (nullptr != BaseDataTable)
	{
		SetupStat(BaseDataTable->StatInfos);
	}*/

	FRNPCDataTable* NPCDataTable = DataManager->GetDataTableRow<FRNPCDataTable>(ERDataTableType::NPCDataTable,
	                                                              OwnerID);

	if (nullptr != NPCDataTable)
	{
		SetupStat(NPCDataTable->StatInfos);
		SetRegionPreferences(NPCDataTable->PreferredLocations, NPCDataTable->DislikedRegions);
	}
}
