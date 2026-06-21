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
	Speed = Velocity.Size2D(); // Z값을 제외하고, 값을 구해야하기 때문에 Speed2D 함수를 사용한다.
	bIsFalling = CharacterMovementComponent->IsFalling();
	bShouldMove = false == CharacterMovementComponent->GetCurrentAcceleration().IsNearlyZero() && Speed > 3.0f;
}

void URAnimInstance::SetDesiredDirection(FVector InDirection, ERAnimType AnimType)
{
	switch (AnimType)
	{
	case ERAnimType::In_Place:
		// Root-Lock 모드에서는 외부 Direction을 무시하고 실제 이동 속도를 기준으로 삼는다.
		// (InDirection은 RootMotion 모드 전용 파라미터)
		Direction = Velocity.GetSafeNormal();
		break;

	case ERAnimType::RootMotion:
		Direction = InDirection;
		break;
	}
}
