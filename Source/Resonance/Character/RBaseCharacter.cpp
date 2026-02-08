#include "Character/RBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "System/RStateContext.h"

ARBaseCharacter::ARBaseCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 1인칭이 아닌 3인칭 모드
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// 회전 속도
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

}

void ARBaseCharacter::PushStateGameTag(const FGameplayTag& StateTag)
{

	if (false == IsValid(StateContext))
	{
	//	StateContext = NewObject<URStateContext>(this, URStateContext::StaticClass());
	}

	if (false == StateContext->ContainTag(StateTag))
	{
		CurrentStateTags.AddTag(StateTag);
	//	StateContext->PushTag(StateTag);
	}
}

void ARBaseCharacter::PopStateGameTag(const FGameplayTag& StateTag)
{
	CurrentStateTags.RemoveTag(StateTag);
}
