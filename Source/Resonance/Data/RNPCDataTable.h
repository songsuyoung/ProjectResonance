#pragma once

#include "CoreMinimal.h"
#include "RNPCDataTable.generated.h"

USTRUCT(BlueprintType)
struct RESONANCE_API FRNPCDataTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems) override;
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;

	const FName GetID() const { return ID; }
public:
	
	// TODO: NPC마다 이름, 외형 등도 여기에 있을 예정
	// NPC들 마다 첫 시작 스탯도 정의될 예정
	UPROPERTY(EditDefaultsOnly, Category = "NPCBase")
	float SpawnTime; // StartTime + SpawnTime 이후에 스폰될 예정

	UPROPERTY(EditDefaultsOnly, Category = "NPCBase")
	FName RegionID;
	
private:
	UPROPERTY(Transient)
	FName ID;
};
