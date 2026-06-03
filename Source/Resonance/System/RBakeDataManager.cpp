#include "System/RBakeDataManager.h"

#include "JsonObjectConverter.h"
#include "RGameInstance.h"
#include "Data/RCoreStructs.h"
#include "System/RFileHelper.h"

URBakeDataManager* URBakeDataManager::Get(UObject* Object)
{
    UWorld* World = Object->GetWorld();

    check(World);

    URGameInstance* GameInstance = Cast<URGameInstance>(World->GetGameInstance());

    check(GameInstance);

    return GameInstance->GetBakeDataManager();
}

bool URBakeDataManager::GetRandomTransformData(ERBakeType BakeType, FRTransformData& OutData) const
{
    if (false == BakeDataGroup.BakeDataMap.Contains(BakeType))
    {
        return false;
    }
    
    int32 RandomIndex = FMath::RandRange(0, BakeDataGroup.BakeDataMap[BakeType].TransformData.Num() - 1);
    
    OutData = BakeDataGroup.BakeDataMap[BakeType].TransformData[RandomIndex];
    
    return true;
}

bool URBakeDataManager::GetTransformData(ERBakeType BakeType, TArray<FRTransformData>& OutData) const
{
    if (false == BakeDataGroup.BakeDataMap.Contains(BakeType))
    {
        return false;
    }
    
    OutData = BakeDataGroup.BakeDataMap[BakeType].TransformData;
    
    return true;
}

bool URBakeDataManager::GetLocationData(ERBakeType BakeType, TArray<FVector>& OutData) const
{
    TArray<FRTransformData> TempTransformData;
    if (false == GetTransformData(BakeType, TempTransformData))
    {
        return false;
    }
    
    for (int32 Index = 0; Index < TempTransformData.Num(); Index++)
    {
        OutData.Add(TempTransformData[Index].Tansform.GetLocation());
    }
    
    return true;
}

void URBakeDataManager::Initialize()
{
    FString OutResult;
    
    FRFileHelper::ReadStringFromJson(TEXT("/BakeData.json"), OutResult);
    FJsonObjectConverter::JsonObjectStringToUStruct(OutResult, &BakeDataGroup);
}
