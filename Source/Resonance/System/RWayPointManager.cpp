#include "System/RWayPointManager.h"

#include "JsonObjectConverter.h"
#include "RGameInstance.h"
#include "Game/Interface/RWayPointSerializable.h"
#include "System/RFileHelper.h"

URWayPointManager* URWayPointManager::Get(UObject* Object)
{
    UWorld* World = Object->GetWorld();

    check(World);

    URGameInstance* GameInstance = Cast<URGameInstance>(World->GetGameInstance());

    check(GameInstance);

    return GameInstance->GetWayPointManager();
}

const FRWayPoint& URWayPointManager::GetRandomPoint() const
{
    int32 RandomIndex = FMath::RandRange(0, WayPoints.Num() - 1);
    
    return WayPoints[RandomIndex];
}

void URWayPointManager::Initialize()
{
    FString OutResult;
    
    FRWayPointArray WayPointArray;
    
    FRFileHelper::ReadStringFromJson(TEXT("/WayPoint.json"), OutResult);
    FJsonObjectConverter::JsonObjectStringToUStruct(OutResult, &WayPointArray);
    
    for (const auto& WayPoint : WayPointArray.WayPointData)
    {
        WayPoints.Add(WayPoint);
    }
}
