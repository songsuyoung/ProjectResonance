#include "Components/RHitCheckComponent.h"

// UE
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"

// Newly Created Files...
#include "Weapon/RWeaponBase.h"

URHitCheckComponent::URHitCheckComponent()
{

}

void URHitCheckComponent::ProcessAttackHit(const TArray<FHitResult>& InTarget)
{
	TSet<AActor*> TargetActor;
	TArray<FHitResult> OverlapResults;
	
	for (const FHitResult& Target : InTarget)
	{
		if (false == TargetActor.Contains(Target.GetActor()))
		{
			OverlapResults.Add(Target);
			TargetActor.Add(Target.GetActor());
		}
	}

	InternalProcessHitCheck(OverlapResults);
}

void URHitCheckComponent::InternalProcessHitCheck(const TArray<FHitResult>& InTarget)
{
	// ó���Ѵ�.
}


