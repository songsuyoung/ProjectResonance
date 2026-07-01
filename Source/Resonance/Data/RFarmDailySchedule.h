#pragma once

#include "CoreMinimal.h"
#include "RBaseDataTable.h"
#include "RFarmDailySchedule.generated.h"

enum class ERequiredFarmTask : uint8;

USTRUCT(BlueprintType)
struct RESONANCE_API FRFarmDailySchedule : public FTableRowBase
{
	GENERATED_BODY()

public:

	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems) override;
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;

	const FName GetID() { return ID; }

	
	UPROPERTY(EditDefaultsOnly, Category = "Farm|Settings")
	TArray<ERequiredFarmTask> Tasks;
	
private:
	UPROPERTY(Transient)
	FName ID;
};
