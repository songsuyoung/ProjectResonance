#pragma once

#include "CoreMinimal.h"
#include "StateContext/RStateContext.h"
#include "RMove.generated.h"

class ACharacter;
class UEnhancedPlayerInput;
class UInputAction;
class APlayerController;

UCLASS()
class RESONANCE_API URMove : public URStateContext
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(AActor* InOwner) override;
	virtual void Execute() override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Move|Condition")
	TObjectPtr<UInputAction> MoveInputAction;
	
protected:
	
	UPROPERTY(Transient)
	TWeakObjectPtr<ACharacter> CharacterRef;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<APlayerController> PlayerControllerRef;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<UEnhancedPlayerInput> EnhancedComponentRef;
};
