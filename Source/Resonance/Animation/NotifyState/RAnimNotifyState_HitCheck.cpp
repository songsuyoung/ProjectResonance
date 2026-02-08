#include "RAnimNotifyState_HitCheck.h"

// UE
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"

// New Created Files...
#include "Character/ResonanceCharacter.h"
#include "Components/RHitCheckComponent.h"
#include "Components/RCombatComponent.h"
#include "Weapon/RWeaponBase.h"

void URAnimNotifyState_HitCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (false == IsValid(MeshComp))
	{
		return;
	}

	AResonanceCharacter* Character = Cast<AResonanceCharacter>(MeshComp->GetOwner());

	if (IsValid(Character))
	{
		HitCheckComponent = Character->GetHitCheckComponent();
	}

	URCombatComponent* CombatComponent = Character->GetCombatComponent();

	if (IsValid(CombatComponent))
	{
		Weapon = CombatComponent->GetWeapon();
	}

	if (Weapon.IsValid())
	{
		Weapon->GetHitCheckSocketLocation(PrePoint);
	}
}

void URAnimNotifyState_HitCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (false == IsValid(MeshComp))
	{
		return;
	}
	// 매 틱마다 CurrentPoint가 존재.
	TArray<FVector> CurrentPoint;

	if (Weapon.IsValid())
	{
		Weapon->GetHitCheckSocketLocation(CurrentPoint);
	}
	
	if (PrePoint.Num() < 2 || CurrentPoint.Num() < 2 || PrePoint == CurrentPoint)
	{
		// 2개보다 작은 경우는 없음.
		return;
	}

	const float FixedFPS = 1.f / 60.f; // 1초당 60프레임 0.0166
	
	// 반올림 진행, 1번은 실행되어야 한다.
	const int32 Count = FMath::Max(1, FMath::CeilToInt(FrameDeltaTime / FixedFPS));

	for (int FrameIndex = 0; FrameIndex < Count; FrameIndex++)
	{
		// 0 - 1.0
		float Alpha = float(FrameIndex + 1) / (float)Count;

		// 현재 위치 - 이전 위치에 대해 보간 진행 
		FVector BeginPoint = FMath::Lerp(PrePoint[0], CurrentPoint[0], Alpha); // 칼머리 부분
		FVector EndPoint = FMath::Lerp(PrePoint[1], CurrentPoint[1], Alpha); // 칼꼬리 부분

		// 중앙 위치
		// 계산이 빠르려면, 나눗셈 보다 *이 빠름을 알아야 한다.
		const FVector Origin = (BeginPoint + EndPoint) * 0.5f;

		FVector Dir = EndPoint - BeginPoint; // 방향과 크기

		// 길이가 짧아지는 문제가 있다.
		// 이 문제를 해결하기 위해 길이는 고정 길이를 이용한다.
		float HalfLength = FVector::Distance(CurrentPoint[0], CurrentPoint[1]) * 0.5f;

		// 회전에 대해 보간 진행 ? ->  시작점과 끝점이 있는데 EndPoint - BeginPoint  하면 방향이 존재
		FVector NormalDir = Dir.GetSafeNormal();

		UWorld* World = MeshComp->GetWorld();

		check(World);
		TArray<FOverlapResult> OutOverlaps;

		FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(10.f, HalfLength);
		FCollisionQueryParams Params;

		Params.AddIgnoredActor(Weapon.Get());
		Params.AddIgnoredActor(MeshComp->GetOwner());
		bool bResult = World->OverlapMultiByChannel
		(
			OutOverlaps,
			Origin,
			NormalDir.ToOrientationQuat(),
			ECollisionChannel::ECC_Visibility,
			CollisionShape,
			Params
		);

	
		if (bDebugDraw)
		{
			DrawDebugCapsule(
				World,
				Origin,
				HalfLength,
				10,
				NormalDir.ToOrientationQuat(),
				FColor::MakeRandomColor(),
				false,
				3.f
			);
		}

	}

	PrePoint = MoveTemp(CurrentPoint);
}

void URAnimNotifyState_HitCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	PrePoint.Empty();

	if (HitCheckComponent.IsValid())
	{
		HitCheckComponent->ProcessAttackHit(Target);
	}
}
