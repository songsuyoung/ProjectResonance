#include "Animation/RAnimInstance.h"

// UE 5. Files..
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void URAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	OwnerCharacter = Cast<ACharacter>(GetOwningActor());
	
	if (OwnerCharacter.IsValid())
	{
		CharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void URAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (false == CharacterMovementComponent.IsValid() || false == OwnerCharacter.IsValid())
	{
		return;
	}
	Velocity = OwnerCharacter->GetVelocity();
	Direction = Velocity.GetSafeNormal();
	Speed = Velocity.Size2D(); // Z값을 제외하고, 값을 구해야하기 때문에 Speed2D 함수를 사용한다.
	bIsFalling = CharacterMovementComponent->IsFalling();
	bShouldMove = false == CharacterMovementComponent->GetCurrentAcceleration().IsNearlyZero() && Speed > 3.0f;
}
