#include "RSTTask_PlayAnimMontage.h"

#include "MotionWarpingComponent.h"
#include "SmartObjectSubsystem.h"
#include "StateTreeExecutionContext.h"
#include "Character/RNPCCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	if (bUseMotionWarping)
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(MotionWarpingName,SlotTransform.GetValue());
	}
	USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();
	
	if (false == IsValid(MeshComponent))
	{
		return EStateTreeRunStatus::Failed;
	}
	
	UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance();
	
	AnimInstance->OnMontageBlendingOut.AddDynamic(this, &ThisClass::OnAnimMontageEnded);
		
	OwnerCharacter->PlayAnimMontage(AnimationMontage);
	
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
	
	UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance();
	
	if (IsValid(AnimInstance))
	{
		// 델리게이트 객체 생성 및 함수 바인딩
		AnimInstance->OnMontageEnded.RemoveAll(this);
	}
}

void URSTTask_PlayAnimMontage::OnAnimMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (AnimMontage == AnimationMontage)
	{
		FinishTask(true);
		return;
	}
	
	FinishTask(false);
}
