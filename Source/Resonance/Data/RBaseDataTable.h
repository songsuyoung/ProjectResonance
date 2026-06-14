#pragma once

#include "CoreMinimal.h"
#include "Data/ResonanceStructs.h"
#include "RBaseDataTable.generated.h"

USTRUCT(BlueprintType)
struct RESONANCE_API FRBaseDataTable : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems) override;
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;

	const FName GetID() const { return ID; }
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Base")
	TArray<FRStatInfo> StatInfos;

private:
	UPROPERTY(Transient)
	FName ID;
};
