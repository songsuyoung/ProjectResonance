#include "RSTTask_EnterRegion.h"

#include "Actor/RRegionVolume.h"
#include "Character/RNPCCharacter.h"
#include "Components/SplineComponent.h"
#include "System/RRegionManager.h"

URSTTask_EnterRegion::URSTTask_EnterRegion(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , EnterRegion(EREnterRegionStage::TurnInPlace)
{
}

EStateTreeRunStatus URSTTask_EnterRegion::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    FName CurrentRegionID = OwnerCharacter->GetCurrentVisitedRegion();
    
    URRegionManager* RegionManager = URRegionManager::Get(this);
    check(RegionManager);
    
    TWeakObjectPtr<ARRegionVolume> RegionVolume = RegionManager->GetRegionVolume(CurrentRegionID);
    
    if (false == RegionVolume.IsValid())
    {
        return EStateTreeRunStatus::Failed;
    }

    USplineComponent* SplineComponent = RegionVolume->GetSplineComponent();

    // Point 1 위치로 목적지 설정
    TargetLocation = SplineComponent->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::World);

    // 캐릭터 현재 위치 → Point 1 방향으로 회전 목표 설정
    FVector Dir = (TargetLocation - OwnerCharacter->GetActorLocation());
    Dir.Z = 0.f;
    TargetRotation = FRotator(0.f, Dir.Rotation().Yaw, 0.f);

    LimitedDistance = 50.f;
    EnterRegion = EREnterRegionStage::TurnInPlace;
    
    return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus URSTTask_EnterRegion::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    switch (EnterRegion)
    {
    case EREnterRegionStage::TurnInPlace:
        TurnInPlace(DeltaTime);
        break;
    case EREnterRegionStage::Walk:
        Walk(DeltaTime);
        break;
    case EREnterRegionStage::Hidden:
        Enter();
        return EStateTreeRunStatus::Succeeded;
    }
    
    return EStateTreeRunStatus::Running;
}

void URSTTask_EnterRegion::TurnInPlace(float DeltaTime)
{
    FVector CurrentForward = OwnerCharacter->GetActorForwardVector();
    FVector TargetForward = TargetRotation.Vector();

    float Dot = FVector::DotProduct(CurrentForward, TargetForward);
    if (Dot >= 0.996f)
    {
        OwnerCharacter->SetActorRotation(TargetRotation);
        EnterRegion = EREnterRegionStage::Walk;
        return;
    }

    OwnerCharacter->SetActorRotation(
        FMath::RInterpTo(OwnerCharacter->GetActorRotation(), TargetRotation, DeltaTime, 10.f)
    );
}

void URSTTask_EnterRegion::Walk(float DeltaTime)
{
    FVector CurrentLocation = OwnerCharacter->GetActorLocation();
    float Distance = FVector::Dist2D(CurrentLocation, TargetLocation);

    UE_LOG(LogTemp, Warning, TEXT("Distance: %.2f / LimitedDistance: %.2f"), Distance, LimitedDistance);

    if (Distance <= LimitedDistance)
    {
        EnterRegion = EREnterRegionStage::Hidden;
        return;
    }

    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
    float ScaleValue = FMath::Clamp(Distance / 200.f, 0.1f, 0.5f);
    OwnerCharacter->AddMovementInput(Direction, ScaleValue);
}

void URSTTask_EnterRegion::Enter()
{
    FName NextRegionID = OwnerCharacter->PeekNextRegion();
    OwnerCharacter->VisitRegion(NextRegionID);
}