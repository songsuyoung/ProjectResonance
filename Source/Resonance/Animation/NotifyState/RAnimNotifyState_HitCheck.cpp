#include "RAnimNotifyState_HitCheck.h"

// UE
#include "DrawDebugHelpers.h"
#include "CollisionShape.h"

// New Created Files...
#include "Character/ResonanceCharacter.h"
#include "Components/RHitCheckComponent.h"
#include "Components/RCombatComponent.h"
#include "Weapon/RWeaponBase.h"

URAnimNotifyState_HitCheck::URAnimNotifyState_HitCheck()
	: Super()
	, bDebugDraw(true)
	, CollisionHalfLength(50.f)
	, DiffDist(60.f)
{
}

void URAnimNotifyState_HitCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (false == IsValid(MeshComp))
	{
		return;
	}

	AResonanceCharacter* Character = Cast<AResonanceCharacter>(MeshComp->GetOwner());

	if (false == IsValid(Character))
	{
		return;
	}

	HitCheckComponent = Character->GetHitCheckComponent();

	URCombatComponent* CombatComponent = Character->GetCombatComponent();

	if (IsValid(CombatComponent))
	{
		Weapon = CombatComponent->GetWeapon();
	}
	
	// 초기화
	World = MeshComp->GetWorld();
	PrevBoneTransform = MeshComp->GetSocketTransform(Weapon->GetSocketName(ERWeaponAttachLocation::Hand), RTS_World);
	
	Params.AddIgnoredActor(Weapon.Get());
	Params.AddIgnoredActor(MeshComp->GetOwner());
}

void URAnimNotifyState_HitCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	if (false == Weapon.IsValid())
	{
		return;
	}
	
	FTransform PrevWorldTF = PrevBoneTransform;
	FTransform CurrWorldTF = MeshComp->GetSocketTransform(Weapon->GetSocketName(ERWeaponAttachLocation::Hand), RTS_World);

	TArray<FVector> PreLocations;
	PreLocations.Add(PrevWorldTF.GetLocation());
	PreLocations.Add(PrevWorldTF.GetLocation() + PrevWorldTF.GetUnitAxis(EAxis::X) * CollisionHalfLength);
	PreLocations.Add(PrevWorldTF.GetLocation() + PrevWorldTF.GetUnitAxis(EAxis::X) * CollisionHalfLength * 2.f);

	float Dist = FVector::Dist(PrevWorldTF.GetLocation(), CurrWorldTF.GetLocation());
	int32 NumSteps = FMath::Max(1, FMath::FloorToInt(Dist / DiffDist));

	for (int Step = 1; Step <= NumSteps; Step++)
	{
		float Alpha = (float)Step / (float)NumSteps;
		FTransform LerpWorldTF;
		LerpWorldTF.Blend(PrevWorldTF, CurrWorldTF, Alpha);

		TArray<FVector> LerpLocations;
		LerpLocations.Add(LerpWorldTF.GetLocation());
		LerpLocations.Add(LerpWorldTF.GetLocation() + LerpWorldTF.GetUnitAxis(EAxis::X) * CollisionHalfLength);
		LerpLocations.Add(LerpWorldTF.GetLocation() + LerpWorldTF.GetUnitAxis(EAxis::X) * CollisionHalfLength * 2.f);

		TryHitCheck(LerpLocations[0], LerpLocations[2]);

		for (int i = 0; i < PreLocations.Num(); i++)
			TryHitCheck(PreLocations[i], LerpLocations[i]);

		for (int i = 0; i < PreLocations.Num() - 1; i++)
			TryHitCheck(PreLocations[i], LerpLocations[i + 1]);

		PreLocations = LerpLocations;
	}

	PrevBoneTransform = CurrWorldTF;
}

void URAnimNotifyState_HitCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (HitCheckComponent.IsValid())
	{
		HitCheckComponent->ProcessAttackHit(Target);
	}
}

bool URAnimNotifyState_HitCheck::TryHitCheck(const FVector& StartLocation, const FVector& TargetLocation)
{
	bool bResult = false;

	if (false == World.IsValid() || false == Weapon.IsValid())
	{
		return bResult;
	}
	
	TArray<FHitResult> HitResults;
	
	bResult = World->SweepMultiByChannel(
		HitResults,
		StartLocation,
		TargetLocation,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape(),
		Params
	);

	if (bDebugDraw)
	{
		DrawDebugLine(
			World.Get(),
			StartLocation,
			TargetLocation,
			FColor::Red,
			false,
			10.f);	
	}

	if (false == bResult)
	{
		return bResult;
	}
	
	Target.Append(HitResults);
	
	return bResult;
}
