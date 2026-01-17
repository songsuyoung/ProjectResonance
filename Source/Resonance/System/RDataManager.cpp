#include "System/RDataManager.h"

// UE
#include "Engine/AssetManager.h"

// Newly Created Files...
#include "System/RGameInstance.h"

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

void URDataManager::PostLoad()
{
    Super::PostLoad();

    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

    Streamable.RequestAsyncLoad(DataAssetClass.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &ThisClass::LoadDataAsset));
}

void URDataManager::LoadDataAsset()
{
    DataAsset = DataAssetClass.Get();
}

TArray<UDataTable*> URDataManager::GetAllDataTable()
{
    return TArray<UDataTable*>();
}

UDataTable* URDataManager::GetDataTable(const ERDataTableType& DataType)
{
    return nullptr;
}

UDataTable* URDataManager::GetDataTable(const FName& DataType)
{
    return nullptr;
}

FTableRowBase* URDataManager::GetDataTableRow(const ERDataTableType& DataType, int32 ID)
{
    return nullptr;
}

FTableRowBase* URDataManager::GetDataTableRow(const FName& DataType, int32 ID)
{
    return nullptr;
}
