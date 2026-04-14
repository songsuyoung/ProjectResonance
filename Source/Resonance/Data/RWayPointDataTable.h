#pragma once

#include "CoreMinimal.h"
#include "RWayPointDataTable.generated.h"

USTRUCT(BlueprintType)
struct RESONANCE_API FRWayPointDataTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	
	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems) override;
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
	

	// 절대 위치값
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Location")
	FVector Location;
	
protected:
	
	UPROPERTY(Transient)
	int32 ID;
};
