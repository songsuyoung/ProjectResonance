#include "System/RDataManager.h"

// UE
#include "Engine/AssetManager.h"

// Newly Created Files...
#include "System/RGameInstance.h"
#include "Data/RDataAsset.h"

URDataManager* URDataManager::Get(UObject* Object)
{ 
    // UObject는 World 를 가지고 있지 않다.
    // 그렇기때문에 외부 액터로부터 World를 가져와야한다.
    UWorld* World = Object->GetWorld();

    check(World);

    URGameInstance* GameInstance = Cast<URGameInstance>(World->GetGameInstance());

    check(GameInstance);

    return GameInstance->GetDataManager();
}

void URDataManager::Initialize()
{
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

    Streamable.RequestAsyncLoad(DataAssetClass.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &ThisClass::LoadDataAsset));
}

void URDataManager::LoadDataAsset()
{
    DataAsset = DataAssetClass.Get();
}

TArray<UDataTable*> URDataManager::GetAllDataTable()
{
    TArray<UDataTable*> Results;

    for (ERDataTableType Type : TEnumRange<ERDataTableType>())
    {
        UDataTable* DataTable  = GetDataTable(Type);

        if (IsValid(DataTable))
        {
            Results.Add(DataTable);
        }
    }
    return Results;
}

UDataTable* URDataManager::GetDataTable(const ERDataTableType& DataType)
{
    if (false == IsValid(DataAsset))
    {
        return nullptr;
    }

    return DataAsset->GetTable(DataType);
}

UDataTable* URDataManager::GetDataTable(const FName& DataType)
{
    return nullptr;
}