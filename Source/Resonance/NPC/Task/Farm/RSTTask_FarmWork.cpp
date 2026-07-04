#include "NPC/Task/Farm/RSTTask_FarmWork.h"

#include "Actor/RFarmPlotVolume.h"
#include "AIController.h"
#include "Character/RNPCCharacter.h"
#include "Components/RJobComponent.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"

EStateTreeRunStatus URSTTask_FarmWork::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	if (false == OwnerCharacter.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}

	if (false == FarmVolume.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}

	if (false == IsValid(WorkMontage))
	{
		return EStateTreeRunStatus::Failed;
	}

	FRPlotData PlotData;
	// PendingTask인 나랑 가장 가까운 위치를 찾는다.
	if (false == FarmVolume->GetActivePlot(PlotData))
	{
		return EStateTreeRunStatus::Failed;
	}
	ActivePlotIndex = PlotData.Index;
	PlotCenter = PlotData.Location;

	int32 Count = FMath::RandRange(2, 5);
	WorkDuration = WorkMontage->GetPlayLength() * Count;
	Duration = WorkMontage->GetPlayLength();
	ElapsedTime = 0.f;
	ActiveCount = 1;
	NoMove = false;
	OwnerCharacter->PlayAnimMontage(WorkMontage);
	JobComponent = OwnerCharacter->GetJobComponent();
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus URSTTask_FarmWork::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	if (NoMove)
	{
		// 이동 중 - 도착 확인
		if (AIController->GetMoveStatus() == EPathFollowingStatus::Type::Idle)
		{
			NoMove = false;
			OwnerCharacter->PlayAnimMontage(WorkMontage);
		}
	}
	else
	{
		// 밭일 중에만 시간 누적
		ElapsedTime += DeltaTime;

		if (ElapsedTime >= WorkDuration)
		{
			FarmVolume->CompleteCurrentPlot(ActivePlotIndex);
			return EStateTreeRunStatus::Succeeded;
		}

		if (ElapsedTime >= Duration * ActiveCount)
		{
			OwnerCharacter->StopAnimMontage(WorkMontage);
			ActiveCount++;
			NoMove = true;

			const FVector Forward = OwnerCharacter->GetActorForwardVector();                                                                                                                                                                          
			const int32 DirIndex = FMath::RandRange(-1, 1);  // -1, 0, 1                                                                                                                                                                              
			const float AngleDeg = DirIndex * 45.f;	//0~45, -45 도 각도를 기준으로 회전 예정                                                                                                                                                                  
			const FVector Dir = Forward.RotateAngleAxis(AngleDeg, FVector::UpVector);                                                                                                                                                                 
			const FVector RawTarget = OwnerCharacter->GetActorLocation() + Dir * MoveStepSize;
			
			const FVector MaxLocation = PlotCenter + FarmVolume->GetPlotExtent();
			const FVector MinLocation = PlotCenter - FarmVolume->GetPlotExtent();
			// 밭 범위 안으로 Clamp
			const FVector TargetLocation = FVector(
				FMath::Clamp(RawTarget.X, MinLocation.X, MaxLocation.X),
				FMath::Clamp(RawTarget.Y, MinLocation.Y, MaxLocation.Y),
				RawTarget.Z
			);

			AIController->MoveToLocation(TargetLocation, 10.f);
		}
	}

	return EStateTreeRunStatus::Running;
}

void URSTTask_FarmWork::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
	
	if (false == JobComponent.IsValid())
	{
		return;
	}
	
	// 일할 시간이 끝났기에, 현재 활성화됐던 Index 완료됐음을 알리고, Montage 를 강제로 멈춤.
	if (false == JobComponent->IsWorkTime())
	{
		FarmVolume->CompleteCurrentPlot(ActivePlotIndex);
		OwnerCharacter->StopAnimMontage(WorkMontage);
	}
}
