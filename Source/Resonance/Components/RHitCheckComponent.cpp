#include "Components/RHitCheckComponent.h"

// UE
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"

// Newly Created Files...
#include "Weapon/RWeaponBase.h"

URHitCheckComponent::URHitCheckComponent()
{

}

void URHitCheckComponent::BeginPlay()
{
	Super::BeginPlay();


}

void URHitCheckComponent::ProcessAttackHit(AActor* Weapon)
{
	if (nullptr == Weapon)
	{
		return;
	}

	ARWeaponBase* WeaponBase = Cast<ARWeaponBase>(Weapon);

	if (false == IsValid(WeaponBase))
	{
		return;
	}

	// Interface를 만들어서, 전투/HitCheck/데미지 까지 적용할 수 있는 값을 만들어 낸다.
	TSet<AActor*> Targets;

	TMap<FName, FVector> CurrentOffset;

	WeaponBase->GetHitCheckSocketLocation(CurrentOffset);

	// 누적 값이 있으면 오버랩 생성
	if (false == AccumulatedOffset.IsEmpty())
	{
		InternalProcessHitCheck(Targets, CurrentOffset);
	}

	AccumulatedOffset = MoveTemp(CurrentOffset); // 현재 값을 시동시킨다.

}

void URHitCheckComponent::HandleMontageStarted(UAnimMontage* Montage)
{
	// 어떤 애니메이션이든 상관없이 애니메이션 실행전 항상 초기화
	AccumulatedOffset.Empty();
}

void URHitCheckComponent::InternalProcessHitCheck(TSet<AActor*> OutOverlapActor, const TMap<FName, FVector>& CurrentOffset)
{
	// 있으면 오버랩을 생성한다.
	UWorld* World = GetWorld();

	check(World);

	// Pos => Capsule 시 정 가운데 위치
	for (const TPair<FName, FVector>& Offset : AccumulatedOffset)
	{
		const FVector& PreOffset = Offset.Value;
		const FVector& CurOffset = CurrentOffset[Offset.Key];

		// 정 가운데 위치는 빼준다.
		const FVector& OriginOffset = (CurOffset + PreOffset) * 0.5f; //원점 중앙

		const FVector& Delta = PreOffset - CurOffset;
		const FVector& Dir = Delta.GetSafeNormal(); // 어느 방향으로 이동하는지

		// 실제 Dir으로 투영시켜야 한다.
		FQuat CapsuleRotation = FRotationMatrix::MakeFromZ(Dir).ToQuat();

		// 뺀 길이 만큼
		float Distance = FVector::Dist(PreOffset, CurOffset) * 0.5f;

		TArray<FOverlapResult> OverlapResults;
	
		FCollisionQueryParams CollisionQueryParams;
		
		// 높이(길이)가 원점과 끝점 까지의 길이어야함.
		FCollisionShape Shape = FCollisionShape::MakeCapsule(10, Distance);
		bool bResult = World->OverlapMultiByChannel(OverlapResults,
			OriginOffset,
			CapsuleRotation,
			ECollisionChannel::ECC_Visibility, // 적에만 부여하도록 변경해야함. 이그노어 설정안하려면
			Shape,
			CollisionQueryParams
		);

		DrawDebugCapsule(
			World,
			OriginOffset,
			Distance,
			10,
			CapsuleRotation,
			FColor::MakeRandomColor(),
			false,
			3.f
		);
	}
}