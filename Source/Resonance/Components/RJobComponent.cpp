#include "Components/RJobComponent.h"

#include "GameFramework/Character.h"
#include "NPC/Task/Farm/RSTTask_FarmWork.h"

URJobComponent::URJobComponent()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
}

void URJobComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URJobComponent::SetToolMesh(const FToolMeshData& ToolMeshData)
{
	if (false == IsValid(StaticMeshComponent))
	{
		return;
	}
	
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (IsValid(Character))
	{
		// JobComponent 자체를 소켓에 붙임
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
