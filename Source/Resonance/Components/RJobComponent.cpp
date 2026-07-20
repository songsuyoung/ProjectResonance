#include "Components/RJobComponent.h"

#include "Character/RBaseCharacter.h"
#include "Components/StateTreeAIComponent.h"
#include "Engine/AssetManager.h"
#include "GameFramework/Character.h"
#include "NPC/System/RAIController.h"
#include "Resonance/Data/ResonanceStructs.h"
#include "System/RNPCSpawnManager.h"
#include "System/RTimeManager.h"

URJobComponent::URJobComponent()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
}

void URJobComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Character = Cast<ARBaseCharacter>(GetOwner());
	
	if (Character.IsValid())
	{
		ARAIController* AIController = Cast<ARAIController>(Character->GetController());
		
		if (IsValid(AIController))
		{
			StateTreeAIComponent = AIController->GetStateTreeAIComponent();
		}
	}
	
	URTimeManager* TimeManager = URTimeManager::Get(this);
	check(TimeManager);
	
	TimeManager->OnHourChanged.AddUObject(this, &ThisClass::OnHourChanged);
}

void URJobComponent::SetToolMesh(const FRToolMeshData& ToolMeshData)
{
	if (Character.IsValid())
	{
		StaticMeshComponent->AttachToComponent(
			Character->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			ToolMeshData.SocketName
		);
	}
	
	StaticMeshComponent->SetRelativeRotation(ToolMeshData.RelativeRotation);
	StaticMeshComponent->SetRelativeLocation(ToolMeshData.RelativeLocation);
	StaticMeshComponent->SetStaticMesh(ToolMeshData.Mesh);
}

void URJobComponent::SetPendingToolType(ERToolType InToolType)
{
	PendingToolType = InToolType;
}

void URJobComponent::ApplyToolMesh(ERToolType ToolType)
{
	if (false == IsValid(StaticMeshComponent))
	{
		return;
	}
	
	const FRToolMeshData* ToolMeshData = ToolMeshMap.Find(PendingToolType);
	if (nullptr == ToolMeshData)
	{
		ToolMeshData = ToolMeshMap.Find(ToolType);
		PendingToolType = ToolType;
		if (nullptr == ToolMeshData)
		{
			ClearToolMesh();
			return;
		}
	}
	
	if (IsValid((*ToolMeshData).Mesh))
	{
		SetToolMesh(*ToolMeshData);
	}
	else
	{
		StreamableHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
			(*ToolMeshData).MeshObj.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &URJobComponent::OnMeshLoaded));
	}

	CurrentToolType = PendingToolType;
}

void URJobComponent::ClearToolMesh()
{
	if (IsValid(StaticMeshComponent))
	{
		StaticMeshComponent->SetStaticMesh(nullptr);
	}
	
	CurrentToolType = ERToolType::None;
}

bool URJobComponent::IsToolChangeRequired() const
{
	return CurrentToolType != PendingToolType;
}

bool URJobComponent::HasTool() const
{
	return ToolMeshMap.Contains(CurrentToolType);
}

void URJobComponent::OnHourChanged(int NewHour)
{
	FName EventGameTagName;
	
	bool bIsDiff = false;
	if (NewHour >= StartWorkHour && NewHour < EndWorkHour)
	{
		EventGameTagName = TEXT("StateTree.Task.Event.Work");
		
		bIsDiff = true;
	}
	else
	{
		EventGameTagName = TEXT("StateTree.Task.Event.StopWork");
		bIsDiff = false;
	}
	
	if (bWork != bIsDiff)
	{
		if (bIsDiff)
		{
			// 일해야하는데 SpawnManager에 의해 잠들어있을 수 있음.
			URNPCSpawnManager* SpawnManager = URNPCSpawnManager::Get(this);
			check(SpawnManager);
			SpawnManager->AcquireNPC(Character->GetID());
		}
		if (StateTreeAIComponent.IsValid())
		{
			StateTreeAIComponent->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(EventGameTagName));
		}
		// 전달
		
		bWork = bIsDiff;
	}
}

void URJobComponent::OnMeshLoaded()
{
	FRToolMeshData* ToolMeshData = ToolMeshMap.Find(CurrentToolType);
	if (nullptr == ToolMeshData)
	{
		return;
	}
	
	ToolMeshData->Mesh = ToolMeshData->MeshObj.Get();
	
	SetToolMesh(*ToolMeshData);
}
