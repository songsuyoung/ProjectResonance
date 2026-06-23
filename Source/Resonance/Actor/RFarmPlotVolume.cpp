#include "Actor/RFarmPlotVolume.h"

void ARFarmPlotVolume::BeginPlay()
{
	Super::BeginPlay();
	
	InitializePlotData();
}

bool ARFarmPlotVolume::GetActivePlot(FRPlotData& OutPlotData)
{
	if (PlotData.IsValidIndex(ActivePlotIndex))
	{
		OutPlotData = PlotData[ActivePlotIndex];
		return true;
	}
	
	return false;
}

void ARFarmPlotVolume::InitializePlotData()
{
	FVector Origin, BoxExtent;
	GetActorBounds(false, Origin, BoxExtent);

	FVector RightLower = FVector(Origin.X - BoxExtent.X, Origin.Y + BoxExtent.Y, Origin.Z);
	
	// BoxExtent 는 절반임
	float Width  = (BoxExtent.X * 2) / Coordinate.Column;
	float Height = (BoxExtent.Y * 2) / Coordinate.Row;
	
	UWorld* World = GetWorld();
	check(World);

	DrawDebugBox(World, Origin, BoxExtent, FColor::Green, true, -1.f, 0, 3.f);
	
	// 최상단 좌측 Location값을 구해서, Row * Column만큼 이동
	for (int Row = 0; Row < Coordinate.Row; Row++)
	{
		for (int Col = 0; Col < Coordinate.Column; Col++)
		{
			FVector Location = FVector(
				RightLower.X + Width * (Col + 0.5f),
				RightLower.Y - Height * (Row + 0.5f),
				RightLower.Z - BoxExtent.Z
			);
			DrawDebugBox(World, Location, FVector(10.f, 10.f, 10.f), FColor::Blue, true, -1.f, 0, 3.f);
			
			DrawDebugBox(World, Location, FVector(Width * 0.5f, Height * 0.5f, 10.f), FColor::Red, true, -1.f, 0, 3.f);
			PlotData.Add({PlotData.Num(), Location, ERequiredFarmTask::Till});
		}
	}
}

bool ARFarmPlotVolume::FindNearestPendingPlot(const FVector& Location, FRPlotData &OutPlotData)
{
	float MinDist = FLT_MAX;
	bool bFound = false;
	for (int32 Index = 0; Index < PlotData.Num(); Index++ )
	{
		if (ActiveFarmTask != PlotData[Index].RequiredFarmTask)
		{
			continue;
		}
		
		float Dist = FVector::DistSquared2D(PlotData[Index].Location, Location);
		
		if (Dist < MinDist)
		{
			MinDist = Dist;
			OutPlotData = PlotData[Index];
			ActivePlotIndex = Index;
			bFound = true;
		}
	}
	
	return bFound;
}

void ARFarmPlotVolume::CompleteCurrentPlot(ERequiredFarmTask InFarmTask)
{
	if (false == PlotData.IsValidIndex(ActivePlotIndex))
	{
		return;
	}
	
	ERequiredFarmTask NextTask = static_cast<ERequiredFarmTask>(static_cast<uint8>(InFarmTask) + 1);
	
	// TODO: Enum을 이용한 상태로 확장 가능성있게 구현해야 한다. 
	// 모든 PlotData의 bIsPlowed 가 true라면, 다음 단계로 이동할 수 있도록 순차실행이 가능하도록 해야함.
	PlotData[ActivePlotIndex].RequiredFarmTask = NextTask;
	RefreshActiveFarmTask();
}

bool ARFarmPlotVolume::RefreshActiveFarmTask()
{
	uint8 MinPriority = static_cast<uint8>(ERequiredFarmTask::Max);
	int RequiredTaskIndex = -1;
	for (int Index = 0; Index < PlotData.Num(); Index++)
	{
		uint8 TaskIndex = static_cast<uint8>(PlotData[Index].RequiredFarmTask);
		if (MinPriority > TaskIndex)
		{
			RequiredTaskIndex = Index;
			MinPriority = TaskIndex;
		}
	}
	
	if (RequiredTaskIndex == INDEX_NONE)
	{
		return false;
	}
	
	ActiveFarmTask = static_cast<ERequiredFarmTask>(MinPriority);
	return true;
}
