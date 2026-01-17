#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/ResonanceEnums.h"
#include "RDataAsset.generated.h"

// URDataAsset 자체를 TSoftObjectPtr 로 사용하기 때문에
UCLASS()
class RESONANCE_API URDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UDataTable* GetTable(const ERDataTableType& DataType) const;
	
protected:

	UPROPERTY(EditAnywhere, Category = "Data Tables")
	TMap<ERDataTableType, TObjectPtr<UDataTable>> DataTableMap;
};
