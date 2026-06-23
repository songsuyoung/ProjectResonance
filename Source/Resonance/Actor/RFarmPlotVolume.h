#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "RFarmPlotVolume.generated.h"

UENUM(BlueprintType)
enum class ERequiredFarmTask : uint8
{
	Till       UMETA(DisplayName = "호미질 (Till)"),
	Clear      UMETA(DisplayName = "땅 정리 (Clear)"),
	Sow        UMETA(DisplayName = "씨앗 심기 (Sow)"),
	Water      UMETA(DisplayName = "물주기 (Water)"),
	Weed       UMETA(DisplayName = "잡초 뽑기 (Weed)"),
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

UCLASS()
class RESONANCE_API ARFarmPlotVolume : public AVolume
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;
	bool GetActivePlot(FRPlotData& OutPlotData);
	bool FindNearestPendingPlot(const FVector& Location, FRPlotData &PlotData);
	void CompleteCurrentPlot(ERequiredFarmTask FarmTask);
	bool RefreshActiveFarmTask();
	
protected:
	
	void InitializePlotData();
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Plow|Settings")
	FName OwnerNPCID;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Plow|Settings")
	FRPlotCoord Coordinate; 

protected:
	
	UPROPERTY(Transient)
	TArray<FRPlotData> PlotData;
	
	UPROPERTY(Transient)
	ERequiredFarmTask ActiveFarmTask;
	
	UPROPERTY(Transient)
	int32 ActivePlotIndex;
};
