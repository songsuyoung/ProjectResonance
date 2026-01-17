#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/ResonanceEnums.h"
#include "RDataManager.generated.h"

class UDataAsset;

UCLASS(Blueprintable)
class RESONANCE_API URDataManager : public UObject
{
	GENERATED_BODY()

public:

	static URDataManager* Get(UObject* Object);
	
public:
	// 검색 용도

	TArray<UDataTable*> GetAllDataTable();

	UDataTable* GetDataTable(const ERDataTableType& DataType);
	UDataTable* GetDataTable(const FName& DataType);

	FTableRowBase* GetDataTableRow(const ERDataTableType& DataType, int32 ID);
	FTableRowBase* GetDataTableRow(const FName& DataType, int32 ID);


protected:
	virtual void PostLoad() override;

	void LoadDataAsset();

protected:

	// 동적으로 생성-> 비동기로 호출하면, GC에 의해 회사될 수 있다.
	UPROPERTY(EditAnywhere, Category = "DataTables")
	TSoftObjectPtr<UDataAsset> DataAssetClass;

protected:

	UPROPERTY(Transient)
	TObjectPtr<UDataAsset> DataAsset;
};
