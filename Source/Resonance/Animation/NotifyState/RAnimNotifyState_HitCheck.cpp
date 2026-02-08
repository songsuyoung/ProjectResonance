#include "RAnimNotifyState_HitCheck.h"

// UE
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"

// New Created Files...
#include "Character/ResonanceCharacter.h"
#include "Components/RHitCheckComponent.h"
#include "Components/RCombatComponent.h"
#include "Weapon/RWeaponBase.h"

URAnimNotifyState_HitCheck::URAnimNotifyState_HitCheck()
	: Super()
	, bDebugDraw(true)
	, FixedFrame(60.f)
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

	const float FixedFPS = 1.f / FMath::Max(1.f, FixedFrame); // 1초당 60프레임 0.0166

	// 반올림 진행, 1번은 실행되어야 한다.
	const int32 Count = FMath::Max(1, FMath::CeilToInt(FrameDeltaTime / FixedFPS));

	for (int FrameIndex = 0; FrameIndex < Count; FrameIndex++)
	{
		// 0 - 1.0
		float Alpha = float(FrameIndex + 1) / (float)Count;

		// 현재 위치 - 이전 위치에 대해 보간 진행 
		// 선형보간의 경우 구형아닌 선형으로 진행되기 때문에 내부 안쪽으로 결정된다.
		FVector BeginPoint = FMath::Lerp(PrePoint[0], CurrentPoint[0], Alpha); // 칼머리 부분

		// 방향 길이 보간
		// Slerp를 통해 이전 프레임의 무기 각도와 현재 프레임의 각도 사이를 보간

		FVector StartDirVec = (PrePoint[1] - PrePoint[0]).GetSafeNormal();
		FVector EndDirVec = (CurrentPoint[1] - CurrentPoint[0]).GetSafeNormal();

		FQuat StartQuat = StartDirVec.Rotation().Quaternion();
		FQuat EndQuat = EndDirVec.Rotation().Quaternion();
	
		FQuat InterpRot = FQuat::Slerp(StartQuat, EndQuat, Alpha); 
		FQuat FinalRot = FRotationMatrix::MakeFromZ(InterpRot.GetForwardVector()).ToQuat();

		// 칼의 벡터방향과 캡슐과 다름 90도 회전해주어야 한다.
		///// 방향 보간 (회전 구간을 보간 해줄 때에는 Slerp 사용해야함

		// 길이 보간 : 이유 - 보간에 의해서 칼의 소켓 지점이 짧아지는 문제가 존재
		// 이전 위치 - 현재 위치 간의 길이 보간으로 짧아지는 문제점 해결 
		float BeginLength = FVector::Distance(PrePoint[0], PrePoint[1]);
		float EndLength = FVector::Distance(CurrentPoint[0], CurrentPoint[1]);

		float HalfLength = FMath::Lerp(BeginLength, EndLength, Alpha) * 0.5f;

		////// 길이 보간
		
		// 중앙 위치
		// 계산이 빠르려면, 나눗셈 보다 *이 빠름을 알아야 한다.
		const FVector Origin = BeginPoint + (FinalRot.GetUpVector() * HalfLength);


		UWorld* World = MeshComp->GetWorld();

		check(World);
		TArray<FOverlapResult> OutOverlaps;

		// Capsule의 경우 가로로 눕혀있기 때문에, 회전 시켜주어야 함. Up으로 올린 후 계산을 진행해야함.
		FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(10.f, HalfLength);

		FCollisionQueryParams Params;

		Params.AddIgnoredActor(Weapon.Get());
		Params.AddIgnoredActor(MeshComp->GetOwner());
		bool bResult = World->OverlapMultiByChannel
		(
			OutOverlaps,
			Origin,
			FinalRot,
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
				FinalRot,
				FColor::MakeRandomColor(),
				false,
				10.f
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
