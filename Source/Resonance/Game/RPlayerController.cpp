#include "Game/RPlayerController.h"
#include "Components/RHeroComponent.h"

ARPlayerController::ARPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HeroComponent = CreateDefaultSubobject<URHeroComponent>("HeroComponent");
}

void ARPlayerController::SetupInputComponent()
{
	if (IsValid(HeroComponent))
	{
		HeroComponent->SetupInputComponent();
	}
}