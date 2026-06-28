#include "RSTTask_EnterRegion.h"

// UE 5.
#include "Components/ArrowComponent.h"

//
#include "Actor/RRegionVolume.h"
#include "Character/RNPCCharacter.h"
#include "System/RRegionManager.h"

URSTTask_EnterRegion::URSTTask_EnterRegion(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , EnterRegion(EREnterRegionStage::TurnInPlace)
    , SlowDownRadius(200.f)
    , LimitedDistanceScale(0.7f)
{
}

EStateTreeRunStatus URSTTask_EnterRegion::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    // 현재 도달한 다음 구역을 가져온다.
    NextRegionID = OwnerCharacter->PeekNextRegion();
    
    if (NextRegionID.IsNone())
    {
        return EStateTreeRunStatus::Failed;
    }
    
    URRegionManager* RegionManager = URRegionManager::Get(this);
    check(RegionManager);
    
    // 그 구역의 RegionVolume을 가져오고,
    TWeakObjectPtr<ARRegionVolume> RegionVolume = RegionManager->GetRegionVolume(NextRegionID);
    
    if (false == RegionVolume.IsValid())
    {
        return EStateTreeRunStatus::Failed;
    }
    
    UArrowComponent* LookAtArrowComponent = RegionVolume->GetLookAtArrowComponent();
    
    if (false == IsValid(LookAtArrowComponent))
    {
        return EStateTreeRunStatus::Failed;
    }
    
    FVector ForwardVector = LookAtArrowComponent->GetForwardVector();
    TargetRotation = ForwardVector.Rotation();
    TargetRotation.Roll = TargetRotation.Pitch = 0.f;
    
    FVector Extent, Origin;
    RegionVolume->GetActorBounds(false, Origin, Extent);
    TargetLocation = RegionVolume->GetActorLocation() + ForwardVector * Extent.X;
    LimitedDistance = Extent.X * LimitedDistanceScale; //70.f 길이만큼 오면 인정.
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
        MoveTo(DeltaTime);
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

    FRotator NewRot = FMath::RInterpTo(
         OwnerCharacter->GetActorRotation(),
         TargetRotation,
         DeltaTime,
         10.f
     );
    
    OwnerCharacter->SetActorRotation(NewRot);
}

void URSTTask_EnterRegion::MoveTo(float DeltaTime)
{
    FVector CurrentLocation = OwnerCharacter->GetActorLocation();
    float Distance = FVector::Dist2D(CurrentLocation, TargetLocation);

    if (Distance <= LimitedDistance)
    {
        EnterRegion = EREnterRegionStage::Hidden;
        return;
    }

    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
    float ScaleValue = FMath::Clamp(Distance / SlowDownRadius, 0.1f, 0.5f);
    OwnerCharacter->AddMovementInput(Direction, ScaleValue);
}

void URSTTask_EnterRegion::Enter()
{
    OwnerCharacter->VisitRegion(NextRegionID, bHide);
}