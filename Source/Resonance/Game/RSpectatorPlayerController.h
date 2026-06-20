#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RSpectatorPlayerController.generated.h"

enum class ERActionContext : uint8;
class URHeroComponent;

UCLASS()
class RESONANCE_API ARSpectatorPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ARSpectatorPlayerController();
	
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
protected:
	void OnInputReceived(ERActionContext ActionContext);
	void SwitchObserveTarget();
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<URHeroComponent> HeroComponent;
	
protected:
	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> Spectator;
};
