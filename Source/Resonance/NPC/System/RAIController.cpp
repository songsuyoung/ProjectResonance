#include "RAIController.h"

// UE 5.
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
}
