#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "RFarmPlotVolume.generated.h"

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
	FVector Location;        // Center

	UPROPERTY(Transient)
	bool bIsPlowed;

	UPROPERTY(Transient)
	bool bIsSown;

	UPROPERTY(Transient)
	bool bIsWatered;
};

UCLASS()
class RESONANCE_API ARFarmPlotVolume : public AVolume
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Plow|Settings")
	FName OwnerNPCID;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Plow|Settings")
	FRPlotCoord Coordinate; 

protected:
	
	UPROPERTY(Transient)
	TArray<FRPlotData> PlotData;
};
