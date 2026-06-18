#include "Game/RSpectatorPlayerController.h"

#include "Character/RCameraCharacter.h"
#include "Components/RHeroComponent.h"
#include "Data/ResonanceEnums.h"
#include "System/RNPCSpawnManager.h"
#include "Character/RNPCCharacter.h"

ARSpectatorPlayerController::ARSpectatorPlayerController()
{
	HeroComponent = CreateDefaultSubobject<URHeroComponent>(TEXT("HeroComponent"));
}

void ARSpectatorPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsValid(HeroComponent))
	{
		HeroComponent->SetupInputComponent();
		HeroComponent->OnInputReceived.AddUObject(this, &ThisClass::OnInputReceived);
	}
}

void ARSpectatorPlayerController::OnInputReceived(ERActionContext ActionContext)
{
	switch (ActionContext)
	{
	case ERActionContext::Action_Move:
		SwitchObserveTarget();
		break;
	}
}

void ARSpectatorPlayerController::SwitchObserveTarget()
{
	URNPCSpawnManager* NPCSpawnManager= URNPCSpawnManager::Get(this);
	
	check(NPCSpawnManager);
	
	ARBaseCharacter* BaseCharacter = Cast<ARBaseCharacter>(NPCSpawnManager->GetObserveTarget());
	
	if (IsValid(BaseCharacter))
	{
		SetViewTarget(BaseCharacter);
	}
}
