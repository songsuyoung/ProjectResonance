#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPlayerController.generated.h"

class URHeroComponent;

UCLASS()
class RESONANCE_API ARPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ARPlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void SetupInputComponent() override;

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<URHeroComponent> HeroComponent;

};
