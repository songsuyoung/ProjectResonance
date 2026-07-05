#include "NPC/Task/SmartObject/RSTTask_ApproachAndAlignToSlot.h"

#include "AIController.h"
#include "SmartObjectComponent.h"
#include "SmartObjectSubsystem.h"
#include "SmartObjectDefinition.h"
#include "StateTreeExecutionContext.h"
#include "Character/RNPCCharacter.h"

EStateTreeRunStatus URSTTask_ApproachAndAlignToSlot::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	UWorld* World = Context.GetWorld();
	check(World);
	
	SmartObjectSubsystem = World->GetSubsystem<USmartObjectSubsystem>();
	if (false == SmartObjectSubsystem.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	TOptional<FTransform> SlotTransform = SmartObjectSubsystem->GetSlotTransform(ClaimHandle);
	if (false == SlotTransform.IsSet())
	{                                                                                                   
		return EStateTreeRunStatus::Failed;                                                                                                                                                                                                   
	}
	TargetLocation = SlotTransform->GetLocation();
	TargetRotator = SlotTransform->GetRotation().GetForwardVector().Rotation();
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus URSTTask_ApproachAndAlignToSlot::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	if (false == OwnerCharacter.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}

	FVector CurrentLocation = OwnerCharacter->GetActorLocation();
	FVector Direction = TargetLocation - CurrentLocation;
	float Distance = Direction.Size2D();

	if (Distance > 80.f)
	{
		// 이동
		OwnerCharacter->AddMovementInput(Direction.GetSafeNormal2D());
		return EStateTreeRunStatus::Running;
	}

	// 도착 후 회전
	FRotator CurrentRotator = OwnerCharacter->GetActorRotation();
	FRotator NewRotator = FMath::RInterpTo(CurrentRotator, TargetRotator, DeltaTime, 5.f);
	OwnerCharacter->SetActorRotation(NewRotator);

	if (TargetRotator.Equals(NewRotator, 1.f))
	{
		return EStateTreeRunStatus::Succeeded;
	}

	return EStateTreeRunStatus::Running;
}
