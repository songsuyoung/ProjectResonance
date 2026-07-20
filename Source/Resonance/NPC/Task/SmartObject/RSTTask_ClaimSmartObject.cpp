#include "NPC/Task/SmartObject/RSTTask_ClaimSmartObject.h"
#include "SmartObjectSubsystem.h"
#include "StateTreeExecutionContext.h"
#include "Character/RNPCCharacter.h"

EStateTreeRunStatus URSTTask_ClaimSmartObject::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	UWorld* World = Context.GetWorld();
	check(World);
	
	USmartObjectSubsystem* SmartObjectSubsystem = World->GetSubsystem<USmartObjectSubsystem>();
	
	FSmartObjectRequest Request;                                       
	const FVector NPCLocation = OwnerCharacter->GetActorLocation();
	TArray<FSmartObjectRequestResult> OutResults;
	
	if (false == SmartObjectSubsystem->FindSmartObjects(Request, OutResults))
	{
		return EStateTreeRunStatus::Failed;
	}
	
	if (OutResults.IsEmpty())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	OutResults.Sort([&](const FSmartObjectRequestResult& A, const FSmartObjectRequestResult& B)
	{
		const float DistA = FVector::DistSquared(SmartObjectSubsystem->GetSlotLocation(A.SlotHandle).GetValue(), NPCLocation);
		const float DistB = FVector::DistSquared(SmartObjectSubsystem->GetSlotLocation(B.SlotHandle).GetValue(), NPCLocation);
		return DistA < DistB;
	});

	if (false == SmartObjectSubsystem->CanBeClaimed(OutResults[0].SlotHandle))
	{
		return EStateTreeRunStatus::Failed;
	}
	
	ClaimHandle = SmartObjectSubsystem->MarkSlotAsClaimed(OutResults[0].SlotHandle, ESmartObjectClaimPriority::Normal);
	
	if (false == ClaimHandle.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	return EStateTreeRunStatus::Succeeded;
}
