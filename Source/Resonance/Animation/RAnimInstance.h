#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RAnimInstance.generated.h"

class ACharacter;
class UCharacterMovementComponent;

UENUM(BlueprintType)
enum class ERAnimType : uint8
{
	In_Place,
	RootMotion
};
UCLASS()
class RESONANCE_API URAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	void SetDesiredDirection(FVector InDirection, ERAnimType AnimType = ERAnimType::In_Place);
	
protected:
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(Transient, BlueprintReadOnly)
	FVector Velocity;
	
	UPROPERTY(Transient, BlueprintReadOnly)
	float Speed;
	
	UPROPERTY(Transient, BlueprintReadOnly)
	FVector Direction;
	
	UPROPERTY(Transient, BlueprintReadOnly)
	uint8 bIsFalling : 1;
	
	UPROPERTY(Transient, BlueprintReadOnly)
	uint8 bShouldMove : 1;
	
	UPROPERTY(Transient, BlueprintReadOnly)
	TWeakObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;
	
	UPROPERTY(Transient, BlueprintReadOnly)
	TWeakObjectPtr<ACharacter> OwnerCharacter;
};
