#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RHUD.generated.h"

class UUserWidget;

UCLASS()
class RESONANCE_API ARHUD : public AHUD
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
protected:

	UPROPERTY(EditAnywhere, Category = "PrimaryLayoutClass")
	TSubclassOf<UUserWidget> PrimaryLayoutClass;

protected:

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> PrimaryLayout;
};
