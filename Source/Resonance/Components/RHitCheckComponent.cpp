#include "Components/RHitCheckComponent.h"

// UE
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"

// Newly Created Files...
#include "Weapon/RWeaponBase.h"

URHitCheckComponent::URHitCheckComponent()
{

}

void URHitCheckComponent::ProcessAttackHit(const TArray<FOverlapResult>& InTarget)
{
	TSet<AActor*> TargetActor;
	TArray<FOverlapResult> OverlapResults;


	for (const FOverlapResult& Target : InTarget)
	{
		if (false == TargetActor.Contains(Target.GetActor()))
		{
			OverlapResults.Add(Target);
			TargetActor.Add(Target.GetActor());
		}
	}

	InternalProcessHitCheck(OverlapResults);
}

void URHitCheckComponent::InternalProcessHitCheck(const TArray<FOverlapResult>& InTarget)
{
	// 처리한다.
}


