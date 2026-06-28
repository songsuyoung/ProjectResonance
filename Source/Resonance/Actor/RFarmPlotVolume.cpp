#include "Actor/RFarmPlotVolume.h"

void ARFarmPlotVolume::BeginPlay()
{
	Super::BeginPlay();
	
	InitializePlotData();
}

bool ARFarmPlotVolume::GetActivePlot(FRPlotData& OutPlotData)
{
	int32 Row = ActivePlotIndex / Coordinate.Column;
	int32 ColPos = ActivePlotIndex % Coordinate.Column;

	if (Row >= Coordinate.Row)
	{
		return false;
		// 모든 Plot 완료                                                                                                                                                                                                   
	}

	int32 PlotDataIndex = (Row % 2 == 0)
		                      ? Row * Coordinate.Column + ColPos
		                      : Row * Coordinate.Column + (Coordinate.Column - 1 - ColPos);

	OutPlotData = PlotData[PlotDataIndex];
	return true;
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

void ARFarmPlotVolume::CompleteCurrentPlot(int32 InActivePlotIndex)
{
	// Snake 방식으로, PlotDataIndex를 바꾸고 있기 때문에, 실제 인덱스를 알려면 다시 변환해주어야 한다.
	int32 Row = ActivePlotIndex / Coordinate.Column;                                                                                                                                                                                          
	int32 ColPos = ActivePlotIndex % Coordinate.Column;                                                                                                                                                                                       
	int32 PlotDataIndex = (Row % 2 == 0)                                                                                                                                                                                                      
		? Row * Coordinate.Column + ColPos                                                                                                                                                                                                    
		: Row * Coordinate.Column + (Coordinate.Column - 1 - ColPos);  
	
	// 같아야함.
	if (InActivePlotIndex != PlotDataIndex)
	{
		return;
	}
	
	FRPlotData& CurrentPlotData = PlotData[InActivePlotIndex];

	ERequiredFarmTask NextTask = static_cast<ERequiredFarmTask>(static_cast<uint8>(CurrentPlotData.RequiredFarmTask) + 1);
	
	// TODO: Enum을 이용한 상태로 확장 가능성있게 구현해야 한다. 
	// 모든 PlotData의 bIsPlowed 가 true라면, 다음 단계로 이동할 수 있도록 순차실행이 가능하도록 해야함.
	CurrentPlotData.RequiredFarmTask = NextTask;
	ActivePlotIndex++;
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
	
	/*if (RequiredTaskIndex == INDEX_NONE)
	{
		return false;
	}*/
	
	ERequiredFarmTask NewActiveFarmTask = static_cast<ERequiredFarmTask>(MinPriority);
	
	if (NewActiveFarmTask != ActiveFarmTask)
	{
		ActiveFarmTask = NewActiveFarmTask;
		ActivePlotIndex = 0; //초기 과정으로 돌아감. 항상 0부터 시작하도록 함.
		// 원래 자리로 돌아가야함.
	}
	
	return true;
}
