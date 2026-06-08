#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RRegionDataTable.generated.h"

USTRUCT(BlueprintType)
struct RESONANCE_API FRRegionDataTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	
	int32 GetID() const { return RegionID; }
	
protected:
	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems) override;
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	int32 RegionID;
};
