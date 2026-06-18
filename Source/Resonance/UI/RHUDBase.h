#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RHUDBase.generated.h"

class UUserWidget;

UCLASS()
class RESONANCE_API ARHUDBase : public AHUD
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	
protected:
	
	virtual void Init() { }
};
