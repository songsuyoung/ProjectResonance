#include "Animation/RNPCAnimInstance.h"

// UE 5. Files..
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


void URNPCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (false == CharacterMovementComponent.IsValid() || false == OwnerCharacter.IsValid())
	{
		return;
	}
	
	bShouldMove = Speed > 3.0f;
}

