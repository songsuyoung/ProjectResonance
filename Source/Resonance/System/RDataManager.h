#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/ResonanceEnums.h"
#include "RDataManager.generated.h"

class URDataAsset;

UCLASS(Blueprintable)
class RESONANCE_API URDataManager : public UObject
{
	GENERATED_BODY()

public:

	static URDataManager* Get(UObject* Object);

	void Initialize();
public:
	// �˻� �뵵
	TArray<UDataTable*> GetAllDataTable();

	UDataTable* GetDataTable(const ERDataTableType& DataType);
	UDataTable* GetDataTable(const FName& DataType);


	// FTableRowBase�� cast���� �ʵ���, ���ø� �Լ� ���� ����
	template<typename T>
	T* GetDataTableRow(const ERDataTableType& DataType, const FName& ID);

	template<typename T>
	T* GetDataTableRow(const ERDataTableType& DataType, int32 ID);

protected:

	void LoadDataAsset();

protected:

	// �������� ����-> �񵿱�� ȣ���ϸ�, GC�� ���� ȸ��� �� �ִ�.
	UPROPERTY(EditAnywhere, Category = "DataTables")
	TSoftObjectPtr<URDataAsset> DataAssetClass;

protected:

	UPROPERTY(Transient)
	TObjectPtr<URDataAsset> DataAsset;
};

template<typename T>
T* URDataManager::GetDataTableRow(const ERDataTableType& DataType, const FName& ID)
{
	UDataTable* DataTable = GetDataTable(DataType);

	if (IsValid(DataTable))
	{
		return DataTable->FindRow<T>(ID, FString());
	}

	return nullptr;
}

template<typename T>
T* URDataManager::GetDataTableRow(const ERDataTableType& DataType, int32 ID)
{
	FString StrID = FString::Printf(TEXT("%d"), ID);

	const FName& IDName = FName(StrID);

	return GetDataTableRow(DataType, IDName);
}
