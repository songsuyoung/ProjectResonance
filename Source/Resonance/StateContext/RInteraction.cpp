#include "RInteraction.h"

#include "DrawDebugHelpers.h"    

#include "Character/ResonanceCharacter.h"
#include "Engine/OverlapResult.h"
#include "Interface/RInteractable.h"

void URInteraction::Execute()
{
	UWorld* World = GetWorld();

	check(World);

	TArray<FOverlapResult> OverlapResult;
	FCollisionQueryParams CollisionParams;

	OwnerCharacter = Cast<ACharacter>(GetOuterActor());
	
	if (false == OwnerCharacter.IsValid())
	{
		return;
	}
	
	CollisionParams.AddIgnoredActor(OwnerCharacter.Get());

	bool bRes = World->OverlapMultiByChannel(
		OverlapResult,
		GetOuterLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		FCollisionShape::MakeSphere(SearchRange),
		CollisionParams
	);
	
	// UI를 띄운다. => 이지만, 지금은 DebugDraw를 그린다.
	DrawDebugSphere(
		World,
		GetOuterLocation(),
		SearchRange,
		10.f,
		FColor::Green,
		false,
		10.f
	);
	
	if (bRes)
	{
		for (int32 Index = 0; Index < OverlapResult.Num(); Index++)
		{
			IRInteractable* Interactable = Cast<IRInteractable>(OverlapResult[Index].GetActor());
			
			// 거리 안에 들어옴.
			if (nullptr != Interactable)
			{
				float HalfRange = SearchFOV / 2.f;
				
				// Degree -> Radian 으로 변경
				float SearchRadianRange = FMath::Cos(FMath::DegreesToRadians(HalfRange));
				
				// 두 벡터
				FVector DiffVector = OverlapResult[Index].GetActor()->GetActorLocation() - OwnerCharacter->GetActorLocation();
				
				// 정규화
				DiffVector.Normalize();
				
				float DotResult = FVector::DotProduct(OwnerCharacter->GetActorForwardVector(), DiffVector);
				if (SearchRadianRange <= DotResult)
				{
					// 시야각 범위 내에 들어옴
					// 인터랙션 실행
					// 임시 코드 인터랙션 가능함을 확인하기 위해 사용
					Interactable->Interact(OwnerCharacter.Get());
					break;
				}
			}
		}
	}
}
