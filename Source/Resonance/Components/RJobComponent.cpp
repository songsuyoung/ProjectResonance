#include "Components/RJobComponent.h"

#include "Engine/AssetManager.h"
#include "GameFramework/Character.h"
#include "Resonance/Data/ResonanceStructs.h"

URJobComponent::URJobComponent()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
}

void URJobComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URJobComponent::SetToolMesh(const FRToolMeshData& ToolMeshData)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (IsValid(Character))
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

void URJobComponent::ApplyToolMesh()
{
	if (false == IsValid(StaticMeshComponent))
	{
		return;
	}
	
	const FRToolMeshData* ToolMeshData = ToolMeshMap.Find(PendingToolType);
	if (nullptr == ToolMeshData)
	{
		StaticMeshComponent->SetStaticMesh(nullptr);
		return;
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

bool URJobComponent::IsToolChangeRequired() const
{
	return CurrentToolType != PendingToolType;
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
