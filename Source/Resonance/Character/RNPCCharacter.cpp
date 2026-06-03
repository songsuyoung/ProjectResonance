#include "RNPCCharacter.h"

// UE 5.
#include "Components/SplineComponent.h"

// 
#include "NPC/System/RAIController.h"

ARNPCCharacter::ARNPCCharacter()
	: Super()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	AIControllerClass = ARAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ARNPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
