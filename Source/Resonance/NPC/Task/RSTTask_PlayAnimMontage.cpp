#include "RSTTask_PlayAnimMontage.h"

#include "MotionWarpingComponent.h"
#include "SmartObjectSubsystem.h"
#include "StateTreeExecutionContext.h"
#include "Character/RNPCCharacter.h"

URSTTask_PlayAnimMontage::URSTTask_PlayAnimMontage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bUseMotionWarping(false)
{
}

EStateTreeRunStatus URSTTask_PlayAnimMontage::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	if (false == OwnerCharacter.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	if (false == SmartObjectActor.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	if (bUseMotionWarping)
	{
		UWorld* World = Context.GetWorld();
		check(World);

		USmartObjectSubsystem* SmartObjectSubsystem = World->GetSubsystem<USmartObjectSubsystem>();
		if (false == IsValid(SmartObjectSubsystem))
		{
			return EStateTreeRunStatus::Failed;
		}

		TOptional<FTransform> SlotTransform = SmartObjectSubsystem->GetSlotTransform(ClaimHandle);

		UMotionWarpingComponent* MotionWarpingComponent = OwnerCharacter->GetMotionWarpingComponent();

		if (false == IsValid(MotionWarpingComponent))
		{
			return EStateTreeRunStatus::Failed;
		}

		MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(MotionWarpingName, SlotTransform.GetValue());
	}
	USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();
	
	if (false == IsValid(MeshComponent))
	{
		return EStateTreeRunStatus::Failed;
	}
	
	AnimInstance = MeshComponent->GetAnimInstance();
	
	if (false == AnimInstance.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	AnimInstance->OnMontageBlendingOut.AddDynamic(this, &ThisClass::OnAnimMontageBlendOut);
		
	OwnerCharacter->PlayAnimMontage(AnimationMontage);
	
	if (bUseLookAt)
	{
		OwnerCharacter->SetLookAt(true);
	}
		
	if (bWaitForAnimationToFinish)
	{
		return EStateTreeRunStatus::Running;	
	}
	
	return EStateTreeRunStatus::Succeeded;
}

void URSTTask_PlayAnimMontage::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
	
	USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();
	
	if (false == IsValid(MeshComponent))
	{
		return;
	}
	
	AnimInstance = MeshComponent->GetAnimInstance();
	
	if (AnimInstance.IsValid())
	{
		// 델리게이트 객체 생성 및 함수 바인딩
		AnimInstance->OnMontageBlendingOut.RemoveAll(this);
	}
}

void URSTTask_PlayAnimMontage::OnAnimMontageBlendOut(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (bUseLookAt)
	{
		OwnerCharacter->SetLookAt(false);
	}

	if (AnimMontage == AnimationMontage)
	{
		FinishTask(true);
		return;
	}
	
	FinishTask(false);
}
