#include "RInteraction.h"

#include "DrawDebugHelpers.h"    

#include "Character/ResonanceCharacter.h"
#include "Engine/OverlapResult.h"
#include "Interface/RInteractable.h"

void URInteraction::Initialize(AActor* InOwner)
{
	OwnerCharacter = Cast<ACharacter>(InOwner);
	
	float HalfRange = SearchFOV / 2.f;
	// Degree -> Radian 으로 변경
	SearchRadianRange = FMath::Cos(FMath::DegreesToRadians(HalfRange));
}

void URInteraction::Execute()
{
	UWorld* World = GetWorld();

	check(World);

	TArray<FOverlapResult> OverlapResult;
	FCollisionQueryParams CollisionParams;

	if (false == OwnerCharacter.IsValid())
	{
		return;
	}
	
	CollisionParams.AddIgnoredActor(OwnerCharacter.Get());

	FVector CenterLocation = OwnerCharacter->GetActorLocation();
	
	bool bRes = World->OverlapMultiByChannel(
		OverlapResult,
		CenterLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		FCollisionShape::MakeSphere(SearchRange),
		CollisionParams
	);
	
	// UI를 띄운다. => 이지만, 지금은 DebugDraw를 그린다.
	DrawDebugSphere(
		World,
		CenterLocation,
		SearchRange,
		10.f,
		FColor::Green,
		false,
		10.f
	);
	
	if (bRes)
	{
		OverlapResult.Sort([CenterLocation](const FOverlapResult& A, const FOverlapResult& B)
		{
			float ADist = FVector::DistSquared(A.GetActor()->GetActorLocation() ,CenterLocation);
			float BDist = FVector::DistSquared(B.GetActor()->GetActorLocation(), CenterLocation);
			
			return ADist < BDist;
		});
		
		for (int32 Index = 0; Index < OverlapResult.Num(); Index++)
		{
			IRInteractable* Interactable = Cast<IRInteractable>(OverlapResult[Index].GetActor());
			
			// 거리 안에 들어옴.
			if (nullptr != Interactable)
			{
				// 두 벡터
				FVector DiffVector = OverlapResult[Index].GetActor()->GetActorLocation() - CenterLocation;
				
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
