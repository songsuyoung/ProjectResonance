#pragma once

#include "CoreMinimal.h"
#include "RBaseDataTable.h"
#include "RLocationPreferenceTable.generated.h"

USTRUCT(BlueprintType)
struct FRPreferenceContainer
{
	GENERATED_BODY()
public:
	
	// Volume마다 ID가 존재함.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Preference|Settings")
	TMap<FName, float> Preferences;
};
USTRUCT(BlueprintType)
struct RESONANCE_API FRLocationPreferenceTable : public FTableRowBase
{
	GENERATED_BODY()

public:

	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems) override;
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;

	const FName GetID() { return ID; }
	
	// Location 선호도 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Preference|Settings")
	TMap<EREmotionState, FRPreferenceContainer> Preferences;
	
private:
	UPROPERTY(Transient)
	FName ID;
};
