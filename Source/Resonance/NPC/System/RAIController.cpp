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
	
	OnRegionVisited.AddUObject(this, &ThisClass::VisitRegion);
}

void ARAIController::OnUnPossess()
{
	Super::OnUnPossess();
	
	OnRegionVisited.RemoveAll(this);
}

void ARAIController::VisitRegion(FName RegionID)
{
	BP_OnRegionVisited(RegionID);
}