#include "UI/RHUD.h"

// UE
#include "Blueprint/UserWidget.h"

void ARHUD::BeginPlay()
{
	Super::BeginPlay();

	PrimaryLayout = CreateWidget<UUserWidget>(GetOwningPlayerController(), PrimaryLayoutClass);

	if (IsValid(PrimaryLayout))
	{
		PrimaryLayout->AddToViewport();
	}
}
