#include "Components/RClimbComponent.h"

// UE 5.
#include "VectorTypes.h"
#include "GameFramework/Character.h"
#include "Math/UnitConversion.h"

URClimbComponent::URClimbComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bIsClimbing(false)
	, OwnerPawn(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URClimbComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPawn = Cast<ACharacter>(GetOwner());
	
}

void URClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool URClimbComponent::RequestClimb()
{
	bool bCanClimb = CanClimb();
	
	if (bCanClimb)
	{
		StartClimb();
	}
	
	return bCanClimb;
}

void URClimbComponent::StartClimb()
{
	bIsClimbing = true;
}

bool URClimbComponent::CanClimb()
{
	if (false == OwnerPawn.IsValid())
	{
		return false;
	}
	
	// 등반 코드 프로토타입
	// head  Offset 으로 
	// upperarm_l upperarm_r 의 간의 길이를 Extent로 박스 생상.
	const FVector& HeadLocation = OwnerPawn->GetMesh()->GetSocketLocation(TEXT("head"));
	const FVector& StartLocation = HeadLocation + OwnerPawn->GetActorForwardVector() * 30.f;
	
	FVector Left = OwnerPawn->GetMesh()->GetSocketLocation(TEXT("upperarm_l")); //left 
	FVector Right = OwnerPawn->GetMesh()->GetSocketLocation(TEXT("upperarm_r")); //left 
	
	float Dist = FVector::Dist(Left, Right); // 어깨 길이를 알아냄.
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerPawn.Get());
	
	// 다른 액터가 있으면 
	TSet<int32> HitResult;
	
	bool bResult = ComputeClimbVerticalBounds(HitResult, QueryParams, StartLocation, Dist);
	bool bResult2 = ComputeClimbSideBounds(HitResult, QueryParams, StartLocation, Dist);
	
	return bResult && bResult2;
}

bool URClimbComponent::ComputeClimbVerticalBounds(TSet<int32>& HitResults, const FCollisionQueryParams& QueryParams,const FVector& StartLocation, float Distance)
{
	UWorld* World = GetWorld();
	check(World);
	
	// Identity를 사용하면 안되는 이유
	// 여러 방향에서 등반을 하기위해 시도하는데 Identity는 한방향에 대해서만 체크함.
	// 캐릭터가 바라보는 축으로 회전이 이루어져야함. 
	// 이유 : 캐릭터가 바라보는 방향에 있는 벽에 대해서 검사 예정이기 때문에.
	FQuat Quat = OwnerPawn->GetActorQuat();
	// 앞 뒤 X
	// 좌 우  Y -> 어깨 길이보다 크게 체크해야한다.
	// 위 아래 Z
	FVector Extent = FVector(30.f, Distance, 30.f);
	FCollisionShape Shape = FCollisionShape::MakeBox(Extent);
	FHitResult HitResult;
	
	bool bResult = World->SweepSingleByChannel
	(
		HitResult,
		StartLocation,
		StartLocation,
		Quat,
		ECollisionChannel::ECC_Visibility,
		Shape,
		QueryParams
		);

	if (false == bResult || nullptr == HitResult.GetActor())
	{
		return false;
	}
	
	HitResults.Add(HitResult.GetActor()->GetUniqueID());
	
	DrawDebugBox(World, StartLocation, Extent, Quat, FColor::Green, false, 5.f);
	
	// 바닥으로 내려야 한다.
	// - 붙임으로써 Down 벡터로 만들고, 
	// 절반만큼 내리면 몸퉁까지밖에 안오기때문에 2배만큼 내려준다.
	FVector DownLocation = StartLocation + -OwnerPawn->GetActorUpVector() * OwnerPawn->GetDefaultHalfHeight() * 1;
	
	DrawDebugBox(World, DownLocation, Extent, Quat, FColor::Yellow, false, 5.f);
	
	bResult = World->SweepSingleByChannel
	(
	HitResult,
	DownLocation,
	DownLocation,
	Quat,
	ECollisionChannel::ECC_Visibility,
	Shape,
	QueryParams
	);
	
	if (false == bResult || nullptr == HitResult.GetActor())
	{
		return false;
	}
	
	return HitResults.Contains(HitResult.GetActor()->GetUniqueID());
}

bool URClimbComponent::ComputeClimbSideBounds(TSet<int32>& HitResults, const FCollisionQueryParams& QueryParams,
	const FVector& StartLocation, float Distance)
{
	UWorld* World = GetWorld();
	check(World);
	
	const float HalfCapsule = OwnerPawn->GetDefaultHalfHeight() * 0.9f;
	
	// left 니까, RightVector 음수화
	const FVector& LeftLocation = StartLocation +
		(-OwnerPawn->GetActorRightVector() * Distance)
	+ (-OwnerPawn->GetActorUpVector() * HalfCapsule);
	
	// 왼쪽을 기준으로 먼저 계산을 진행한다.
	// head - foot_l 포함해야 한다.
	// StartLocation 에서 <- 우측으로 Distance만큼 이동한다.
	// OwnerPawn->GetActorUp 에 음수를 곱해서 아래로 내리는데, 거기에 HalfCapsule값을 곱한다.
	// 그리고 z축으로 HalfCapsule 만큼, 위와 일정하게 10, 30 으로 한다.
	FVector Extent = FVector(10.f, HalfCapsule, 30.f);
	FCollisionShape Shape = FCollisionShape::MakeBox(Extent);
	// Quat은 ActorLocation값 -> 우측/좌측인가에 따라 Z축으로 회전을 수행해야한다.
	// Y축해서 회전을 해야한다.
	FQuat Quat = FRotationMatrix::MakeFromZ(OwnerPawn->GetActorRotation().Vector()).ToQuat();
	FHitResult HitResult;
	
	DrawDebugBox(
		World,
		LeftLocation, 
		Extent,
		Quat,
		FColor::Red, 
		false, 
		5.f);

	bool bResult = World->SweepSingleByChannel
	(
		HitResult,
		LeftLocation,
		LeftLocation,
		Quat,
		ECollisionChannel::ECC_Visibility,
		Shape,
		QueryParams
	);
	
	if (false == bResult || nullptr == HitResult.GetActor())
	{
		// 실패
		return false;
	}
	
	if (false == HitResults.Contains(HitResult.GetActor()->GetUniqueID()))
	{
		//실패 온전히 같은 액터가 존재해야한다.
		return false;
	}
	
	const FVector& RightLocation = LeftLocation + OwnerPawn->GetActorRightVector() * Distance * 2.f;
		
	DrawDebugBox(
		World,
		RightLocation, 
		Extent,
		Quat,
		FColor::Red, 
		false, 
		5.f);

	bResult = World->SweepSingleByChannel
	(
		HitResult,
		RightLocation,
		RightLocation,
		Quat,
		ECollisionChannel::ECC_Visibility,
		Shape,
		QueryParams
	);
	if (false == bResult || nullptr == HitResult.GetActor())
	{
		// 실패
		return false;
	}
	
	return HitResults.Contains(HitResult.GetActor()->GetUniqueID());
}
