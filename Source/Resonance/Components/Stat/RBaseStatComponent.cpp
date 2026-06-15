#include "RBaseStatComponent.h"

// 
#include "Character/RBaseCharacter.h"
#include "Data/RBaseDataTable.h"
#include "System/RDataManager.h"
#include "System/ResonanceMacro.h"
#include "System/REventManager.h"
#include "System/RMessage.h"
#include "UI/System/ResonanceUIStructs.h"

URBaseStatComponent::URBaseStatComponent()
{
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
				REVENT_MESSAGE_NOTIFY_MSG(this, ERMessageType::UpdateStat, UpdateStat);
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

void URBaseStatComponent::SetupStat(const TArray<FRStatInfo>& InStatInfos)
{
	StatInfos = InStatInfos;

	FRInitStat InitStat;
	InitStat.NPCID = OwnerID;
	for (const FRStatInfo& StatInfo : StatInfos)
	{
		InitStat.StatInfos.Add(FRUIStatInfo(StatInfo.StatType, StatInfo.Value));
		OnStatChanged.Broadcast(StatInfo.StatType, StatInfo.MaxValue, StatInfo.Value);
	}
	
	REVENT_MESSAGE_NOTIFY_MSG(this, ERMessageType::InitStat, InitStat);
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

	FRBaseDataTable* BaseDataTable = DataManager->GetDataTableRow<FRBaseDataTable>(ERDataTableType::NPCDataTable,
	                                                              OwnerID);

	if (nullptr != BaseDataTable)
	{
		SetupStat(BaseDataTable->StatInfos);
	}
}
