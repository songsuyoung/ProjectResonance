#include "Animation/RNPCAnimInstance.h"

// UE 5. Files..
#include "Character/RNPCCharacter.h"
#include "Components/RJobComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void URNPCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ARNPCCharacter* NPCCharacter = Cast<ARNPCCharacter>(OwnerCharacter);
	if (IsValid(NPCCharacter))
	{
		JobComponent = NPCCharacter->GetJobComponent();
	}
}

void URNPCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (false == JobComponent.IsValid())
	{
		return;
	}

	bShouldMove = Speed > 3.0f;
	bHasTool = JobComponent->HasTool();
}

