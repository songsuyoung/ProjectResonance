#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "RFarmPlotVolume.generated.h"

UENUM(BlueprintType)
enum class ERequiredFarmTask : uint8
{
	None UMETA(DisplayName = "생략 (None)"),
	Till UMETA(DisplayName = "호미질 (Till)"),
	Clear UMETA(DisplayName = "땅 정리 (Clear)"),
	Sow UMETA(DisplayName = "씨앗 심기 (Sow)"),
	Weed  UMETA(DisplayName = "잡초 제거 (Weed)"),    
	Water UMETA(DisplayName = "물주기 (Water)"),
	Harvest UMETA(DisplayName = "수확 (Harvest)"),
	Max
};

ENUM_RANGE_BY_COUNT(ERequiredFarmTask, ERequiredFarmTask::Max);

UENUM(BlueprintType)
enum class ERTaskStatus : uint8
{
	NotStarted,
	InProgress,
	Completed
};

USTRUCT(BlueprintType)
struct FRPlotCoord
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Plow|Settings")
	int32 Row;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Plow|Settings")
	int32 Column;
};

USTRUCT()
struct FRPlotData
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	int32 Index;
	
	UPROPERTY(Transient)
	FVector Location;        // Center
	
	UPROPERTY(Transient)
	ERequiredFarmTask RequiredFarmTask;
};
struct FRFarmDailySchedule;

UCLASS()
class RESONANCE_API ARFarmPlotVolume : public AVolume
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	bool GetActivePlot(FRPlotData& OutPlotData);
	void CompleteCurrentPlot(int32 InActivePlotIndex);
	const FVector& GetPlotExtent() { return Extent; }
	bool RefreshActiveFarmTask();
	
protected:
	void OnDayChanged(int32 NewDay);
	void InitializePlotData();
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Plow|Settings")
	FName OwnerNPCID;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Plow|Settings")
	FRPlotCoord Coordinate; 

protected:
		
	UPROPERTY(Transient)
	FVector Extent;
	
	UPROPERTY(Transient)
	TArray<FRPlotData> PlotData;
	
	UPROPERTY(Transient)
	ERequiredFarmTask ActiveFarmTask;
	
	UPROPERTY(Transient)
	int32 ActivePlotIndex;
	
	UPROPERTY(Transient)
	int32 CycleDays;
	
	FRFarmDailySchedule* FarmDailySchedule;
};
