#include "Animation/RNPCAnimInstance.h"

// UE 5. Files..
#include "Character/RNPCCharacter.h"
#include "Components/RJobComponent.h"

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
	HoldToolAlpha = FMath::FInterpTo(HoldToolAlpha, bHasTool? 1.0f:0.f, DeltaSeconds, 8.0);
	LookAtAlpha =  FMath::FInterpTo(HoldToolAlpha, bLookAt? 1.0f:0.f, DeltaSeconds, 8.0);
	
	if (LookAtAlpha > 0.2f)
	{
		LookAtLocation = OwnerCharacter->GetLookAtLocation();
	}
}
