#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RHeroComponent.generated.h"

class UInputAction;
struct FInputActionValue;
class UInputMappingContext;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RESONANCE_API URHeroComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	URHeroComponent();

	void SetupInputComponent();

protected:
	
	virtual void BeginPlay() override;

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump();
	void Attack();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/*
	* TODO : 데이터화 시켜 데이터 매니저로부터 얻어올 수 있도록 해야한다.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CombatInputAction;


protected:

	UPROPERTY(Transient)
	TWeakObjectPtr<class APlayerController> PlayerController;

	UPROPERTY(Transient)
	TWeakObjectPtr<class ACharacter> OwnerPawn;

};
