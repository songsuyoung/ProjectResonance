#include "System/RWayPointManager.h"
#include "RGameInstance.h"

URWayPointManager* URWayPointManager::Get(UObject* Object)
{
    UWorld* World = Object->GetWorld();

    check(World);

    URGameInstance* GameInstance = Cast<URGameInstance>(World->GetGameInstance());

    check(GameInstance);

    return GameInstance->GetWayPointManager();
}
