#include "UI/RHUDBase.h"

// UE
#include "Blueprint/UserWidget.h"

void ARHUDBase::BeginPlay()
{
	Super::BeginPlay();
	
	Init();
}
