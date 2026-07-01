#include "Actor/RFarmPlotVolume.h"

#include "System/RDataManager.h"
#include "System/RTimeManager.h"
#include "Data/RFarmDailySchedule.h"
#include "Data/ResonanceEnums.h"

void ARFarmPlotVolume::BeginPlay()
{
	Super::BeginPlay();
	
	InitializePlotData();
	
	URTimeManager* TimeManager = URTimeManager::Get(this);
	check(TimeManager);
	TimeManager->OnDayChanged.AddUObject(this, &ThisClass::OnDayChanged);
}

void ARFarmPlotVolume::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	URTimeManager* TimeManager = URTimeManager::Get(this);
	check(TimeManager);
	TimeManager->OnDayChanged.RemoveAll(this);
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
	Extent = FVector(Width * 0.5f, Height * 0.5f, 10.f);
	
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
			PlotData.Add({PlotData.Num(), Location, TArray<ERequiredFarmTask>()});
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
	if (CurrentPlotData.PendingTasks.Num() > 0)
	{
		CurrentPlotData.PendingTasks.RemoveAt(0);
	}
	
	ActivePlotIndex++;
}

void ARFarmPlotVolume::OnDayChanged(int32 NewDay)
{
	UE_LOG(LogTemp, Log, TEXT("[Farm] %d"), NewDay);
	
	URDataManager* DataManager = URDataManager::Get(this);
	check(DataManager);
	
	UDataTable* DataTable = DataManager->GetDataTable(ERDataTableType::FarmDailySchedule);
	
	int32 TargetDay = INT_MAX;
	FRFarmDailySchedule* FarmDailySchedule = nullptr;
	for (auto& RowName : DataTable->GetRowNames())
	{
		int32 RowDay = FCString::Atoi(*RowName.ToString().RightChop(4));
		if (RowDay >= CycleDays && RowDay < TargetDay)
		{
			TargetDay = RowDay;

			FarmDailySchedule = DataTable->FindRow<FRFarmDailySchedule>(RowName, TEXT(""));
		}
	}
	
	if (nullptr != FarmDailySchedule)
	{
		for (auto& Plot : PlotData)
		{
			Plot.PendingTasks.Append(FarmDailySchedule->Tasks);
		}	
		
		CycleDays++;
	}
}
