#include "RAIController.h"

// UE 5.
#include "Character/RNPCCharacter.h"
#include "Components/StateTreeAIComponent.h"

ARAIController::ARAIController()
{
	StateTreeComponent = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeAIComponent"));
}

void ARAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (IsValid(StateTreeComponent))
	{
		StateTreeComponent->StartLogic();
	}
	
	ARNPCCharacter* NPCCharacter = Cast<ARNPCCharacter>(InPawn);
	
	if (IsValid(NPCCharacter))
	{
		NPCCharacter->OnRegionExited.AddUObject(this, &ThisClass::HandleRegionExited);
		NPCCharacter->OnRegionEntered.AddUObject(this, &ThisClass::HandleRegionEntered);
	}
}

void ARAIController::OnUnPossess()
{
	ARNPCCharacter* NPCCharacter = Cast<ARNPCCharacter>(GetOwner());
	
	if (IsValid(NPCCharacter))
	{
		NPCCharacter->OnRegionExited.RemoveAll(this);
		NPCCharacter->OnRegionEntered.RemoveAll(this);
	}
	
	Super::OnUnPossess();
	
	if (IsValid(StateTreeComponent))
	{
		StateTreeComponent->StopLogic(TEXT("UnPossess"));
	}

}

void ARAIController::HandleRegionExited()
{
	if (IsValid(StateTreeComponent))
	{
		StateTreeComponent->StartLogic();
	}
}

void ARAIController::HandleRegionEntered()
{
	if (IsValid(StateTreeComponent))
	{
		StateTreeComponent->StopLogic(TEXT("VisitRegion"));
	}
}
