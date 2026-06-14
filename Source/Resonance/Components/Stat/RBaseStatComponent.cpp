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
				UpdateStat.StatType = StatType;
				UpdateStat.NewValue = NewValue;
				
				REVENT_MESSAGE_NOTIFY_MSG(this, ERMessageType::UpdateStat, UpdateStat);
			}
			break;
		}
	}
}

void URBaseStatComponent::SetupStat(const TArray<FRStatInfo>& InStatInfos)
{
	StatInfos = InStatInfos;

	FRInitStat InitStat;
	
	for (const FRStatInfo& StatInfo : StatInfos)
	{
		InitStat.StatInfos.Add(FRUIStatInfo(StatInfo.StatType, StatInfo.Value));
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

	FRBaseDataTable* BaseDataTable = DataManager->GetDataTableRow<FRBaseDataTable>(
		ERDataTableType::CharacterData, BaseCharacter->GetID());

	if (nullptr != BaseDataTable)
	{
		SetupStat(BaseDataTable->StatInfos);
	}

	BaseDataTable = DataManager->GetDataTableRow<FRBaseDataTable>(ERDataTableType::NPCDataTable,
	                                                              BaseCharacter->GetID());

	if (nullptr != BaseDataTable)
	{
		SetupStat(BaseDataTable->StatInfos);
	}
}
