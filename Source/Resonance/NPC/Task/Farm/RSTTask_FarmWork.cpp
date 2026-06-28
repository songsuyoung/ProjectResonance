#include "NPC/Task/Farm/RSTTask_FarmWork.h"

#include "Actor/RFarmPlotVolume.h"
#include "AIController.h"
#include "Character/RNPCCharacter.h"
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

		// 한 구간 완료 시 상하좌우 중 하나로 이동
		UE_LOG(LogTemp, Warning, TEXT("[FarmWork] ElapsedTime=%.2f / Threshold=%.2f / MoveStepSize=%.1f"), ElapsedTime, Duration * ActiveCount, MoveStepSize);
		if (ElapsedTime >= Duration * ActiveCount)
		{
			UE_LOG(LogTemp, Warning, TEXT("[FarmWork] → NoMove=true 진입"));
			OwnerCharacter->StopAnimMontage(WorkMontage);
			ActiveCount++;
			NoMove = true;

			const int32 Dir = FMath::RandRange(0, 1);
			const FVector Directions[] = {
				FVector( MoveStepSize, 0.f, 0.f),  // 앞
				FVector(-MoveStepSize, 0.f, 0.f),  // 뒤
			};
			const FVector RawTarget = OwnerCharacter->GetActorLocation() + Directions[Dir];
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
	
	OwnerCharacter->StopAnimMontage(WorkMontage);
}
