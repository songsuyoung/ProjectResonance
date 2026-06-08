#pragma once

#include "CoreMinimal.h"
#include "RRegionDataTable.generated.h"

USTRUCT(BlueprintType)
struct RESONANCE_API FRRegionDataTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems) override;
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
	
	/*// 장소 이름
	UPROPERTY(EditAnywhere, Category = "")
	FString RegionName;
	
	// 장소 위치 (스폰/목표 지점 위치)
	UPROPERTY(EditAnywhere, Cetegory = "")
	FVector Location;*/
	
private:
	
	UPROPERTY(Transient)
	int32 ID;
};
