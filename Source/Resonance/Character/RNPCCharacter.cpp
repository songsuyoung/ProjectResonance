#include "RNPCCharacter.h"

// UE 5.

// 
#include "NPC/System/RAIController.h"

ARNPCCharacter::ARNPCCharacter()
	: Super()
{
	AIControllerClass = ARAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ARNPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
